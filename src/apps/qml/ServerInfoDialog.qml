/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Window 2.0
import KDE.Ruqola.RocketChatAccount 1.0
import KDE.Ruqola.ServerConfigInfo 1.0

QQC2.Dialog {
    id: serverinfodialog
    title: i18n("Server Info")
    standardButtons: QQC2.Dialog.Close
    property QtObject rcAccount

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    modal: true
    focus: true
    ColumnLayout {
        QQC2.Label {
            text: rcAccount ? i18n("Account name: %1", rcAccount.serverConfigInfo().serverName) : ""
            font.pointSize: 15
        }
        QQC2.Label {
            text: rcAccount ? i18n("User name: %1", rcAccount.serverConfigInfo().userName) : ""
            font.pointSize: 15
        }
        QQC2.Label {
            text: rcAccount ? i18n("Server version: %1", rcAccount.serverConfigInfo().serverVersionStr) : ""
            font.pointSize: 15
        }
    }

}
