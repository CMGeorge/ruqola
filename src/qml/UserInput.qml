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
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import KDE.Ruqola.RocketChatAccount 1.0
import KDE.Ruqola.DDPClient 1.0
import org.kde.kirigami 2.1 as Kirigami

RowLayout {
    id: footerItem
    anchors.left: parent.left
    anchors.right: parent.right
    property QtObject rcAccount
    property alias messageLineText: messageLine.text
    property string messageId
    property string savePreviousMessage

    signal textEditing(string str)
    signal uploadFile()
    signal clearUnreadMessages()
    
    height: 2*Kirigami.Units.largeSpacing
    
    function setOriginalMessage(messageStr)
    {
        messageLine.text = messageStr
        savePreviousMessage = messageStr
        messageLine.selectAll()
    }

    Kirigami.Icon {
        id: attachment

        enabled: selectedRoomID !== ""
        source: "document-send-symbolic"
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        width: height
        height: messageLine.height/2
        MouseArea {
            anchors.fill: parent
            onClicked: {
                footerItem.uploadFile()
            }
        }
    }
    MessageLine {
        id: messageLine
    }

    InputTextCompleter {
        id: inputTextCompleter
    }
    
    Kirigami.Icon {
        source: "mail-sent"
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        width: height
        MouseArea {
            anchors.fill: parent
            onClicked: {
                messageLine.accepted()
            }
        }
    }
}
