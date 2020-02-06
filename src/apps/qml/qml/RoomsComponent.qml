/*
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (C) 2017-2020 Laurent Montel <montel@kde.org>
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
import QtQml 2.1 as QtQml
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2
import org.kde.kirigami 2.8 as Kirigami

import Ruqola 1.0
import "common"
Component {
    id: roomsComponent
    Kirigami.ScrollablePage {
        id: roomsPage
        title: i18n("Rooms")

        actions {
            contextualActions: [
                Kirigami.Action {
                    id: editAction
                    iconName: "list-add"
                    text: i18n("Open Room");
                    onTriggered: {
                        searchChannelDialog.initializeAndOpen();
                    }
                },
                Kirigami.Action {
                    iconName: "edit-symbolic"
                    text: i18n("Unread on Top");
                    checkable: true
                    checked: appid.rocketChatAccount.sortUnreadOnTop;
                    onToggled: {
                        appid.rocketChatAccount.setSortUnreadOnTop(checked);
                    }
                },
                Kirigami.Action {
                    iconName: "edit-symbolic"
                    text: i18n("Show Close Icons");
                    checkable: true
                    onToggled: {
                        appid.rocketChatAccount.switchEditingMode(checked);
                    }
                },
                Kirigami.Action {
                    text: i18n("Create New Channel")
                    onTriggered: {
                        createNewChannelDialogLoader.active = true;
                    }
                },
                Kirigami.Action {
                    function mod(n, m) {
                        // see: https://stackoverflow.com/questions/4467539/javascript-modulo-gives-a-negative-result-for-negative-numbers
                        return ((n % m) + m) % m;
                    }

                    text: i18n("Previous Channel");
                    shortcut: "Alt+Up"
                    visible: false // no need to display it in right-click menu
                    onTriggered: {
                        roomsList.currentIndex = mod(roomsList.currentIndex - 1, roomsList.count);
                        roomsList.currentItem.selectRoom();
                    }
                },
                Kirigami.Action {
                    text: i18n("Next Channel");
                    shortcut: "Alt+Down"
                    visible: false // no need to display it in right-click menu
                    onTriggered: {
                        roomsList.currentIndex = (roomsList.currentIndex + 1) % roomsList.count;
                        roomsList.currentItem.selectRoom();
                    }
                },
                Kirigami.Action {
                   separator: true
                },
                Kirigami.Action {
                    text: i18n("Server Info")
                    onTriggered: {
                        serverinfodialogLoader.active = true;
                    }
                }

            ]
        }

        // Since we can't have actions at the bottom on mobile, force always toolbar mode
        globalToolBarStyle: Kirigami.ApplicationHeaderStyle.ToolBar
        titleDelegate: Kirigami.SearchField {
            id: searchField
            placeholderText: i18n("Search Room... (%1)", focusSequence)
            Layout.fillWidth: true
            onTextChanged: {
                appid.rocketChatAccount.roomFilterProxyModel.setFilterString(text);
            }
            onAccepted: {
                var roomID = roomsList.currentItem.d_roomID;
                appid.switchToRoom(roomID);
                searchField.text = "";
            }
            focusSequence: "Ctrl+K"
        }
        background: Rectangle {
            color: Kirigami.Theme.backgroundColor
        }
        footer: QQC2.ToolBar {
            Kirigami.Theme.inherit: false
            Kirigami.Theme.colorSet: Kirigami.Theme.Window
            position: QQC2.ToolBar.Footer
            RowLayout {
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    margins: Kirigami.Units.smallSpacing
                }

                QQC2.ToolButton {
                    id: accountButton
                    icon.name: "network-workgroup"
                    onClicked: accountMenu.open()
                    visible: accountMenuInstantiator.model.accountNumber > 1
                    QQC2.Menu {
                        id: accountMenu
                        QtQml.Instantiator {
                            id: accountMenuInstantiator
                            model: appid.accountManagerModel

                            delegate: QQC2.MenuItem {
                                text: name
                                checkable: true
                                autoExclusive: true
                                checked: appid.accountManager.currentAccount === name

                                onTriggered: {
                                    appid.accountManager.currentAccount = name;
                                }
                            }

                            onObjectAdded: {
                                accountMenu.insertItem(index, object)
                            }
                            onObjectRemoved: {
                                accountMenu.removeItem(object)
                            }
                        }
                    }
                }


                QQC2.Label {
                    id: comboboxLabel
                    textFormat: Text.PlainText
                    text: i18n("Status:")
                }

                StatusCombobox {
                    id: statusCombobox
                    model: appid.rocketChatAccount.statusModel
                }
            }
        }

        mainItem: RoomsView {
            id: roomsList
            editingMode: appid.rocketChatAccount.editingMode
            implicitWidth: Kirigami.Units.gridUnit * 10
            anchors.fill: parent
            model: appid.rocketChatAccount.roomFilterProxyModel
            selectedRoomID: appid.selectedRoomID;
            onHideRoom: {
                rocketChatAccount.hideRoom(roomID, roomType)
            }
            onLeaveRoom: {
                //TODO move to desktop.qml
                leaveChannelDialogLoader.rid = roomID
                leaveChannelDialogLoader.roomType = roomType
                leaveChannelDialogLoader.active = true
            }

            onRoomSelected: {
                appid.switchToRoom(roomID)
            }
        } //RoomsView
    }
}
