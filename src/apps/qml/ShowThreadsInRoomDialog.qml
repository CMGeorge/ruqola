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
import org.kde.kirigami 2.7 as Kirigami
import "common"

QQC2.Dialog {
    id: showThreadsInRoomDialog

    title: i18n("Threads")
    signal openThread(string threadMessageId, string threadPreviewText)

    property QtObject threadsModel
    property string roomId
    width: parent.width * 9 / 10
    height: parent.height * 9 / 10
    anchors.centerIn: parent

    modal: true
    focus: true
    standardButtons: QQC2.Dialog.Close

    function initializeAndOpen()
    {
        threadsModel.clearFilter();
        searchField.text = "";
        searchField.forceActiveFocus();
        open();
    }

    //Add menu here
    contentItem: ColumnLayout {
        LineEditWithClearButton {
            id: searchField
            placeholderText: i18n("Search Threads...")
            Layout.fillWidth: true
            onTextChanged: {
                threadsModel.setFilterString(text);
            }
        }

        SearchLabel {
            hasFullList: threadsModel.hasFullList
            numberOfElements: listview.count
            labelText: listview.count === 0 ? i18n("No Thread found") : i18np("%1 thread in room (Total: %2)", "%1 threads in room (Total: %2)", listview.count, threadsModel.total())
            onLoadMoreElements: {
                appid.rocketChatAccount.loadMoreThreads(roomId)
            }
        }

        ListView {
            id: listview
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Scrollars
            QQC2.ScrollIndicator.vertical: QQC2.ScrollIndicator { }
            QQC2.ScrollIndicator.horizontal: QQC2.ScrollIndicator { }
            onDragEnded : {
                if (roomId !== "" && listview.atYBeginning) {
                    appid.rocketChatAccount.loadMoreThreads(roomId)
                }
            }

            model: threadsModel
            delegate: Kirigami.BasicListItem {
                reserveSpaceForIcon: false
                reserveSpaceForLabel: false
                RowLayout {
                    width: parent.width
                    AvatarImage {
                        avatarurl: avatar == "" ? appid.rocketChatAccount.avatarUrl(userid) : avatar
                        aliasname: aliasname
                        username: username
                    }

                    ColumnLayout {
                        QQC2.Label {
                            text: description
                            elide: Text.ElideRight
                            Component.onCompleted: {
                                font.italic = true
                            }
                            wrapMode: QQC2.Label.Wrap
                            renderType: Text.NativeRendering
                            Layout.preferredWidth: parent.width
                        }

                        RowLayout {
                            QQC2.Label {
                                text: i18np("1 reply", "%1 replies", numberofmessages)
                                elide: Text.ElideRight
                                wrapMode: QQC2.Label.Wrap
                                Component.onCompleted: {
                                    font.bold = true
                                }
                                renderType: Text.NativeRendering
                            }
                            QQC2.Label {
                                id: timestampText
                                Layout.alignment: Qt.AlignTop | Qt.AlignRight
                                text: lastmessage
                                opacity: .5
                                renderType: Text.NativeRendering
                            }
                        }
                        QQC2.Label {
                            text: i18n("Open Thread")
                            elide: Text.ElideRight
                            wrapMode: QQC2.Label.Wrap
                            color: Kirigami.Theme.negativeTextColor
                            renderType: Text.NativeRendering
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    showThreadsInRoomDialog.openThread(threadmessageid, threadMessagePreview)
                                }
                            }
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                    }
                }
            }
        }
    }
}
