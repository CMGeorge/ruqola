/*
 * Copyright (C) 2019 Laurent Montel <montel@kde.org>
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

import KDE.Ruqola.RuqolaUtils 1.0
import KDE.Ruqola.Message 1.0

import QtQuick.Controls 2.2
import org.kde.kirigami 2.7 as Kirigami
import KDE.Ruqola.ExtraColors 1.0
import QtQuick.Layouts 1.12
import KDE.Ruqola.RocketChatAccount 1.0
import KDE.Ruqola.DebugCategory 1.0
Rectangle {

    id: dicussionMessageDelegateItem
    property int numberofmessages
    property string lastmessage
    property string discussionid
    property string description

    color: RuqolaSingleton.backgroundColor
    implicitHeight: 4*Kirigami.Units.smallSpacing + loaded.item.implicitHeight

    implicitWidth: 150

    Loader {
        id: loaded
        anchors.fill: parent

        Component.onCompleted: {
            setSource("DiscussionMessage.qml",
                      {
                          numberofmessages: numberofmessages,
                          lastmessage: lastmessage,
                          discussionid: discussionid,
                          description: description
                      })
        }
    }
}
