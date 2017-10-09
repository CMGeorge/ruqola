/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

import QtQuick 2.0

import QtQuick.Controls 2.2
import org.kde.kirigami 2.1 as Kirigami

import QtQuick.Layouts 1.1
import KDE.Ruqola.Ruqola 1.0
import KDE.Ruqola.RuqolaUtils 1.0
import KDE.Ruqola.ExtraColors 1.0
import "../js/message.js" as MessageScript;

Rectangle {

    property string i_messageText
    property string i_messageID
    property string i_username
    property string i_aliasname
    property string i_avatar
    property var i_timestamp
    property var i_urls
    property var i_attachments
    property var i_date

    signal linkActivated(string link)

    id: messageMain

    color: RuqolaSingleton.backgroundColor;

    NewDateLabel {
        id: newDateRect
        date: i_date
    }

    Rectangle {
        Layout.alignment: Qt.AlignCenter
        width: textLabel.implicitWidth + 6*Kirigami.Units.smallSpacing
        height: textLabel.height

        color: Kirigami.Theme.disabledTextColor
        radius: 4*Kirigami.Units.smallSpacing

        Kirigami.Label {
            id: textLabel

            color: Kirigami.Theme.textColor
            opacity: 1

            anchors.centerIn: parent
            anchors.leftMargin: Kirigami.Units.smallSpacing
            anchors.rightMargin: Kirigami.Units.smallSpacing

            width: Math.min(implicitWidth, parent.width - Kirigami.Units.largeSpacing)

            text: i_username + i18n(" Image ")

            wrapMode: Label.Wrap

            renderType: Text.NativeRendering

            onLinkActivated: messageMain.linkActivated(link)
        }
    }
}
