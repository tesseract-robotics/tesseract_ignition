import textwrap

from snapcraft.plugins.v1 import catkin

class CatkinPlugin(catkin.CatkinPlugin):
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
