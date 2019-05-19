/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Window 2.2
import QtQuick 2.9
import KDE.Ruqola.UsersForRoomFilterProxyModel 1.0
import org.kde.kirigami 2.7 as Kirigami
//If we use it move TimestampText in common
import "messages"
QQC2.Dialog {
    id: showDiscussionsInRoomDialog

    title: i18n("Discussions")

    signal openDiscussion(string discussionId)

    property QtObject discussionsModel
    property string roomId
    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    modal: true
    focus: true
    standardButtons: QQC2.Dialog.Close

    function initializeAndOpen()
    {
        searchField.text = "";
        open();
    }

    ColumnLayout {
        LineEditWithClearButton {
            id: searchField
            placeholderText: i18n("Search Discussions...")
            Layout.fillWidth: true
            onTextChanged: {
                discussionsModel.setFilterString(text);
            }
        }
        QQC2.Label {
            text: listview.count === 0 ? i18n("No Discussion found") : ""
            Component.onCompleted: {
                font.italic = true
                font.bold = true
            }
        }

        ListView {
            id: listview
            width: 400;
            height: 200
            clip: true
            // Scrollars
            QQC2.ScrollIndicator.vertical: QQC2.ScrollIndicator { }
            QQC2.ScrollIndicator.horizontal: QQC2.ScrollIndicator { }

            model: discussionsModel
            delegate:
                RowLayout {
                width: ListView.view.width
                ColumnLayout {
                    QQC2.Label {
                        text: description
                        elide: Text.ElideRight
                        wrapMode: QQC2.Label.Wrap
                    }
                    RowLayout {
                        QQC2.Label {
                            text: i18n("Open Discussion")
                            elide: Text.ElideRight
                            wrapMode: QQC2.Label.Wrap
                            color: Kirigami.Theme.negativeTextColor
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    showDiscussionsInRoomDialog.openDiscussion(discussionid)
                                    //Allow to close it.
                                }
                            }
                        }
                        QQC2.Label {
                            text: i18np("1 message", "%1 messages", numberofmessages)
                            elide: Text.ElideRight
                            wrapMode: QQC2.Label.Wrap
                        }
                        TimestampText {
                            timestamp: lastmessage
                        }
                    }
                }
            }
        }
    }
}
