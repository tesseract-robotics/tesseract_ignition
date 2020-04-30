/*
 * Copyright (C) 2020 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
import QtQuick 2.9
import QtQuick.Controls 2.1
import QtQuick.Dialogs 1.0
import QtQuick.Layouts 1.3
import "qrc:/qml"

GridLayout {
  columns: 6
  columnSpacing: 10
  Layout.minimumWidth: 100
  Layout.minimumHeight: 600
  anchors.fill: parent
  anchors.leftMargin: 10
  anchors.rightMargin: 10

  // Left spacer
  Item {
    Layout.columnSpan: 1
    Layout.rowSpan: 15
    Layout.fillWidth: true
  }

  CheckBox {
    Layout.alignment: Qt.AlignHCenter
    id: showgrid
    Layout.columnSpan: 4
    text: qsTr("Show/Hide Grid")
    checked: true
    onClicked: {
      TesseractGridConfig.OnShow(checked)
    }
  }

  // Right spacer
  Item {
    Layout.columnSpan: 1
    Layout.rowSpan: 15
    Layout.fillWidth: true
  }

  Label {
    Layout.columnSpan: 4
    text: "Cell Count"
    font.bold: true
  }

  Label {
    Layout.columnSpan: 2
    id: vercelltext
    text: "Vertical"
  }

  IgnSpinBox {
    Layout.columnSpan: 2
    id: verticalCellCount
    maximumValue: 1000
    minimumValue: 0
    value: 0
    onEditingFinished: TesseractGridConfig.UpdateVCellCount(verticalCellCount.value)
  }

  Label {
    Layout.columnSpan: 2
    id: honcelltext
    text: "Horizontal"
  }

  IgnSpinBox {
    Layout.columnSpan: 2
    id: horizontalCellCount
    maximumValue: 1000
    minimumValue: 1
    value: 20
    onEditingFinished: TesseractGridConfig.UpdateHCellCount(horizontalCellCount.value)
  }

  Label {
    Layout.columnSpan: 4
    id: celllengthtext
    text: "Cell Length"
    font.bold: true
  }

  Label {
    Layout.columnSpan: 2
    id: length
    text: "Length (m)"
  }
  IgnSpinBox {
    Layout.columnSpan: 2
    id: cellLength
    maximumValue: 1000.00
    minimumValue: 0.01
    value: 1.00
    decimals: 2
    stepSize: 0.01
    onEditingFinished: TesseractGridConfig.UpdateCellLength(cellLength.value)
  }

  Label {
    Layout.columnSpan: 2
    id: cartesian
    font.bold: true
    text: "Position (m)"
  }

  Label {
    Layout.columnSpan: 2
    id: principal
    text: "Rotation (rad)"
    font.bold: true
  }

  Label {
    text: "X"
  }

  IgnSpinBox {
    id: x
    value: 0.00
    maximumValue: 1000.00
    minimumValue: -1000.00
    decimals: 2
    stepSize: 0.01
    onEditingFinished: TesseractGridConfig.SetPose(x.value, y.value, z.value, roll.value, pitch.value, yaw.value)
  }

  Label {
    text: "Roll"
  }

  IgnSpinBox {
    id: roll
    maximumValue: 6.28
    minimumValue: 0.00
    value: 0.00
    decimals: 2
    stepSize: 0.01
    onEditingFinished: TesseractGridConfig.SetPose(x.value, y.value, z.value, roll.value, pitch.value, yaw.value)
  }

  Label {
    text: "Y"
  }

  IgnSpinBox {
    id: y
    value: 0.00
    maximumValue: 1000.00
    minimumValue: -1000.00
    decimals: 2
    stepSize: 0.01
    onEditingFinished: TesseractGridConfig.SetPose(x.value, y.value, z.value, roll.value, pitch.value, yaw.value)
  }

  Label {
    text: "Pitch"
  }

  IgnSpinBox {
    id: pitch
    maximumValue: 6.28
    minimumValue: 0.00
    value: 0.00
    decimals: 2
    stepSize: 0.01
    onEditingFinished: TesseractGridConfig.SetPose(x.value, y.value, z.value, roll.value, pitch.value, yaw.value)
  }

  Label {
    text: "Z"
  }

  IgnSpinBox {
    id: z
    value: 0.00
    maximumValue: 1000.00
    minimumValue: -1000.00
    decimals: 2
    stepSize: 0.01
    onEditingFinished: TesseractGridConfig.SetPose(x.value, y.value, z.value, roll.value, pitch.value, yaw.value)
  }

  Label {
    text: "Yaw"
  }

  IgnSpinBox {
    id: yaw
    maximumValue: 6.28
    minimumValue: 0.00
    value: 0.00
    decimals: 2
    stepSize: 0.01
    onEditingFinished: TesseractGridConfig.SetPose(x.value, y.value, z.value, roll.value, pitch.value, yaw.value)
  }

  Label {
    Layout.columnSpan: 4
    text: "Color"
    font.bold: true
  }

  Label {
    text: "R"
  }

  IgnSpinBox {
    id: r
    maximumValue: 1.00
    minimumValue: 0.00
    value: 0.7
    stepSize: 0.01
    decimals: 2
    onEditingFinished: TesseractGridConfig.SetColor(r.value, g.value, b.value, a.value)
  }

  Label {
    text: "G"
  }

  IgnSpinBox {
    id: g
    maximumValue: 1.00
    minimumValue: 0.00
    value: 0.7
    stepSize: 0.01
    decimals: 2
    onEditingFinished: TesseractGridConfig.SetColor(r.value, g.value, b.value, a.value)
  }

  Label {
    text: "B"
  }

  IgnSpinBox {
    id: b
    maximumValue: 1.00
    minimumValue: 0.00
    value: 0.7
    stepSize: 0.01
    decimals: 2
    onEditingFinished: TesseractGridConfig.SetColor(r.value, g.value, b.value, a.value)
  }

  Label {
    text: "A"
  }

  IgnSpinBox {
    id: a
    maximumValue: 1.00
    minimumValue: 0.00
    value: 1.0
    stepSize: 0.01
    decimals: 2
    onEditingFinished: TesseractGridConfig.SetColor(r.value, g.value, b.value, a.value)
  }

  Button {
    Layout.alignment: Qt.AlignHCenter
    Layout.columnSpan: 4
    id: color
    text: qsTr("Custom Color")
    onClicked: colorDialog.open()

    ColorDialog {
      id: colorDialog
      title: "Choose a grid color"
      visible: false
      onAccepted: {
        r.value = colorDialog.color.r
        g.value = colorDialog.color.g
        b.value = colorDialog.color.b
        a.value = colorDialog.color.a
        TesseractGridConfig.SetColor(colorDialog.color.r, colorDialog.color.g, colorDialog.color.b, colorDialog.color.a)
        colorDialog.close()
      }
      onRejected: {
        colorDialog.close()
      }
    }
  }

  // Bottom spacer
  Item {
    Layout.columnSpan: 4
    Layout.fillHeight: true
  }
}
