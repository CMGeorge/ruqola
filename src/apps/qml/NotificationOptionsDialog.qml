/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

QQC2.Dialog {
    id: notificationOptionsDialog

    title: i18n("Notifications")
    standardButtons: QQC2.Dialog.Close

    property QtObject roomInfo

    signal modifyNotificationsSetting(string roomId, int type, var newVal)
    property string rid
    modal: true
    focus: true
    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    function updateStatus(check)
    {
        desktopNotificationGroupBox.enabled = !check;
        mobileNotificationGroupBox.enabled = !check;
        emailNotificationGroupBox.enabled = !check;
    }

    function initializeAndOpen()
    {
        //Enable groupbox
        updateStatus(disableNotification.checked);
        open();
    }

    ColumnLayout {
        anchors.fill: parent
        GridLayout {
            columns: 2
            QQC2.Label {
                text: i18n("Disable Notifications:");
            }

            QQC2.Switch {
                id: disableNotification
                checked: roomInfo === null ? false : roomInfo.notificationOptions.disableNotifications
                onClicked: {
                    notificationOptionsDialog.modifyNotificationsSetting(rid, RocketChatAccount.DisableNotifications, checked)
                    updateStatus(checked)
                }
            }

            QQC2.Label {
                text: i18n("Hide Unread Room Status:");
            }

            QQC2.Switch {
                id: hideUnreadRoomStatus
                checked: roomInfo === null ? false : roomInfo.notificationOptions.hideUnreadStatus
                onClicked: {
                    notificationOptionsDialog.modifyNotificationsSetting(rid, RocketChatAccount.HideUnreadStatus, checked)
                }
            }

            QQC2.Label {
                text: i18n("Mute Groups Mention:");
            }

            QQC2.Switch {
                id: muteGroupMention
                checked: roomInfo === null ? false : roomInfo.notificationOptions.muteGroupMentions
                onClicked: {
                    notificationOptionsDialog.modifyNotificationsSetting(rid, RocketChatAccount.MuteGroupMentions, checked)
                }
            }
        }

        QQC2.GroupBox {
            id: desktopNotificationGroupBox
            title: i18n("Desktop")
            GridLayout {
                columns: 2
                anchors.fill: parent
                QQC2.Label {
                    text: i18n("Alert:");
                }
                NotificationAlertCombobox {
                    id: desktopAlertCombobox
                    model: NotificationPreferences.desktopNotificationModel
                    currentIndex: roomInfo !== null ? model.setCurrentNotificationPreference(roomInfo.notificationOptions.desktopNotifications) : 0
                    onActivated: {
                        notificationOptionsDialog.modifyNotificationsSetting(rid, RocketChatAccount.DesktopNotifications, model.currentPreference(index))
                    }
                }
                QQC2.Label {
                    text: i18n("Audio:");
                }
                NotificationAlertCombobox {
                    id: desktopAudioCombobox
                    model: NotificationPreferences.desktopAudioNotificationModel
                    currentIndex: roomInfo !== null ? model.setCurrentNotificationPreference(roomInfo.notificationOptions.audioNotifications) : 0
                    onActivated: {
                        notificationOptionsDialog.modifyNotificationsSetting(rid, RocketChatAccount.AudioNotifications, model.currentPreference(index))
                    }
                }
                QQC2.Label {
                    text: i18n("Sound:");
                }
                NotificationAlertCombobox {
                    id: desktopSoundCombobox
                    model: NotificationPreferences.desktopSoundNotificationModel
                    currentIndex: roomInfo !== null ? model.setCurrentNotificationPreference(roomInfo.notificationOptions.audioNotificationValue) : 0
                    onActivated: {
                        notificationOptionsDialog.modifyNotificationsSetting(rid, RocketChatAccount.DesktopSoundNotifications, model.currentPreference(index))
                    }
                }
                QQC2.Label {
                    text: i18n("Duration:");
                }
                NotificationAlertCombobox {
                    id: desktopDurationCombobox
                    model: NotificationPreferences.desktopDurationNotificationModel
                    currentIndex: roomInfo !== null ? model.setCurrentNotificationPreference(roomInfo.notificationOptions.desktopNotificationDuration) : 0
                    onActivated: {
                        notificationOptionsDialog.modifyNotificationsSetting(rid, RocketChatAccount.DesktopDurationNotifications, model.currentPreference(index))
                    }
                }
            }
        }

        QQC2.GroupBox {
            id: mobileNotificationGroupBox
            title: i18n("Mobile")
            GridLayout {
                columns: 2
                anchors.fill: parent

                QQC2.Label {
                    text: i18n("Alert:");
                }
                NotificationAlertCombobox {
                    id: mobileAlertCombobox
                    model: NotificationPreferences.mobileNotificationModel
                    currentIndex: roomInfo !== null ? model.setCurrentNotificationPreference(roomInfo.notificationOptions.mobileNotifications) : 0
                    onActivated: {
                        notificationOptionsDialog.modifyNotificationsSetting(rid, RocketChatAccount.MobilePushNotifications, NotificationPreferences.mobileNotificationModel().currentPreference(index))
                    }
                }
            }
        }

        QQC2.GroupBox {
            id: emailNotificationGroupBox
            title: i18n("Email")
            GridLayout {
                anchors.fill: parent
                columns: 2
                QQC2.Label {
                    text: i18n("Alert:");
                }
                NotificationAlertCombobox {
                    id: emailAlertCombobox
                    model: NotificationPreferences.emailNotificationModel
                    currentIndex: roomInfo !== null ? model.setCurrentNotificationPreference(roomInfo.notificationOptions.emailNotifications) : 0
                    onActivated: {
                        notificationOptionsDialog.modifyNotificationsSetting(rid, RocketChatAccount.EmailNotifications, NotificationPreferences.emailNotificationModel().currentPreference(index))
                    }
                }
            }
        }
    }
}
