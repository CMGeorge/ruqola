/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Window 2.0

import KDE.Ruqola.RocketChatAccount 1.0
import KDE.Ruqola.SearchChannelFilterProxyModel 1.0
import org.kde.kirigami 2.4 as Kirigami

QQC2.Dialog {
    id: searchChannelDialog

    property QtObject searchChannelModel
    signal searchChannel(string pattern)
    signal openChannel(string channelname, string channelid, var channeltype)

    title: i18n("Search Channel")
    standardButtons: QQC2.Dialog.Close

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    modal: true
    focus: true
    function initializeAndOpen()
    {
        channelnametext.text = "";
        open();
    }

    ColumnLayout {
        QQC2.TextField {
            id: channelnametext
            selectByMouse: true
            focus: true
            Layout.minimumHeight: Layout.maximumHeight
            Layout.maximumHeight: Kirigami.Units.iconSizes.smallMedium + Kirigami.Units.smallSpacing * 2
            Layout.fillWidth: true
            placeholderText: i18n("Search Channel...")
            onAccepted: {
                searchChannelDialog.searchChannel(channelnametext.text)
            }
        }
        ListView {
            id: listview
            width: 300;
            height: 200
            clip: true

            model: searchChannelModel
            delegate:
                RowLayout {
                Kirigami.Icon {
                    source: "list-add"
                    //FIXME
                    height: 22
                    width: 22
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            searchChannelDialog.openChannel(channelname, channelid, channeltype)
                        }
                    }
                }
                Kirigami.Icon {
                    source: iconname
                    //FIXME icon size
                    height: 22
                    width: 22
                }
                QQC2.Label {
                    text: channelname
                }
            }
        }
    }
}
