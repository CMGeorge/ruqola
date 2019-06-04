/*
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (C) 2017-2019 Laurent Montel <montel@kde.org>
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
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2

import KDE.Ruqola.RuqolaUtils 1.0
import KDE.Ruqola.Ruqola 1.0
import KDE.Ruqola.Clipboard 1.0
import KDE.Ruqola.RoomFilterProxyModel 1.0
import org.kde.kirigami 2.7 as Kirigami
import KDE.Ruqola.DebugCategory 1.0
import KDE.Ruqola.ReceiveTypingNotificationManager 1.0
import KDE.Ruqola.DebugCategory 1.0

Component {
    id: mainComponent


    Kirigami.Page {
        id: mainWidget

        title: appid.selectedRoom ? appid.selectedRoom.displayRoomName : ""
        leftPadding: 0
        rightPadding: 0
        topPadding: 0
        bottomPadding: 0

        actions {
            left: Kirigami.Action {
                icon.name: "preferences-desktop-notification"
                icon.color: "transparent"
                visible: appid.selectedRoom
                onTriggered: {
                    notificationsDialogLoader.active = true;
                }
            }
            main: Kirigami.Action {
                id: showUsersAction
                icon.name: "system-users"
                visible: appid.selectedRoom
                checkable: true
            }
            right: Kirigami.Action {
                icon.name: "edit-find"
                visible: appid.selectedRoom
                onTriggered: {
                    searchMessageDialogLoader.active = true;
                }
            }
            contextualActions: [
                Kirigami.Action {
                    visible: appid.selectedRoom
                    text: i18n("Channel Info")
                    onTriggered: {
                        var channelType = appid.selectedRoom.channelType;
                        if (channelType === "c" || channelType === "p") {
                            //Only for debug
                            //                                    if (channelType === "c") {
                            //                                        appid.rocketChatAccount.channelInfo(appid.selectedRoom.rid);
                            //                                    } else {
                            //                                        appid.rocketChatAccount.groupInfo(appid.selectedRoom.rid);
                            //                                    }
                            //For testing
                            channelInfoDialog.roomInfo = appid.selectedRoom
                            channelInfoDialog.initializeAndOpen()
                        } else if (channelType === "d") {
                            privateChannelInfoDialog.roomInfo = appid.selectedRoom
                            privateChannelInfoDialog.initializeAndOpen()
                        } else {
                            console.log(RuqolaDebugCategorySingleton.category,"channel type " + appid.selectedRoom.channelType)
                        }
                    }
                },
                Kirigami.Action {
                    visible: appid.selectedRoom
                    text: i18n("Mentions")
                    onTriggered: {
                        appid.rocketChatAccount.channelGetAllUserMentions(appid.selectedRoomID);
                        showMentionsInRoomDialogLoader.active = true;
                    }
                },
                Kirigami.Action {
                    visible: appid.selectedRoom && appid.rocketChatAccount.discussionEnabled()
                    text: i18n("Discussions")
                    onTriggered: {
                        appid.rocketChatAccount.discussionsInRoom(appid.selectedRoomID);
                        showDiscussionsInRoomDialogLoader.active = true;
                    }
                },
                Kirigami.Action {
                    visible: appid.selectedRoom && appid.rocketChatAccount.threadsEnabled()
                    text: i18n("Threads")
                    onTriggered: {
                        appid.rocketChatAccount.threadsInRoom(appid.selectedRoomID);
                        showThreadsInRoomDialogLoader.active = true
                    }
                },
                Kirigami.Action {
                    id: menuVideoChatAction
                    property bool shouldBeVisible: false
                    visible: shouldBeVisible && appid.selectedRoom
                    text: i18n("Video Chat")
                    onTriggered: {
                        appid.rocketChatAccount.createJitsiConfCall(appid.selectedRoomID);
                    }
                },
                Kirigami.Action {
                    text: i18n("Add User In Room")
                    visible: appid.selectedRoom ? appid.selectedRoom.canBeModify : false
                    onTriggered: {
                        var channelType = appid.selectedRoom.channelType;
                        if (channelType === "c" || channelType === "p") {
                            addUserDialog.roomInfo = appid.selectedRoom
                            addUserDialog.roomId = appid.selectedRoomID
                            addUserDialog.initializeAndOpen()
                        }
                    }
                },
                Kirigami.Action {
                    visible: appid.selectedRoom
                    text: i18n("Take a Video Message")
                    onTriggered: {
                        takeVideoMessageLoader.active = true;
                    }
                },
                Kirigami.Action {
                    visible: appid.selectedRoom
                    text: i18n("Load Recent History")
                    onTriggered: {
                        appid.rocketChatAccount.loadHistory(appid.selectedRoomID);
                    }
                },
                Kirigami.Action {
                    visible: appid.selectedRoom
                    text: i18n("Show Files Attachment In Room")
                    onTriggered: {
                        showFilesInRoomDialogLoader.active = true
                    }
                }
            ]
        }
        
        onContextualActionsAboutToShow: {
            menuVideoChatAction.shouldBeVisible = appid.rocketChatAccount.jitsiEnabled()
        }

        globalToolBarStyle: Kirigami.ApplicationHeaderStyle.ToolBar
        titleDelegate: RowLayout {
            QQC2.ToolButton {
                icon.name: "favorite"
                checkable: true
                visible: appid.selectedRoom && !appid.selectedRoom.isDiscussionRoom
                Binding on checked {
                    value: appid.selectedRoom && appid.selectedRoom.favorite
                }
                onToggled: {
                    appid.rocketChatAccount.changeFavorite(appid.selectedRoomID, checked)
                }
            }
            QQC2.ToolButton {
                icon.name: "draw-arrow-back"
                visible: appid.selectedRoom && appid.selectedRoom.isDiscussionRoom
                onClicked: {
                    appid.switchToRoom(appid.selectedRoom.parentRid)
                }
            }
            Kirigami.Icon {
                source: "encrypted"
                //FIXME
                height: Kirigami.Units.iconSizes.medium
                width: height
                visible: appid.selectedRoom && appid.selectedRoom.encrypted
            }
            Kirigami.Heading {
                text: appid.selectedRoom ? appid.selectedRoom.displayRoomName : ""
                level: 3
                font.bold: true
            }
            Item {
                Layout.fillWidth: true
            }
        }

        header: Column {
            spacing: Kirigami.Units.smallSpacing
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
                onLinkActivated: {
                    RuqolaUtils.openUrl(link);
                }
            }
            QQC2.Label {
                visible: appid.selectedRoom && (appid.selectedRoom.description !== "")
                text: appid.selectedRoom ? appid.selectedRoom.description : ""
                font.italic: true
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.margins: 2*Kirigami.Units.smallSpacing
                wrapMode: QQC2.Label.Wrap
            }

            Kirigami.Separator {
                anchors.right: parent.right
                anchors.left: parent.left
                visible: appid.selectedRoom
            }
            Flow {
                id: topBarUserList
                readonly property bool isActive: showUsersAction.checked
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Kirigami.Units.smallSpacing
                }
                opacity: topBarUserList.isActive ? 1 : 0
                Behavior on opacity { NumberAnimation { duration: 650; easing.type: Easing.InOutQuad } }

                Repeater {
                    model: parent.opacity > 0.5 ? appid.userModel : 0
                    RowLayout {
                        Kirigami.Icon {
                            source: model.iconstatus
                            //FIXME
                            height: Kirigami.Units.iconSizes.small
                            width: height
                        }
                        UserMenu {
                            id: userMenu
                            userId: model.userid
                            can_manage_users: appid.selectedRoom.canChangeRoles
                            ownUserId: appid.rocketChatAccount.userID
                            isAdirectChannel: appid.selectedRoom.channelType === "d"
                            onKickUser: {
                                appid.rocketChatAccount.kickUser(appid.selectedRoomID, userId, appid.selectedRoom.channelType)
                            }
                            onChangeRole: {
                                appid.rocketChatAccount.changeRoles(appid.selectedRoomID, userId, appid.selectedRoom.channelType, type)
                            }
                            onIgnoreUser: {
                                //TODO verify ignored
                                //appid.rocketChatAccount.ignoreUser(appid.selectedRoomID, userId, ignored)
                            }
                            onOpenConversation: {
                                //TODO
                            }
                            onAboutToShow: {
                                hasLeaderRole = appid.selectedRoom.userHasLeaderRole(model.userid)
                                hasModeratorRole = appid.selectedRoom.userHasModeratorRole(model.userid)
                                hasOwnerRole = appid.selectedRoom.userHasOwnerRole(model.userid)
                            }
                        }

                        QQC2.Label {
                            text: model.displayname
                            onLinkActivated: {
                                if (model.userid !== appid.rocketChatAccount.userID) {
                                    openDirectChannelDialog.username = link;
                                    openDirectChannelDialog.open()
                                }
                            }
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.RightButton

                                onClicked: {
                                    if (mouse.button === Qt.RightButton) {
                                        userMenu.x = mouse.x
                                        userMenu.y = mouse.y
                                        userMenu.open();
                                    }
                                }
                            }
                        }
                    }
                }
                Item {
                    width: parent.width
                    height: topBarUserList.isActive ? 1 : 0
                    Kirigami.Separator {
                        height: parent.height
                        width: height > 0 ? parent.width : 0
                        anchors.centerIn: parent
                        Behavior on width { NumberAnimation { duration: 650; easing.type: Easing.InOutQuad } }
                    }
                }
            }
        }

        Clipboard {
            id: clipboard
        }

        ActiveChat {
            id: activeChat
            model: appid.messageModel
            rcAccount: appid.rocketChatAccount
            roomId: appid.selectedRoomID
            anchors.fill: parent
            clip: true

            QQC2.ScrollBar.vertical: QQC2.ScrollBar { }

            onPinMessage: {
                appid.rocketChatAccount.pinMessage(messageId, pinned)
            }

            onEditMessage: {
                userInputMessage.messageId = messageId;
                userInputMessage.setOriginalMessage(messageStr)
                //console.log(RuqolaDebugCategorySingleton.category, "edit! messageId : " + messageId + " messageStr " + messageStr)
            }
            onCopyMessage: {
                clipboard.text = messageStr
                //console.log(RuqolaDebugCategorySingleton.category, "copy! messageId : " + messageId + " messageStr " + messageStr)
            }
            onReplyMessage: {
                console.log(RuqolaDebugCategorySingleton.category, "Not implemented reply message : " + messageId)
            }
            onSetFavoriteMessage: {
                appid.rocketChatAccount.starMessage(messageId, starred)
            }
            onIgnoreUser: {
                appid.rocketChatAccount.ignoreUser(roomId, userId, ignored)
            }
            onCreateDiscussion: {
                createDiscussionDialog.clearAndOpen()
                createDiscussionDialog.roomName = appid.selectedRoom.displayRoomName
                createDiscussionDialog.roomId = roomId
                createDiscussionDialog.messageId = messageId
                //TODO add message text too
            }

            onOpenChannel: {
                openChannelDialog.channelName = channel
                openChannelDialog.open()
            }

            onOpenDirectChannel: {
                openDirectChannelDialog.username = userName;
                openDirectChannelDialog.open()
            }
            onJitsiCallConfActivated: {
                appid.rocketChatAccount.joinJitsiConfCall(roomId)
            }
            onReportMessage: {
                reportMessageDialogLoader.messageId = messageId
                reportMessageDialogLoader.active = true
            }

            onDeleteMessage: {
                deleteMessageDialog.msgId = messageId
                deleteMessageDialog.open()
            }
            onDownloadAttachment: {
                downloadFileDialog.fileToSaveUrl = url
                downloadFileDialog.open()
            }

            onDisplayImage: {
                displayImageDialog.iUrl = imageUrl
                displayImageDialog.title = title
                displayImageDialog.isAnimatedImage = isAnimatedImage
                displayImageDialog.clearScaleAndOpen();
            }
            onDeleteReaction: {
                appid.rocketChatAccount.reactOnMessage(messageId, emoji, false)
            }
            onAddReaction: {
                appid.rocketChatAccount.reactOnMessage(messageId, emoji, true)
            }

            onOpenThread: {
                showThreadMessageDialogLoader.threadMessageId = threadMessageId;
                showThreadMessageDialogLoader.active = true
            }

            onOpenDiscussion: {
                //console.log(RuqolaDebugCategorySingleton.category, "Open discussion " + discussionRoomId)
                appid.switchToRoom(discussionRoomId)
            }
            onReplyInThread:  {
                console.log(RuqolaDebugCategorySingleton.category,"reply in thread " + messageId)
            }

            OpenChannelDialog {
                id: openChannelDialog
                onOpenChannel: {
                    appid.rocketChatAccount.openChannel(channelName);
                }
            }

            OpenDirectChannelDialog {
                id: openDirectChannelDialog
                onOpenDirectChannel: {
                    if (appid.rocketChatAccount.userName !== userName) {
                        appid.rocketChatAccount.openDirectChannel(userName);
                    }
                }
            }

            CreateDiscussionDialog {
                id: createDiscussionDialog
                onCreateNewDiscussion: {
                    appid.rocketChatAccount.createDiscussion(parentRoomName, discussionTitle, replyMessage, msgId);
                }
            }

            Loader {
                id: reportMessageDialogLoader
                active: false
                parent: mainWidget
                property string messageId
                sourceComponent: ReportMessageDialog {
                    id: reportMessageDialog
                    onReportMessage: {
                        appid.rocketChatAccount.deleteMessage(messageId, message)
                    }
                    onRejected: {
                        reportMessageDialogLoader.active = false
                    }
                    onAccepted: {
                        reportMessageDialogLoader.active = false
                    }
                    Component.onCompleted: {
                        msgId = reportMessageDialogLoader.messageId
                        initializeAndOpen()
                    }

                }
            }

            DeleteMessageDialog {
                id: deleteMessageDialog
                onDeleteMessage: {
                    appid.rocketChatAccount.deleteMessage(messageId, appid.selectedRoomID)
                }
            }

            DownloadFileDialog {
                id: downloadFileDialog
                onAccepted: {
                    if (fileUrl != "") {
                        console.log(RuqolaDebugCategorySingleton.category, "You chose: " + fileUrl)
                        appid.rocketChatAccount.downloadFile(fileToSaveUrl, fileUrl)
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
                    appid.rocketChatAccount.uploadFile(appid.selectedRoomID, description, messageText, filename)
                }
            }

            Loader {
                id: showMentionsInRoomDialogLoader
                active: false
                sourceComponent: ShowMentionsInRoomDialog {
                    id: showMentionsInRoomDialog
                    parent: appid.pageStack
                    mentionsModel: appid.mentionsModel
                    onGoToMessage: {
                        console.log(RuqolaDebugCategorySingleton.category, "Go to Message not implemented yet")
                    }
                    onRejected: {
                        showMentionsInRoomDialogLoader.active = false
                    }
                    onAccepted: {
                        showMentionsInRoomDialogLoader.active = false
                    }
                    Component.onCompleted: {
                        initializeAndOpen()
                    }
                }
            }
            Loader {
                id: showDiscussionsInRoomDialogLoader
                active: false
                sourceComponent: ShowDiscussionsInRoomDialog {
                    id: showDiscussionsInRoomDialog
                    parent: appid.pageStack

                    discussionsModel: appid.discussionsModel

                    Component.onCompleted: {
                        initializeAndOpen()
                    }
                    onOpenDiscussion: {
                        appid.switchToRoom(discussionId)
                        showDiscussionsInRoomDialogLoader.active = false
                    }
                    onRejected: {
                        showDiscussionsInRoomDialogLoader.active = false
                    }
                    onAccepted: {
                        showDiscussionsInRoomDialogLoader.active = false
                    }
                }
            }

            Loader {
                id: showThreadsInRoomDialogLoader
                active: false
                sourceComponent: ShowThreadsInRoomDialog {
                    id: showThreadsInRoomDialog
                    parent: appid.pageStack
                    threadsModel: appid.threadsModel
                    onOpenThread: {
                        appid.rocketChatAccount.getThreadMessages(threadMessageId)
                    }
                    onRejected: {
                        showThreadsInRoomDialogLoader.active = false
                    }
                    onAccepted: {
                        showThreadsInRoomDialogLoader.active = false
                    }
                    Component.onCompleted: {
                        initializeAndOpen()
                    }
                }
            }

            Loader {
                id: showFilesInRoomDialogLoader
                active: false
                sourceComponent: ShowFilesInRoomDialog {
                    id: showFilesInRoomDialog
                    parent: appid.pageStack
                    filesModel: appid.filesModel
                    onDownloadFile: {
                        downloadFileDialog.fileToSaveUrl = file
                        downloadFileDialog.open()
                    }
                    Component.onCompleted: {
                        appid.rocketChatAccount.roomFiles(appid.selectedRoomID, appid.selectedRoom.channelType);
                        initializeAndOpen()
                    }

                    onDeleteFile: {
                        appid.rocketChatAccount.deleteFileMessage(appid.selectedRoomID, fileid, appid.selectedRoom.channelType)
                    }
                    onRejected: {
                        showFilesInRoomDialogLoader.active = false
                    }
                    onAccepted: {
                        showFilesInRoomDialogLoader.active = false
                    }
                }
            }
            Loader {
                id: showThreadMessageDialogLoader
                active: false
                parent: appid.pageStack
                property string threadMessageId
                sourceComponent: ShowThreadMessagesDialog {
                    id: showThreadMessageDialog
                    onAccepted: {
                        showThreadMessageDialogLoader.active = false
                    }
                    onRejected: {
                        showThreadMessageDialogLoader.active = false
                    }
                    Component.onCompleted: {
                        showThreadMessageDialog.threadMessageId = showThreadMessageDialogLoader.threadMessageId;
                        showThreadMessageDialog.open()
                    }
                }
            }
        }

        Keys.onEscapePressed: {
            appid.rocketChatAccount.clearUnreadMessages(appid.selectedRoomID);
        }

        footer: QQC2.ToolBar {
            position: QQC2.ToolBar.Footer
            visible: appid.selectedRoom
            ColumnLayout {
                anchors.fill: parent
                QQC2.Label {
                    id: channelInfo
                    font.bold: true
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                    //FIXME
                    visible: appid.selectedRoom && ((appid.selectedRoom.readOnly === true && !appid.selectedRoom.canChangeRoles)
                                                    || (appid.selectedRoom.blocker === true)
                                                    || (appid.selectedRoom.blocked === true))
                    text: appid.selectedRoom ? appid.selectedRoom.roomMessageInfo : ""
                }

                QQC2.Label {
                    id: typingInfo
                    visible: text.length > 0
                }

                UserInput {
                    id: userInputMessage
                    rcAccount: appid.rocketChatAccount
                    visible: appid.selectedRoom && (appid.selectedRoom.readOnly === false) && (appid.selectedRoom.blocker === false) && (appid.selectedRoom.blocked === false)
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

                Connections {
                    target: appid.rocketChatAccount.receiveTypingNotificationManager()
                    onNotificationChanged: {
                        //console.log(RuqolaDebugCategorySingleton.category, "Typing in roomId: " + roomId + " str " + notificationStr);
                        if (appid.selectedRoomID === roomId) {
                            typingInfo.text = notificationStr;
                        }
                    }
                    onClearNotification: {
                        typingInfo.text = "";
                    }
                }
            }
        }
    }// mainWidget Item
}
