/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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
import KDE.Ruqola.FilesForRoomFilterProxyModel 1.0
import org.kde.kirigami 2.7 as Kirigami
import "common"

QQC2.Dialog {
    id: showFilesInRoomDialog

    title: i18n("Attachments")
    signal downloadFile(string file)
    signal deleteFile(string fileid)

    property string roomId
    property string channelType
    property QtObject filesModel
    width: parent.width * 9 / 10
    height: parent.height * 9 / 10
    anchors.centerIn: parent

    modal: true
    standardButtons: QQC2.Dialog.Close

    function initializeAndOpen()
    {
        searchField.text = "";
        searchField.forceActiveFocus();
        open();
    }


    contentItem: ColumnLayout {
        LineEditWithClearButton {
            id: searchField
            placeholderText: i18n("Search File...")
            Layout.fillWidth: true
            onTextChanged: {
                filesModel.setFilterString(text);
            }
        }

        SearchLabel {
            hasFullList: filesModel.hasFullList
            numberOfElements: listview.count
            labelText: listview.count === 0 ? i18n("No Attachment found") : i18np("%1 attachment in room (Total: %2)", "%1 attachments in room (Total: %2)", listview.count, filesModel.total())
            onLoadMoreElements: {
                appid.rocketChatAccount.loadMoreFileAttachments(roomId, channelType)
            }
        }

        //Add scrollview
        ListView {
            id: listview
            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: true
            // Scrollars
            QQC2.ScrollIndicator.vertical: QQC2.ScrollIndicator { }
            QQC2.ScrollIndicator.horizontal: QQC2.ScrollIndicator { }
            onDragEnded : {
                if (roomId !== "" && listview.atYBeginning) {
                    appid.rocketChatAccount.loadMoreFileAttachments(roomId, channelType)
                }
            }

            model: filesModel
            delegate: FilesInRoomDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: Kirigami.Units.largeSpacing
                anchors.leftMargin: Kirigami.Units.largeSpacing
                i_filename: filename
                i_description: description
                i_username: username
                i_timestamp: timestamp
            }
        }
    }
    DeleteFileAttachmentDialog {
        id: deleteFileAttachmentDialog
        onDeleteAttachment: {
            showFilesInRoomDialog.deleteFile(fileId)
        }
    }
}
