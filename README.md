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

## Setup
For ignition to find the plugins built by this package you must set the following environment variable.

``` bash
export IGN_GUI_PLUGIN_PATH=/home/larmstrong/catkin_ws/trajopt_only_ws/devel/lib/
```

## Questions

* How best to layout things out since everything is plugins.
* I assume you should use Transport to communicate between plugins?
* Does the Scene keep a change history?
* Currently the SceneManager publishes the scene but is there any reason not to expose services to modify the scene?
  * Also is there any examples of populating a Scene Message?
* How best to leverage gizmo's.
* Is there any issue with using ROS 1 communication within a plugin.
  * The other way around is there an issue with using Transport library within a ros node.


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
    * The `--build-ssh` allows access to your local ssh keys for pulling resources from our internal gitlab server.
    * The `--enable-experimental-package-repositories` because we are using version 4.0 which has not been released.
* Test Snap
  * `snap install --dangerous --devmode tesseract_0.1_amd64.snap`
  * tesseract.tesseract-setup-wizard
* Upload to Snap Store
* Notes:
  * Sometimes paths are hardcoded in software based on when they are build causing issues when bundling. This can be addressed leveraging [snap-layouts](https://snapcraft.io/docs/snap-layouts).
  * The `snap run --shell <command>` example `snap run --shell tesseract.tesseract-setup-wizard` thing is a super useful way to test out the confinement profile of a given command, I use it all the time.It just opens a shell instead of running the command.
