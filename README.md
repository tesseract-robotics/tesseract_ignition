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
