/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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
import QtQuick.Window 2.2
import Ruqola 1.0


//import org.kde.kirigami 2.8 as Kirigami

QQC2.Dialog {
    id: addUserDialog

    property QtObject completerModel
    signal searchUserName(string pattern)
    signal addUser(string userId, string rid, string channelType)

    property QtObject roomInfo
    property string roomId: ""

    title: qsTr("Add Users")
    standardButtons: QQC2.Dialog.Close

    width: parent.width * 9 / 10
    height: parent.height * 9 / 10
    anchors.centerIn: parent

    modal: true
    focus: true

    function initializeAndOpen()
    {
        username.text = "";
        completerModel.clear();
        username.forceActiveFocus();
        open();
    }

    contentItem: ColumnLayout {
//        Kirigami.SearchField {
        QQC2.TextField{
            id: username
            placeholderText: qsTr("Search User...")
            Layout.fillWidth: true
            onTextChanged: {
                addUserDialog.searchUserName(username.text)
            }
        }
        ListView {
            id: listview
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            model: completerModel
            delegate:
//                Kirigami.BasicListItem {
                QQC2.ItemDelegate{
//                reserveSpaceForIcon: false
//                reserveSpaceForLabel: false
                RowLayout {
//                    Kirigami.Icon {
                    QQC2.ToolButton{
                        icon.source: "list-add"
                        height: Kirigami.Units.iconSizes.medium
                        width: height
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                addUserDialog.addUser(userid, roomId, roomInfo.channelType)
                            }
                        }
                    }

//                    Kirigami.Icon {
                    QQC2.ToolButton{
                        icon.source: iconstatus
                        height: Kirigami.Units.iconSizes.medium
                        width: height
                    }
                    QQC2.Label {
                        text: username
                    }
                }
            }
        }
    }
}
