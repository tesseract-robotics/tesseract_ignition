import textwrap
import logging

from snapcraft.plugins.v1 import catkin as catkin_ros
from snapcraft.plugins.v1 import _ros

logger = logging.getLogger(__name__)


class CatkinPlugin(catkin_ros.CatkinPlugin):
    @classmethod
    def schema(cls):
        schema = super().schema()

        schema["properties"]["skip-keys"] = {
            "type": "array",
            "minitems": 1,
            "uniqueItems": True,
            "items": {"type": "string"},
            "default": [],
        }

        return schema

    def env(self, root):
        env = super().env(root)

        # Backup any existing ROS_PACKAGE_PATH
        env.insert(0, 'HOST_ROS_PACKAGE_PATH="$ROS_PACKAGE_PATH"')

        # At the end of the process, rewrite the host's ROS_PACKAGE_PATH
        # (if any) to point to the right place in the snap
        env[-1] = env[-1] + "\n\n" + textwrap.dedent("""\
            set -f; IFS=:
            for path in $HOST_ROS_PACKAGE_PATH; do
                if [ "$path" != "${path#/opt/ros/}" ]; then
                    REWRITTEN_HOST_ROS_PACKAGE_PATH="${REWRITTEN_HOST_ROS_PACKAGE_PATH:+$REWRITTEN_HOST_ROS_PACKAGE_PATH:}/var/lib/snapd/hostfs${path}"
                else
                    REWRITTEN_HOST_ROS_PACKAGE_PATH="${REWRITTEN_HOST_ROS_PACKAGE_PATH:+$REWRITTEN_HOST_ROS_PACKAGE_PATH:}${path}"
                fi
            done
            set +f; unset IFS
            ROS_PACKAGE_PATH="${REWRITTEN_HOST_ROS_PACKAGE_PATH:+$REWRITTEN_HOST_ROS_PACKAGE_PATH:}$ROS_PACKAGE_PATH"
            """)
        return env

    def _setup_dependencies(self, rosdep, catkin):
        # Parse the Catkin packages to pull out their system dependencies
        system_dependencies = self._find_system_dependencies2(rosdep, catkin)

        # If the package requires roscore, resolve it into a system dependency
        # as well.
        if self.options.include_roscore:
            roscore = rosdep.resolve_dependency("ros_core")
            if roscore:
                for dependency_type, dependencies in roscore.items():
                    if dependency_type not in system_dependencies:
                        system_dependencies[dependency_type] = set()
                    system_dependencies[dependency_type] |= dependencies
            else:
                raise catkin_ros.CatkinCannotResolveRoscoreError()

        # Pull down and install any apt dependencies that were discovered
        self._setup_apt_dependencies(system_dependencies.get("apt"))

        # Pull down and install any pip dependencies that were discovered
        self._setup_pip_dependencies(system_dependencies.get("pip"))

    def _find_system_dependencies2(self, rosdep, catkin):
        """Find system dependencies for a given set of Catkin packages."""

        resolved_dependencies = {}
        dependencies = set()

        logger.info("Determining system dependencies for Catkin packages...")
        if self.catkin_packages is not None:
            for package in self.catkin_packages:
                # Query rosdep for the list of dependencies for this package
                dependencies |= rosdep.get_dependencies(package)
        else:
            # Rather than getting dependencies for an explicit list of packages,
            # let's get the dependencies for the entire workspace.
            dependencies |= rosdep.get_dependencies()

        # Remove skip keys
        filtered_dependencies = [x for x in dependencies if x not in self.options.skip_keys]

        for dependency in filtered_dependencies:
            self._resolve_package_dependencies(
                self.catkin_packages, dependency, catkin, rosdep, resolved_dependencies
            )

        # We currently have nested dict structure of:
        #    dependency name -> package type -> package names
        #
        # We want to return a flattened dict of package type -> package names.
        flattened_dependencies = {}
        for dependency_types in resolved_dependencies.values():
            for key, value in dependency_types.items():
                if key not in flattened_dependencies:
                    flattened_dependencies[key] = set()
                flattened_dependencies[key] |= value

        # Finally, return that dict of dependencies
        return flattened_dependencies

    def _resolve_package_dependencies(self, catkin_packages, dependency, catkin, rosdep, resolved_dependencies):
        # No need to resolve this dependency if we know it's local, or if
        # we've already resolved it into a system dependency
        if dependency in resolved_dependencies or (
            catkin_packages and dependency in catkin_packages
        ):
            return

        if self._dependency_is_in_underlay(catkin, dependency):
            # Package was found-- don't pull anything extra to satisfy
            # this dependency.
            logger.debug("Satisfied dependency {!r} in underlay".format(dependency))
            return

        # In this situation, the package depends on something that we
        # weren't instructed to build. It's probably a system dependency,
        # but the developer could have also forgotten to tell us to build
        # it.
        try:
            these_dependencies = rosdep.resolve_dependency(dependency)
        except _ros.rosdep.RosdepDependencyNotResolvedError:
            raise catkin_ros.CatkinInvalidSystemDependencyError(dependency)

        for key, value in these_dependencies.items():
            if key not in catkin_ros._SUPPORTED_DEPENDENCY_TYPES:
                raise catkin_ros.CatkinUnsupportedDependencyTypeError(key, dependency)

            resolved_dependencies[dependency] = {key: value}

    def _dependency_is_in_underlay(self, catkin, dependency):
        if catkin:
            # Before trying to resolve this dependency into a system
            # dependency, see if it's already in the underlay.
            try:
                catkin.find(dependency)
            except catkin_ros.CatkinPackageNotFoundError:
                # No package by that name is available
                pass
            else:
                return True
        return False
