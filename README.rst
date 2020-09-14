################################
Welcome to the Tesseract Igntion
################################

Buiding from source
===================
Install Ignition Libraries:

.. code-block:: bash

   sudo sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list'
   wget http://packages.osrfoundation.org/gazebo.key -O - | sudo apt-key add -
   sudo apt-get update
   sudo apt install libignition-common3-dev libignition-common3-av-dev libignition-common3-events-dev libignition-common3-profiler-dev libignition-msgs5-dev libignition-rendering3-dev libignition-rendering3-ogre1-dev libignition-rendering3-ogre2-dev libignition-transport8-dev libignition-gui3-dev libignition-math6-eigen3-dev

For ignition to find the plugins built by this package you must set the following environment variable.

.. code-block:: bash

   export IGN_GUI_PLUGIN_PATH=/home/<username>/<workspace>/devel/lib/

Snap Download
=============

See `Wiki <https://tesseract-ignition.readthedocs.io>`_ for link to the `Snap Store <https://snapcraft.io/tesseract-ignition>`_ to download.

Use Snap with ROS workspace
===========================

* source workspace
* tesseract-ignition.tesseract-setup-wizard
* Note: If you want to include another package not in your path you can leverages the environment variable TSW_RESOURCE_PATH to do this shown below.

  * TSW_RESOURCE_PATH=/home/user/workspace/src/package tesseract-ignition.tesseract-setup-wizard

Snap Creation
=============

Install Snapcraft
-----------------

* Need version 4.0 which is not release so use this command to get it `sudo snap refresh --candidate snapcraft`
* Setup Workspace (This is already setup but the procedure is below.)
* Create directory snap
* Create directory snap/local
* Create rosinstall snap/local/ignition_snap.rosinstall that pull all resources for your snap
* Create config snap/snapcraft.yaml
* Also once registared with snapcraft.io you can setup automatic builds

Build Snap
----------

* `cd snapcraft`
* `SNAPCRAFT_BUILD_ENVIRONMENT_MEMORY=16G snapcraft --enable-experimental-package-repositories`
* Alternative Using LXD
  * `SNAPCRAFT_BUILD_ENVIRONMENT_MEMORY=16G snapcraft --enable-experimental-package-repositories --use-lxd`

.. note::

   * The `--enable-experimental-package-repositories` because we are using version 4.0 which has not been released.

Test Snap
---------

* `snap install --dangerous --classic tesseract-ignition_X.X_amd64.snap`
* `snap connect tesseract-ignition:hostfs-opt-ros`
* Verify connection `snap connections tesseract-ignition`

  .. code-block:: bash

     Interface                             Plug                                      Slot                                                  Notes
     content[icon-themes]                  tesseract-ignition:icon-themes            gtk-common-themes:icon-themes                         -
     content[kde-frameworks-5-core18-all]  tesseract-ignition:kde-frameworks-5-plug  kde-frameworks-5-core18:kde-frameworks-5-core18-slot  -
     content[sound-themes]                 tesseract-ignition:sound-themes           gtk-common-themes:sound-themes                        -
     desktop                               tesseract-ignition:desktop                :desktop                                              -
     desktop-legacy                        tesseract-ignition:desktop-legacy         :desktop-legacy                                       -
     home                                  tesseract-ignition:home                   :home                                                 -
     network                               tesseract-ignition:network                :network                                              -
     opengl                                tesseract-ignition:opengl                 :opengl                                               -
     removable-media                       tesseract-ignition:removable-media        -                                                     -
     system-files                          tesseract-ignition:hostfs-opt-ros         :system-files                                         manual
     wayland                               tesseract-ignition:wayland                :wayland                                              -
     x11                                   tesseract-ignition:x11                    :x11                                                  -


* `tesseract-ignition.tesseract-setup-wizard`

Upload to Snap Store
--------------------

* snapcraft login
* snapcraft upload tesseract-ignition_X.X_amd64.snap --release [stable | candidate | beta | edge]

.. note::

   The risk-levels have the following meaning:

   * stable: for the vast majority of users running on production environments. Releases at this risk level are as stable as they will ever get, according to the project’s standards. Important software will only reach this stage once it’s ready for production use and may be used in products. There is an implied promise to avoid any changes that would disrupt those usages.
   * candidate: for users who need to test updates prior to stable deployment, or those verifying whether a specific issue has been resolved. Releases in candidate are considered almost ready for going into stable, but need some additional real world experimentation before they move forward. Software reaching this stage will typically have passed all available QA and review processes, since users following it expect a high stability level. Should almost never break.
   * beta: for users wanting to test the latest features, typically outside of a production environment. Beta is the first level towards the stabilisation of what was before a fast moving stream of changes. Specific projects may have slightly different terminology for such releases (alpha, beta, etc) but all of these are welcome on this risk level. These releases will almost certainly have passed some sort of review and QA, but may still have unfinished parts. Breaking changes are still relatively common here.
   * edge: for users wanting to closely track development. Edge releases often include a moving stream of changes without QA or review promises and are typically built automatically by a CI process from an arbitrary source code snapshot. Often the CI will only publish after some sort of automatic QA passed, and code reviews remain a good practice, but these are project specific. Assume edge releases may break often.

Snap Notes
==========

* Sometimes paths are hardcoded in software based on when they are build causing issues when bundling. This can be addressed leveraging [snap-layouts](https://snapcraft.io/docs/snap-layouts).
* The `snap run --shell <command>` example `snap run --shell tesseract-ignition.tesseract-setup-wizard` thing is a super useful way to test out the confinement profile of a given command, I use it all the time.It just opens a shell instead of running the command.
* The confinement-levels

  * strict: Used by the majority of snaps. Strictly confined snaps run in complete isolation, up to a minimal access level that’s deemed always safe. Consequently, strictly confined snaps can not access your files, network, processes or any other system resource without requesting specific access via an interface (see below).
  * classic: Allows access to your system’s resources in much the same way traditional packages do. To safeguard against abuse, publishing a classic snap requires manual approval, and installation requires the --classic command line argument.
  * devmode: A special mode for snap creators and developers. A devmode snap runs as a strictly confined snap with full access to system resources, and produces debug output to identify unspecified interfaces. Installation requires the --devmode command line argument. Devmode snaps cannot be released to the stable channel, do not appear in search results, and do not automatically refresh.
* Local intall location is /snap/<snap-name>

  * You can dynamically load libraries (plugins) from here
  * You can also link against packages in this directory by adding it to the appropriate environment variables

QML Notes
=========

* Use Label instead of Text because it respects Themes.
* Use Frame instead of Rectangel because it respects Themes.

  * Frame will consume mouse events so if that is not desirable use control as below

    .. code-block:: qml

       Control {
           anchors.centerIn: parent
           width: 200
           height: width
           background: Rectangle {
               color: parent.Material.background
               border.color: parent.Material.foreground
           }

           Label {
               anchors.centerIn: parent
               text: "some text"
               font.pixelSize: 40
           }
       }

* If using Theme do not manually set color, it is controlled by the Theme.
* If using data models write your qml code to access the model through the object. This way if the model is assigned a
  different model your UI will continue to work.
