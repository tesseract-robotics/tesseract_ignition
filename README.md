## QML Notes
* Use Label instead of Text because it respects Themes.
* Use Frame instead of Rectangel because it respects Themes.
  * Frame will consume mouse events so if that is not desirable use control as below
    ``` qml
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
    ```
* If using Theme do not manually set color, it is controlled by the Theme.
* If using data models write your qml code to access the model through the object. This way if the model is assigned a
  different model your UI will continue to work.

## Buiding from source
For ignition to find the plugins built by this package you must set the following environment variable.

``` bash
export IGN_GUI_PLUGIN_PATH=/home/<username>/<workspace>/devel/lib/
```

## Snap Download

<iframe src="https://snapcraft.io/tesseract-ignition/embedded?button=black&channels=true&summary=true&screenshot=true" frameborder="0" width="100%" height="900px" style="border: 1px solid #CCC; border-radius: 2px;"></iframe>

## Use Snap with ROS workspace
  * source workspace
  * tesseract-ignition.tesseract-setup-wizard
  * Note: If you want to include another package not in your path you can leverages the environment variable TSW_RESOURCE_PATH to do this shown below.
    * TSW_RESOURCE_PATH=/home/user/workspace/src/package tesseract-ignition.tesseract-setup-wizard

## Snap Creation

* Install Snapcraft
  * Need version 4.0 which is not release so use this command to get it `sudo snap refresh --candidate snapcraft`
* Setup Workspace (This is already setup but the procedure is below.)
  * Create directory snapcraft
  * Create directory snapcraft/snap
  * Create directory snapcraft/snap/local
  * Create rosinstall snapcraft/snap/local/ignition_snap.rosinstall
    * Create a rosinstall that pull all resources for your snap
  * Create config snapcraft/snap/snapcraft.yaml
* Build Snap
  * 'cd snapcraft'
  * 'SNAPCRAFT_BUILD_ENVIRONMENT_MEMORY=16G snapcraft --bind-ssh --enable-experimental-package-repositories`
  * Notes:
    * The `--bind-ssh` allows access to your local ssh keys for pulling resources from our internal gitlab server.
    * The `--enable-experimental-package-repositories` because we are using version 4.0 which has not been released.
* Test Snap
  * `sudo snap install --dangerous --classic tesseract-ignition_X.X_amd64.snap`
  * `sudo snap connect tesseract-ignition:hostfs-opt-ros`
  * `tesseract.tesseract-setup-wizard`
* Upload to Snap Store
  * snapcraft login
  * snapcraft upload tesseract-ignition_X.X_amd64.snap --release [stable | candidate | beta | edge]
    * The risk-levels have the following meaning:
      * stable: for the vast majority of users running on production environments. Releases at this risk level are as stable as they will ever get, according to the project’s standards. Important software will only reach this stage once it’s ready for production use and may be used in products. There is an implied promise to avoid any changes that would disrupt those usages.
      * candidate: for users who need to test updates prior to stable deployment, or those verifying whether a specific issue has been resolved. Releases in candidate are considered almost ready for going into stable, but need some additional real world experimentation before they move forward. Software reaching this stage will typically have passed all available QA and review processes, since users following it expect a high stability level. Should almost never break.
      * beta: for users wanting to test the latest features, typically outside of a production environment. Beta is the first level towards the stabilisation of what was before a fast moving stream of changes. Specific projects may have slightly different terminology for such releases (alpha, beta, etc) but all of these are welcome on this risk level. These releases will almost certainly have passed some sort of review and QA, but may still have unfinished parts. Breaking changes are still relatively common here.
      * edge: for users wanting to closely track development. Edge releases often include a moving stream of changes without QA or review promises and are typically built automatically by a CI process from an arbitrary source code snapshot. Often the CI will only publish after some sort of automatic QA passed, and code reviews remain a good practice, but these are project specific. Assume edge releases may break often.

## Snap Notes:

  * Sometimes paths are hardcoded in software based on when they are build causing issues when bundling. This can be addressed leveraging [snap-layouts](https://snapcraft.io/docs/snap-layouts).
  * The `snap run --shell <command>` example `snap run --shell tesseract-ignition.tesseract-setup-wizard` thing is a super useful way to test out the confinement profile of a given command, I use it all the time.It just opens a shell instead of running the command.
  * The confinement-levels
    * strict: Used by the majority of snaps. Strictly confined snaps run in complete isolation, up to a minimal access level that’s deemed always safe. Consequently, strictly confined snaps can not access your files, network, processes or any other system resource without requesting specific access via an interface (see below).
    * classic: Allows access to your system’s resources in much the same way traditional packages do. To safeguard against abuse, publishing a classic snap requires manual approval, and installation requires the --classic command line argument.
    * devmode: A special mode for snap creators and developers. A devmode snap runs as a strictly confined snap with full access to system resources, and produces debug output to identify unspecified interfaces. Installation requires the --devmode command line argument. Devmode snaps cannot be released to the stable channel, do not appear in search results, and do not automatically refresh.
  * Local intall location is /snap/<snap-name>
    * You can dynamically load libraries (plugins) from here
    * You can also link against packages in this directory by adding it to the appropriate environment variables
