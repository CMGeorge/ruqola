/*
 
 * Copyright 2016 Riccardo Iaconelli <riccardo@kde.org>
 * Copyright 2017-2018 Montel Laurent <montel@kde.org>
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
import org.kde.kirigami 2.7 as Kirigami

import QtQuick.Layouts 1.12
import Ruqola 1.0
import QtQuick.Controls 2.5 as QQC2

Kirigami.BasicListItem {
    id: root

    property int d_unread: 0;
    property int d_userMentions: 0;
    property string d_name: "roomName";
    property bool d_selected: false;
    property string d_roomID : "";
    property string d_type: "";
    property bool d_open: false;
    property bool d_alert: false;
    property var d_icon
    
    property bool d_editingMode: false;
    
    
    signal roomSelected(string roomID)
    signal leaveRoom(string roomID, string roomType)
    signal hideRoom(string roomID, string roomType)
    
    icon: d_icon
    topPadding: 0
    bottomPadding: 0
    implicitHeight: contentItem.implicitHeight
    separatorVisible: false
    
    visible: !d_name.empty && d_open

    textColor: ((d_unread > 0) || d_alert) ? Kirigami.Theme.textColor : Kirigami.Theme.disabledTextColor
    font.bold: (d_unread > 0) || d_alert
    label: d_name
    checked: d_selected
    onClicked: {
        root.roomSelected(d_roomID);
    }
    function generateText()
    {
        var str = "";
        if (d_userMentions > 0) {
            str = "@";
        }
        //Add space after (...)
        str += i18n("(%1)", d_unread) + ' ';
        return str;
    }

    RowLayout {
        id: editingActions
        QQC2.Label {
            id: unreadMessage

            visible: !d_editingMode && d_unread > 0
            height: parent.height
            width: height
            font.bold: true
            color: Kirigami.Theme.negativeTextColor
            text: generateText()
            Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
        }
        Kirigami.Icon {
            id: hideAction

            visible: d_editingMode
            source: "hide_table_row"
            height: Kirigami.Units.iconSizes.smallMedium
            width: height
            opacity: d_selected ? 1 : .5
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.active = true;
                onExited: parent.active = false;
                onClicked: {
                    root.hideRoom(d_roomID, d_type)
                }
            }
            Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
        }
        
        Kirigami.Icon {
            id: quitAction

            //We can leave only channel not private chat
            visible: d_editingMode && (d_type == "c" || d_type == "p")
            source: "dialog-close"
            height: Kirigami.Units.iconSizes.smallMedium
            width: height
            opacity: d_selected ? 1 : .5
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.active = true;
                onExited: parent.active = false;
                onClicked: {
                    root.leaveRoom(d_roomID, d_type)
                }
            }
            Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
        }
    }
}
