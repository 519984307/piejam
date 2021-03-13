// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2020  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.13
import QtQuick.Controls 2.13


Item {

    id: root

    property alias nameLabelWidth: nameLabel.width
    property alias nameLabelText: nameLabel.text
    property alias model: comboBox.model
    property alias currentIndex: comboBox.currentIndex
    property string emptyText: qsTr("Not Available")
    property string unselectedText: qsTr("Select...")

    signal optionSelected(int index)

    Frame {
        id: frame

        anchors.fill: parent

        Label {
            id: nameLabel

            width: 128

            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left

            verticalAlignment: Text.AlignVCenter
            textFormat: Text.PlainText
            font.pixelSize: 18
        }

        ComboBox {
            id: comboBox

            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 8
            anchors.right: parent.right
            anchors.left: nameLabel.right

            displayText: comboBox.count == 0
                         ? emptyText
                         : (comboBox.currentIndex == -1 ? unselectedText : comboBox.currentText)
            enabled: comboBox.count != 0

            onActivated: optionSelected(index)
        }
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
