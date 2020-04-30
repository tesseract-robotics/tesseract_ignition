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

## Setup
This application uses the ros_scxml library which itself uses the Qt 5.12 library.  Assuming that you have followed the installation instructions for this package then you should've already configure the corresponding environment variables:
```
export PATH="[path to QT5.12]/cmake:$PATH"
export LD_LIBRARY_PATH=[path to QT5.12]/gcc_64/lib:$LD_LIBRARY_PATH
export QT_PLUGIN_PATH=[path to QT5.12]/gcc_64/plugins
export QML2_IMPORT_PATH="[path to QT5.12]/qml
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
