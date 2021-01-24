// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2020  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.0
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13

import ".."

Item {
    id: root

    property var model

    implicitWidth: 800
    implicitHeight: 48

    Rectangle {
        id: statusBarBackground

        anchors.fill: parent

        color: Material.color(Material.Grey, Material.Shade800)

        Label {
            id: infoMessageLabel

            anchors.left: parent.left
            anchors.right: midiLearn.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 6

            wrapMode: Text.WordWrap
            textFormat: Text.RichText
            clip: true

            padding: 2
            leftPadding: 4
            rightPadding: 4
            text: Info.message

            background: Rectangle {
                color: Material.background
                radius: 4
            }
        }

        Button {
            id: midiLearn

            width: 38

            anchors.right: audioLoadLabel.left
            checkable: true
            anchors.rightMargin: 6

            text: qsTr("M")

            Binding {
                target: MidiLearn
                property: "active"
                value: midiLearn.checked
            }
        }

        Label {
            id: audioLoadLabel

            width: 64

            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 6

            padding: 2
            leftPadding: 4
            rightPadding: 4
            horizontalAlignment: Text.AlignRight
            text: (root.model.audioLoad * 100).toFixed(1) + " %"

            background: Rectangle {
                color: Material.background
                radius: 4
            }

            Label {
                id: xrunsLabel

                height: 16

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                padding: 2
                leftPadding: 4
                rightPadding: 4
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignBottom
                color: root.model.xruns === 0 ? Qt.rgba(0, 1, 0, 1) : Qt.rgba(1, 0, 0, 1)
                text: root.model.xruns
            }
        }

    }

    Timer {
        interval: 40
        running: root.visible
        repeat: true
        onTriggered: root.model.requestUpdate()
    }

    Binding {
        when: root.model
        target: root.model
        property: "subscribed"
        value: root.visible
    }

    Binding {
        when: root.model
        target: Info
        property: "logMessage"
        value: root.model.logMessage
    }
}
