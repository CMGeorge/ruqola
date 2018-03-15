/*
 
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


import QtQuick 2.9

import org.kde.kirigami 2.1 as Kirigami
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Layouts 1.1
import KDE.Ruqola.ExtraColors 1.0
import KDE.Ruqola.DebugCategory 1.0
MessageBase {
    id: messageMain

    property string i_systemMessageType

    function getTextFor(type) {
        if (type === "uj") {
            return i18n("%1 has joined the channel", i_username);
        } else if (type === "ul") {
            return i18n("%1 has left the channel", i_username);
        } else if (type === "room_changed_topic") {
            if (i_messageText == "") {
                return i18n("Topic was cleared by: %1", i_username)
            } else {
                return i18n("%2 changed topic to: <i>%1</i>", i_messageText, i_username)
            }
        } else if (type === "au") {
            return i18n("%2 added %1 to the conversation", i_messageText, i_username)
        } else if (type === "r") {
            return i18n("%2 changed room name to <a href=\"ruqola:/room/%1\">#%1</a>", i_messageText, i_username)
        } else if (type === "ru") {
            return i18n("%2 removed user %1", i_messageText, i_username)
        } else if (type === "room_changed_description") {
            if (i_messageText == "") {
                return i18n("Description was cleared by %1", i_username)
            } else {
                return i18n("%2 changed room description to %1", i_messageText, i_username)
            }
        } else if (type === "room_changed_announcement") {
            if (i_messageText == "") {
                return i18n("Announcement was cleared by %1", i_username)
            } else {
                return i18n("%2 changed room announcement to %1", i_messageText, i_username)
            }
        } else if (type === "room_changed_privacy") {
            return i18n("%2 changed room privacy to %1", i_messageText, i_username)
        } else if (type === "jitsi_call_started") {
            return i18n("Click to join to video")
        } else if (type === "rm") {
            return i18n("Message Deleted")
        } else if (type === "message_pinned") {
            return i18n("Message Pinned")
        } else if (type === "otr") {
            return i18n("Encrypted Message")
        } else if (type === "user-unmuted") {
            //TODO improve it
            return i18n("%1 was unmuted", i_username)
        } else if (type === "user-muted") {
            //TODO improve it
            return i18n("%1 was muted", i_username)
        } else {
            console.log(RuqolaDebugCategorySingleton.category, "Unkown type for message");
            console.log(RuqolaDebugCategorySingleton.category, "type : " + type);
            console.log(RuqolaDebugCategorySingleton.category, "message: " + i_messageText)
            return i18n("Unknown action!");
        }
    }

    RowLayout {
        Item {
            Layout.fillWidth: true
        }

        Rectangle {
            Layout.alignment: Qt.AlignCenter
            width: textLabel.implicitWidth + 6*Kirigami.Units.smallSpacing
            height: textLabel.height

            color: Kirigami.Theme.disabledTextColor
            radius: 4*Kirigami.Units.smallSpacing

            QQC2.Label {
                id: textLabel

                color: Kirigami.Theme.textColor
                opacity: 1

                anchors.centerIn: parent
                anchors.leftMargin: Kirigami.Units.smallSpacing
                anchors.rightMargin: Kirigami.Units.smallSpacing

                width: Math.min(implicitWidth, parent.width - Kirigami.Units.largeSpacing)

                text: getTextFor(i_systemMessageType)

                wrapMode: QQC2.Label.Wrap

                renderType: Text.NativeRendering
            }
        }
        Item {
            Layout.fillWidth: true
        }
        TimestampText {
            id: timestampText
            timestamp: i_timestamp
        }
    }
}
