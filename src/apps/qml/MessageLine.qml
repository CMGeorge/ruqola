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
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Layouts 1.12

import KDE.Ruqola.RocketChatAccount 1.0
import KDE.Ruqola.InputCompleterModel 1.0
import KDE.Ruqola.DDPClient 1.0
import org.kde.kirigami 2.7 as Kirigami

ColumnLayout {
    property alias messageLineText: messageLine.text
    property string savePreviousMessage
    readonly property int popupheight: 100

    function sendMessage()
    {
        messageLine.accepted();
    }

    function setOriginalMessage(messageStr)
    {
        messageLine.text = messageStr
        savePreviousMessage = messageStr
        messageLine.selectAll()
    }

    function insertEmoticon(emotiStr)
    {
        messageLine.insert(messageLine.cursorPosition, emotiStr)
    }

    QQC2.TextField {
        id: messageLine
        //FIXME add multiline !!!
        inputMethodHints: Qt.ImhMultiLine

        Layout.fillWidth: true
        placeholderText: i18n("Enter message...")
        onTextChanged: {
            rcAccount.setInputTextChanged(text, cursorPosition);
            if (listView.count > 0) {
                showPopupCompleting()
            } else {
                popup.close()
            }
            footerItem.textEditing(text)
        }
        Keys.onDownPressed: {
            listView.incrementCurrentIndex()
        }
        Keys.onUpPressed: {
            listView.decrementCurrentIndex()
        }
        Keys.onTabPressed: {
            if (listView.currentItem) {
                textSelected(listView.currentItem.myData.completername)
            }
        }
        Keys.onReturnPressed: {
            if (listView.currentItem) {
                textSelected(listView.currentItem.myData.completername)
            } else {
                accepted()
            }
        }
        Keys.onEnterPressed: {
            if (listView.currentItem) {
                textSelected(listView.currentItem.myData.completername)
            } else {
                accepted()
            }
        }

        onAccepted: {
            if (text != "" && rcAccount.loginStatus === DDPClient.LoggedIn && (selectedRoomID !== "")) {
                //Modify text.
                if (messageId !== "") {
                    //Reply against message
                    if (savePreviousMessage == "") {
                        rcAccount.sendMessage(selectedRoomID, text, messageId);
                    } else if (text !== savePreviousMessage) {
                        rcAccount.updateMessage(selectedRoomID, messageId, text);
                    }
                    savePreviousMessage = "";
                    messageId = "";
                } else {
                    rcAccount.sendMessage(selectedRoomID, text);
                }
                text = "";
            }
        }
        QQC2.Popup {
            id: popup
            x: 0
            height: Kirigami.Units.gridUnit * listView.count + 10
            y: -height - 10
            padding: 0
            width: messageLine.width
            contentHeight: rect.height
            visible: listView.count > 0

            Rectangle {
                id: rect

                anchors.top: popup.top
                anchors.left: popup.left

                height: popup.height
                width: popup.width

                ListView {
                    id: listView
                    height: popup.height
                    width: parent.width
                    interactive: true
                    clip: true
                    model: rcAccount.inputCompleterModel()
                    delegate: Kirigami.BasicListItem {
                        readonly property variant myData: model

                        icon: model.iconname

                        label: model.displayname
                        onClicked: {
                            listView.currentIndex = model.index
                            textSelected(model.completername)
                        }
                        highlighted: focus && ListView.isCurrentItem
                    }
                    QQC2.ScrollIndicator.vertical: QQC2.ScrollIndicator { }
                }
            }
        }
        function showPopupCompleting() {
            if (!popup.visible) {
                popup.open()
                listView.currentIndex = 0
            }
        }
    }

    function textSelected(completerName) {
        if (listView.currentItem) {
            messageLine.text = rcAccount.replaceWord(completerName + " ", messageLine.text, messageLine.cursorPosition)
        }
        popup.close()
    }
    Keys.onEscapePressed: {
        if (popup.visible) {
            popup.close()
            //Clear modified message
        } else if (savePreviousMessage !== "") {
            savePreviousMessage = "";
            messageId = "";
            messageLine.text = "";
        } else {
            clearUnreadMessages();
        }
    }
}
