/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>
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

import Ruqola 1.0

import org.kde.kirigami 2.7 as Kirigami
Kirigami.ApplicationWindow {
    id: appid
    readonly property int margin: 11

    property string selectedRoomID: "";
    property QtObject selectedRoom

    property QtObject messageModel
    property QtObject userModel
    property QtObject rocketChatAccount: accountManager.account

    property QtObject filesModel: appid.rocketChatAccount.filesForRoomFilterProxyModel
    property QtObject discussionsModel: appid.rocketChatAccount.discussionsFilterProxyModel
    property QtObject mentionsModel: appid.rocketChatAccount.mentionsFilterProxyModel
    property QtObject accountManager: Ruqola.accountManager()
    property QtObject accountManagerModel: accountManager.rocketChatAccountModel
    property QtObject searchMessageModel: rocketChatAccount.searchMessageFilterProxyModel
    property QtObject emojiModel: rocketChatAccount.emoticonModel
    property QtObject threadsModel: rocketChatAccount.threadsFilterProxyModel
    property QtObject threadMessagesModel: rocketChatAccount.threadMessageModel
    property QtObject listMessagesModel: rocketChatAccount.listMessagesFilterProxyModel
    property QtObject autotranslateLanguagesModel: rocketChatAccount.autoTranslateLanguagesModel
    property string userInputMessageText: "";

    width: Kirigami.Units.gridUnit * 55
    height: Kirigami.Units.gridUnit * 40

    title: i18n("Ruqola")


    function switchToRoom(roomID)
    {
        if (roomID === selectedRoomID) {
            return;
        }
        appid.rocketChatAccount.switchingToRoom(roomID)
        appid.rocketChatAccount.setUserCurrentMessage(appid.userInputMessageText, selectedRoomID)
        appid.selectedRoomID = roomID;
        appid.messageModel = appid.rocketChatAccount.messageModelForRoom(roomID)
        appid.messageModel.enableQmlHacks(true)
        appid.selectedRoom = appid.rocketChatAccount.roomWrapper(roomID)
        appid.userModel = appid.rocketChatAccount.usersForRoomFilterProxyModel(roomID)
    }

    pageStack.defaultColumnWidth: Kirigami.Units.gridUnit * 15
    pageStack.initialPage: [roomsComponent, mainComponent]
    pageStack.visible: rocketChatAccount.loginStatus === DDPClient.LoggedIn

    globalDrawer: Kirigami.GlobalDrawer {
        drawerOpen: false
        handleVisible: true
        resetMenuOnTriggered: true
        isMenu: true
        
        topContent: [
            QQC2.Label {
                text: rocketChatAccount.userName === "" ? "" : i18n("Hello, %1", rocketChatAccount.userName)
                textFormat: Text.PlainText
            }
        ]
        
        actions: [
            Kirigami.Action {
                text: i18n("About")
                iconName: "ruqola"
                onTriggered: {
                    aboutDataDialogLoader.active = true
                }
            },
            Kirigami.Action {
                text: i18n("Report a Bug")
                iconName: "tools-report-bug"
                onTriggered: {
                    Qt.openUrlExternally("https://bugs.kde.org/report.cgi");
                }
            },
            Kirigami.Action {
                text: i18n("Configure Account")
                iconName: "settings-configure"
                onTriggered: pageStack.push(Qt.resolvedUrl("ConfigureServerList.qml"), {accountModel: accountManagerModel})
            },
            Kirigami.Action {
                text: i18n("Handbook")
                iconName: "system-help"
                onTriggered: {
                    rocketChatAccount.openDocumentation();
                }
            },
            Kirigami.Action {
                separator: true
            },
            Kirigami.Action {
                text: i18n("Log out")
                iconName: "system-log-out"
                onTriggered: {
                    rocketChatAccount.logOut();
                    appid.globalDrawer.drawerOpen = false;
                }
            },
//            Kirigami.Action {
//                text: i18n("autotranslate")
//                onTriggered: {
//                    rocketChatAccount.getSupportedLanguages();
//                }
//            },
            Kirigami.Action {
                separator: true
            },
            Kirigami.Action {
                shortcut: StandardKey.Quit
                text: i18n("Quit")
                iconName: "application-exit"
                onTriggered: {
                    Qt.quit();
                }
            }
        ]
    }

    LoginPage {
        id: loginTab
        rcAccount: rocketChatAccount
    }

    Loader {
        id: aboutDataDialogLoader
        active: false
        sourceComponent: AboutDialog {
            id: aboutDataDialog
            parent: appid.pageStack
            applicationData: Ruqola.applicationData()
            Component.onCompleted: {
                open()
            }

            onOpenurl: {
                RuqolaUtils.openUrl(link);
            }
            onRejected: {
                aboutDataDialogLoader.active = false
            }
        }
    }

    Loader {
        id: customUserStatusDialogLoader
        active: false
        sourceComponent: CustomUserStatusDialog {
            id: customUserStatusDialog
            parent: appid.pageStack
            onRejected: {
                customUserStatusDialogLoader.active = false
            }
            onAccepted: {
                customUserStatusDialogLoader.active = false
            }
            Component.onCompleted: {
                open()
            }
        }
    }

    Loader {
        id: privateChannelInfoDialogLoader
        active: false
        sourceComponent: PrivateChannelInfoDialog {
            id: privateChannelInfoDialog
            roomInfo: appid.selectedRoom
            parent: appid.pageStack
            onBlockUser: {
                rocketChatAccount.blockUser(rid, block)
            }
            onRejected: {
                privateChannelInfoDialogLoader.active = false
            }
            onAccepted: {
                privateChannelInfoDialogLoader.active = false
            }
            Component.onCompleted: {
                initializeAndOpen()
            }
        }
    }

    Loader {
        id: notificationsDialogLoader
        active: false
        sourceComponent: NotificationOptionsDialog {
            id: notificationsDialog
            parent: appid.pageStack
            onModifyNotificationsSetting: {
                rocketChatAccount.changeNotificationsSettings(roomId, type, newVal)
            }

            Component.onCompleted: {
                rid = (appid && appid.selectedRoomID) ? appid.selectedRoomID : ""
                roomInfo = appid ? appid.selectedRoom : ""
                initializeAndOpen()
            }
            onRejected: {
                notificationsDialogLoader.active = false
            }
            onAccepted: {
                notificationsDialogLoader.active = false
            }

        }
    }

    Loader {
        id: channelInfoDialogLoader
        active: false
        sourceComponent: ChannelInfoDialog {
            id: channelInfoDialog
            parent: appid.pageStack
            roomInfo: appid.selectedRoom
            channelName: (appid && appid.selectedRoomID) ? appid.selectedRoomID : ""
            onDeleteRoom: {
                rocketChatAccount.eraseRoom(roomId, appid.selectedRoom.channelType)
            }
            onModifyChannelSetting: {
                rocketChatAccount.changeChannelSettings(roomId, type, newVal, channelType)
            }
            onRejected: {
                channelInfoDialogLoader.active = false
            }
            onAccepted: {
                channelInfoDialogLoader.active = false
            }
            Component.onCompleted: {
                initializeAndOpen()
            }

        }
    }
    Loader {
        id: leaveChannelDialogLoader
        active: false
        property string rid
        property string roomType

        sourceComponent: LeaveChannelDialog {
            id: leaveChannelDialog
            parent: appid.pageStack
            onLeaveChannel:  {
                rocketChatAccount.leaveRoom(roomId, channelType)
            }
            onRejected: {
                leaveChannelDialogLoader.active = false
            }
            onAccepted: {
                leaveChannelDialogLoader.active = false
            }
            Component.onCompleted: {
                leaveChannelDialog.rId = leaveChannelDialogLoader.rid
                leaveChannelDialog.channelType = leaveChannelDialogLoader.roomType
                open()
            }
        }
    }
    Loader {
        id: addUserDialogLoader
        active: false
        sourceComponent: AddUserDialog {
            id: addUserDialog
            parent: appid.pageStack
            completerModel: rocketChatAccount.userCompleterFilterModelProxy
            roomInfo: appid.selectedRoom
            roomId: appid.selectedRoomID

            onSearchUserName: {
                rocketChatAccount.userAutocomplete(pattern, "");
            }
            onAddUser: {
                rocketChatAccount.addUserToRoom(userId, rid, channelType)
            }
            onRejected: {
                addUserDialogLoader.active = false
            }
            onAccepted: {
                addUserDialogLoader.active = false
            }
            Component.onCompleted: {
                initializeAndOpen()
            }

        }
    }
    Loader {
        id: searchMessageDialogLoader
        active: false
        sourceComponent: ShowSearchMessageDialog {
            id: searchMessageDialog
            roomId: appid.selectedRoomID
            parent: appid.pageStack
            searchMessageModel: appid.searchMessageModel
            onSearchMessage: {
                rocketChatAccount.messageSearch(pattern, rid)
            }
            onGoToMessage: {
                console.log(RuqolaDebugCategorySingleton.category, "Show history to message: " + messageId)
            }
            onRejected: {
                rocketChatAccount.clearSearchModel()
                searchMessageDialogLoader.active = false
            }
            onAccepted: {
                rocketChatAccount.clearSearchModel()
                searchMessageDialogLoader.active = false
            }
            Component.onCompleted: {
                roomId = appid.selectedRoomID
                initializeAndOpen()
            }
        }
    }

    Loader {
        id: jobErrorMessageDialogLoader
        property string jobMessageError
        active: false
        sourceComponent: JobErrorMessageDialog {
            id: jobErrorMessageDialog
            parent: appid.pageStack
            Component.onCompleted: {
                jobErrorMessageDialog.jobMessageError = jobErrorMessageDialogLoader.jobMessageError
                open()
            }
            onRejected: {
                jobErrorMessageDialogLoader.active = false
            }
            onAccepted: {
                jobErrorMessageDialogLoader.active = false
            }
        }
    }
    Loader {
        id: createNewChannelDialogLoader
        active: false
        sourceComponent: CreateNewChannelDialog {
            parent: appid.pageStack
            Component.onCompleted: {
                encryptedRoomEnabled = appid.rocketChatAccount.encryptedEnabled
                initializeAndOpen()
            }
            onRejected: {
                createNewChannelDialogLoader.active = false
            }
            onAccepted: {
                createNewChannelDialogLoader.active = false
            }
            onCreateNewChannel: {
                rocketChatAccount.createNewChannel(name, readOnly, privateRoom, usernames, encryptedRoom, password, broadcast);
            }
        }
    }

    Loader {
        id: serverinfodialogLoader
        active: false
        sourceComponent: ServerInfoDialog {
            rcAccount: appid.rocketChatAccount
            parent: appid.pageStack
            Component.onCompleted: {
                open()
            }
            onRejected: {
                serverinfodialogLoader.active = false
            }
        }
    }

    SearchChannelDialog {
        id: searchChannelDialog
        searchChannelModel: rocketChatAccount.searchChannelFilterProxyModel
        onSearchChannel: {
            rocketChatAccount.channelAndPrivateAutocomplete(pattern);
        }
        onOpenChannel: {
            if (channeltype === Channel.Room) {
                rocketChatAccount.openChannel(channelid)
            } else if (channeltype === Channel.PrivateChannel) {
                if (rocketChatAccount.userName !== channelid) {
                    rocketChatAccount.openDirectChannel(channelid)
                }
            } else {
                console.log(RuqolaDebugCategorySingleton.category, "Unknown open channel type : "  + channeltype + " channelid : "  + channelid + " channelname : "  + channelname)
            }
        }
    }

    Loader {
        id: takeVideoMessageLoader
        active: false
        sourceComponent: TakeVideoMessageDialog {
            id: takeVideoMessage
            parent: appid.pageStack
            rcAccount: rocketChatAccount
            Component.onCompleted: {
                open()
            }
            onRejected: {
                takeVideoMessageLoader.active = false
            }
            onAccepted: {
                takeVideoMessageLoader.active = false
            }
        }
    }

    QQC2.BusyIndicator {
        id: busy
        anchors.centerIn: parent
        visible: rocketChatAccount.loginStatus === DDPClient.LoggingIn
    }

    RoomsComponent {
        id: roomsComponent
    }

    MainComponent {
        id: mainComponent
    }

    Loader {
        id: channelPasswordDialogLoader
        active: false
        property string roomId
        sourceComponent: ChannelPasswordDialog {
            id: channelPasswordDialog
            parent: appid.pageStack
            onJoinRoom: {
                rocketChatAccount.joinRoom(roomId, password)
            }
            Component.onCompleted: {
                roomId = channelPasswordDialogLoader.roomId
                initializeAndOpen()
            }
            onRejected: {
                channelPasswordDialogLoader.active = false
            }
            onAccepted: {
                channelPasswordDialogLoader.active = false
            }

        }
    }

    Connections {
        target: rocketChatAccount
        onMissingChannelPassword: {
            channelPasswordDialogLoader.roomId = roomId
            channelPasswordDialogLoader.active = true
        }
        onJobFailed: {
            jobErrorMessageDialogLoader.jobMessageError = message
            jobErrorMessageDialogLoader.active = true
        }
    }


    onClosing: {
        Qt.quit();
    }

    function toggleShow() {
        if (visible) {
            hide();
        } else {
            show();
            raise();
            requestActivate();
        }
    }

    Component.onCompleted: {

        if (Qt.platform.os == "android") {
            return;
        }

        systrayIcon.activateRequested.connect(toggleShow);
    }
}
