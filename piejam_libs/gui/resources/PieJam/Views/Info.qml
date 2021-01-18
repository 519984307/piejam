// PieJam - An audio mixer for Raspberry Pi.
// SPDX-FileCopyrightText: 2020  Dimitrij Kotrev
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.13
import QtQuick.Controls 2.13

TopPane {
    id: root

    // model:
    //   .audioLoad : real
    //   .xruns : int
    //   .logData : list<string>
    //   .subscribed : bool
    property var model

    Label {
        id: lblAudioLoad
        x: 8
        y: 8
        width: 100
        height: 18
        text: qsTr("Audio Load:")
    }

    Label {
        id: lblXRuns
        x: 8
        y: 32
        width: 100
        height: 18
        text: qsTr("Dropouts:")
    }

    Label {
        id: lblAudioLoadValue
        x: 108
        y: 8
        width: 100
        height: 18
        text: (root.model.audioLoad * 100).toFixed(1) + " %"
    }

    Label {
        id: lblXRunsValue
        x: 108
        y: 32
        width: 100
        height: 18
        color: root.model.xruns === 0 ? Qt.rgba(0, 1, 0, 1) : Qt.rgba(1, 0, 0, 1)
        text: root.model.xruns
    }

    Frame {
        id: frame

        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 56
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8

        ListView {
            id: lstLog

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 24
            anchors.bottom: parent.bottom

            clip: true

            model: root.model.logData

            delegate: Label {
                width: 688
                height: 20

                text: modelData

                wrapMode: Text.WordWrap
            }
        }

        Label {
            id: lblLog
            x: 0
            y: 0
            width: 100
            height: 18
            text: qsTr("Log")
            font.bold: true
        }
    }

    Timer {
        interval: 80
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
}
