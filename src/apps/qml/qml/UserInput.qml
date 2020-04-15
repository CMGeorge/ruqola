/*
 
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>
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
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2

//import org.kde.kirigami 2.7 as Kirigami
import "common"
RowLayout {
    id: footerItem
    property QtObject rcAccount
    property alias messageLineText: messageLine.messageLineText
    property alias messageId: messageLine.messageId
    property string threadmessageId
    property string selectedRoomId
    property string selectedThreadMessage
    property QtObject inputTextManager

    signal textEditing(string str)
    signal uploadFile()
    signal clearUnreadMessages()
    
    height: 2*Kirigami.Units.largeSpacing
    
    function setOriginalMessage(messageStr)
    {
        messageLine.setOriginalMessage(messageStr)
    }

//    Kirigami.Icon {
        QQC2.ToolButton{
        id: attachment
        MouseArea {
            hoverEnabled: true
            anchors.fill: parent
            QQC2.ToolTip {
                text: qsTr("Attach Files")
            }
        }

        enabled: selectedRoomId !== ""
        icon.source: "document-send-symbolic"
        width: height
        height: messageLine.height
        MouseArea {
            anchors.fill: parent
            onClicked: {
                footerItem.uploadFile()
            }
        }
    }
    MessageLine {
        id: messageLine
        inputTextManager: footerItem.inputTextManager
        selectedRoomId: footerItem.selectedRoomId
        messageId: footerItem.messageId
        threadmessageId: footerItem.threadmessageId
        selectedThreadMessage: footerItem.selectedThreadMessage
    }
    
    Loader {
        id: emoticonMenuLoader
        active: false
        sourceComponent :EmoticonMenu {
            id: emoticonMenu
            emojiPopupModel: appid.emojiModel
            width: Kirigami.Units.gridUnit * 20
            height: Kirigami.Units.gridUnit * 15
            x: -width + emojiIcon.x + emojiIcon.width
            y: -height
            Component.onCompleted: {
                open()
            }
            onInsertEmoticon: {
                messageLine.insertEmoticon(emoti)
            }
        }
    }
//    Kirigami.Icon {
    QQC2.ToolButton{
        id: emojiIcon
        icon.source: "face-smile"
        width: height
        height: Kirigami.Units.gridUnit*2
        MouseArea {
            hoverEnabled: true
            anchors.fill: parent
            QQC2.ToolTip {
                text: qsTr("Insert Emoji")
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (emoticonMenuLoader.active)
                    emoticonMenuLoader.active = false
                else
                    emoticonMenuLoader.active = true
            }
        }
    }

//    Kirigami.Icon {
    QQC2.TabButton{
        icon.source: messageLine.savePreviousMessage == "" ?  "mail-sent" : "edit-symbolic"
        width: height
        height: Kirigami.Units.gridUnit*2
        MouseArea {
            hoverEnabled: true
            QQC2.ToolTip {
                text: qsTr("Send Message")
            }
            anchors.fill: parent
            onClicked: {
                messageLine.sendMessage()
            }
        }
    }
}
