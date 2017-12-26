/*
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright 2017 Laurent Montel <montel@kde.org>
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
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2 as QQC2

import KDE.Ruqola.RuqolaUtils 1.0
import KDE.Ruqola.Ruqola 1.0
import KDE.Ruqola.DDPClient 1.0
import KDE.Ruqola.RoomFilterProxyModel 1.0
import org.kde.kirigami 2.1 as Kirigami
import KDE.Ruqola.DebugCategory 1.0

Component {
    id: mainComponent
    
    
    Kirigami.ScrollablePage {
        
        id: mainWidget
        leftPadding: Kirigami.Units.smallSpacing
        rightPadding: Kirigami.Units.smallSpacing
        topPadding: Kirigami.Units.smallSpacing
        bottomPadding: Kirigami.Units.smallSpacing
        
        header: Column {
            
            RowLayout {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 2*Kirigami.Units.smallSpacing
                visible: appid.selectedRoom
                
                ToolButton {
                    iconName: "favorite"
                    checkable: true
                    Binding on checked {
                        value: appid.selectedRoom && appid.selectedRoom.favorite
                    }
                    onCheckedChanged: {
                        appid.rocketChatAccount.changeFavorite(appid.selectedRoomID, checked)
                    }
                }
                Kirigami.Heading {
                    text: "#" + (appid.selectedRoom ? appid.selectedRoom.name : "")
                    level: 3
                    font.bold: true
                }
                Item {
                    Layout.fillWidth: true
                }
                ToolButton {
                    iconName: "settings-configure"
                    onClicked: menu.open();
                    QQC2.Menu {
                        id: menu
                        y: parent.height
                        
                        QQC2.MenuItem {
                            text: i18n("Create New Channel")
                            onTriggered: {
                                createNewChannelDialog.initializeAndOpen()
                            }
                        }
                        QQC2.MenuSeparator {
                            padding: 0
                            contentItem: Rectangle {
                                implicitWidth: 200
                                implicitHeight: 1
                                color: "#1E000000"
                            }
                        }
                        QQC2.MenuItem {
                            text: i18n("Channel Info")
                            onTriggered: {
                                channelInfoDialog.initializeAndOpen()
                            }
                        }
                        QQC2.MenuSeparator {
                            padding: 0
                            contentItem: Rectangle {
                                implicitWidth: 200
                                implicitHeight: 1
                                color: "#1E000000"
                            }
                        }
                        QQC2.MenuItem {
                            text: i18n("Video Chat")
                            onTriggered: {
                                if (appid.selectedRoom) {
                                    appid.rocketChatAccount.createJitsiConfCall(appid.selectedRoom);
                                }
                            }
                        }
                        QQC2.MenuSeparator {
                            padding: 0
                            contentItem: Rectangle {
                                implicitWidth: 200
                                implicitHeight: 1
                                color: "#1E000000"
                            }
                        }
                        QQC2.MenuItem {
                            text: i18n("Test add user !")
                            onTriggered: {
                                addUserDialog.initializeAndOpen()
                            }
                        }
                        QQC2.MenuSeparator {
                            padding: 0
                            contentItem: Rectangle {
                                implicitWidth: 200
                                implicitHeight: 1
                                color: "#1E000000"
                            }
                        }
                        QQC2.MenuItem {
                            text: i18n("Take a Video Message")
                            onTriggered: {
                                takeVideoMessage.open();
                            }
                        }
                    }
                }
            }
            
            QQC2.Label {
                visible: appid.selectedRoom && (appid.selectedRoom.topic !== "")
                text: appid.selectedRoom ? appid.selectedRoom.topic : ""
                font.italic: true
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.margins: 2*Kirigami.Units.smallSpacing
                wrapMode: Label.Wrap
            }
            QQC2.Label {
                visible: appid.selectedRoom && (appid.selectedRoom.announcement !== "")
                text: appid.selectedRoom ? appid.selectedRoom.announcement : ""
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.margins: 2*Kirigami.Units.smallSpacing
                wrapMode: Label.Wrap
            }
            
            Rectangle {
                color: Kirigami.Theme.textColor
                height:1
                anchors.right: parent.right
                anchors.left: parent.left
                opacity: .5
                visible: appid.selectedRoom
            }
        }
        ActiveChat {
            id: activeChat
            model: appid.model
            rocketChatAccount: appid.rocketChatAccount
            
            onOpenDirectChannel: {
                openDirectChannelDialog.username = userName;
                openDirectChannelDialog.open()
            }
            onJitsiCallConfActivated: {
                appid.rocketChatAccount.joinJitsiConfCall(appid.selectedRoomID)
            }
            onDeleteMessage: {
                deleteMessageDialog.msgId = messageId
                deleteMessageDialog.open()
            }
            onDownloadAttachment: {
                appid.rocketChatAccount.downloadFile(url)
            }
            
            onEditMessage: {
                console.log(RuqolaDebugCategorySingleton.category, "edit message : " + messageId)
            }
            onReplyMessage: {
                console.log(RuqolaDebugCategorySingleton.category, "reply message : " + messageId)
            }
            onSetFavoriteMessage: {
                appid.rocketChatAccount.starMessage(messageId, appid.selectedRoomID, true)
                console.log(RuqolaDebugCategorySingleton.category, "set Favorite message : " + messageId)
            }
            
            
        }
        footer: UserInput {
            id: userInputMessage
            visible: appid.selectedRoom && (appid.selectedRoom.readOnly === false)
            
            messageLineText: appid.rocketChatAccount.getUserCurrentMessage(appid.selectedRoomID)
            onTextEditing: {
                appid.rocketChatAccount.textEditing(appid.selectedRoomID, str)
                appid.userInputMessageText = str;
            }
            onClearUnreadMessages: {
                appid.rocketChatAccount.clearUnreadMessages(appid.selectedRoomID)
            }
        }
    }// mainWidget Item
}
