/*
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (C) 2017-2018 Laurent Montel <montel@kde.org>
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
import KDE.Ruqola.Clipboard 1.0
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
                        RuqolaMenuSeparator {
                        }
                        QQC2.MenuItem {
                            text: i18n("Channel Info")
                            onTriggered: {
                                channelInfoDialog.initializeAndOpen()
                            }
                        }
                        RuqolaMenuSeparator {
                        }
                        QQC2.MenuItem {
                            text: i18n("Video Chat")
                            onTriggered: {
                                appid.rocketChatAccount.createJitsiConfCall(appid.selectedRoomID);
                            }
                        }
                        RuqolaMenuSeparator {
                        }
                        QQC2.MenuItem {
                            text: i18n("Add User In Room")
                            onTriggered: {
                                addUserDialog.roomId = appid.selectedRoomID
                                addUserDialog.initializeAndOpen()
                            }
                        }
                        RuqolaMenuSeparator {
                        }
                        QQC2.MenuItem {
                            text: i18n("Take a Video Message")
                            onTriggered: {
                                takeVideoMessage.open();
                            }
                        }
                        RuqolaMenuSeparator {}
                        QQC2.MenuItem {
                            text: i18n("Load Recent History")
                            onTriggered: {
                                appid.rocketChatAccount.loadHistory(appid.selectedRoomID);
                            }
                        }
                        RuqolaMenuSeparator {}
                        QQC2.MenuItem {
                            text: i18n("Show Users In Room")
                            onTriggered: {
                                showUsersDialog.userModel = appid.userModel
                                showUsersDialog.initializeAndOpen()
                            }
                        }
                        RuqolaMenuSeparator {
                        }
                        QQC2.MenuItem {
                            text: i18n("Show Files Attachment In Room")
                            onTriggered: {
                                appid.rocketChatAccount.roomFiles(appid.selectedRoomID);
                                showFilesInRoomDialog.initializeAndOpen()
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
                wrapMode: QQC2.Label.Wrap
            }
            QQC2.Label {
                visible: appid.selectedRoom && (appid.selectedRoom.announcement !== "")
                text: appid.selectedRoom ? appid.selectedRoom.announcement : ""
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.margins: 2*Kirigami.Units.smallSpacing
                wrapMode: QQC2.Label.Wrap
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
        Clipboard {
            id: clipboard
        }

        ActiveChat {
            id: activeChat
            model: appid.model
            rcAccount: appid.rocketChatAccount
            roomId: appid.selectedRoomID
            
            onOpenDirectChannel: {
                openDirectChannelDialog.username = userName;
                openDirectChannelDialog.open()
            }
            onJitsiCallConfActivated: {
                appid.rocketChatAccount.joinJitsiConfCall(roomId)
            }
            onDeleteMessage: {
                deleteMessageDialog.msgId = messageId
                deleteMessageDialog.open()
            }
            onDownloadAttachment: {
                downloadFileDialog.fileToSaveUrl = url
                downloadFileDialog.open()
            }
            
            onEditMessage: {
                userInputMessage.messageId = messageId;
                userInputMessage.setOriginalMessage(messageStr)
                console.log(RuqolaDebugCategorySingleton.category, "edit! messageId : " + messageId + " messageStr " + messageStr)
            }
            onCopyMessage: {
                clipboard.text = messageStr
                console.log(RuqolaDebugCategorySingleton.category, "copy! messageId : " + messageId + " messageStr " + messageStr)
            }
            onReplyMessage: {
                console.log(RuqolaDebugCategorySingleton.category, "reply message : " + messageId)
            }
            onSetFavoriteMessage: {
                appid.rocketChatAccount.starMessage(messageId, roomId, starred)
            }

            onDisplayImage: {
                displayImageDialog.iUrl = imageUrl
                displayImageDialog.title = title
                displayImageDialog.clearScaleAndOpen();
            }
            
            OpenDirectChannelDialog {
                id: openDirectChannelDialog
                onOpenDirectChannel: {
                    appid.rocketChatAccount.openDirectChannel(userName);
                }
            }

            DeleteMessageDialog {
                id: deleteMessageDialog
                onDeleteMessage: {
                    appid.rocketChatAccount.deleteMessage(messageId)
                }
            }

            DownloadFileDialog {
                id: downloadFileDialog
                onAccepted: {
                    if (fileUrl != "") {
                        console.log(RuqolaDebugCategorySingleton.category, "You chose: " + fileUrl + " use cache :" + useCache)
                        appid.rocketChatAccount.downloadFile(fileToSaveUrl, fileUrl, useCache)
                    } else {
                        console.log(RuqolaDebugCategorySingleton.category, "No file selected");
                    }
                }
            }
            DisplayImageDialog {
                id: displayImageDialog
            }
            UploadFileDialog {
                id: uploadFileDialog
                onUploadFile: {
                    appid.rocketChatAccount.uploadFile(description, filename)
                }
            }

            ShowUsersInRoomDialog {
                id: showUsersDialog                
            }

            ShowFilesInRoomDialog {
                id: showFilesInRoomDialog
                filesModel: appid.filesModel
                onDownloadFile: {
                    console.log("Download file" + file)
                    //Verify it
                    downloadFileDialog.fileToSaveUrl = file
                    downloadFileDialog.useCache = false
                    downloadFileDialog.open()

                }
            }

        }
        footer: UserInput {
            id: userInputMessage
            rcAccount: appid.rocketChatAccount
            visible: appid.selectedRoom && (appid.selectedRoom.readOnly === false)
            anchors.bottom: mainWidget.bottom
            messageLineText: rcAccount.getUserCurrentMessage(appid.selectedRoomID)
            onTextEditing: {
                rcAccount.textEditing(appid.selectedRoomID, str)
                appid.userInputMessageText = str;
            }
            onClearUnreadMessages: {
                rcAccount.clearUnreadMessages(appid.selectedRoomID)
            }
            onUploadFile: {
                uploadFileDialog.initializeAndOpen()
            }
        }
    }// mainWidget Item
}
