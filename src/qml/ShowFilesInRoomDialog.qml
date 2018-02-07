/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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


import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Window 2.0
import QtQuick 2.9
import KDE.Ruqola.FilesForRoomFilterProxyModel 1.0
import org.kde.kirigami 2.1 as Kirigami
//TODO move in common directory ?
import "messages"

Dialog {
    id: showFilesInRoomDialog

    title: i18n("Show Files In Room")
    signal downloadFile(string file)

    property QtObject filesModel

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    modal: true

    standardButtons: Dialog.Close

    function initializeAndOpen()
    {
        searchField.text = "";
        open();
    }


    ColumnLayout {
        TextField {
            id: searchField
            focus: true
            Layout.minimumHeight: Layout.maximumHeight
            Layout.maximumHeight: Kirigami.Units.iconSizes.smallMedium + Kirigami.Units.smallSpacing * 2
            Layout.fillWidth: true
            placeholderText: i18n("Search file...")
            onTextChanged: {
                filesModel.setFilterString(text);
            }
        }
        Label {
            text: i18np("%1 attachment in room", "%1 attachments in room", filesModel ? filesModel.rowCount() : 0)
        }

        //Add scrollview
        ListView {
            id: listview
            width: 300
            height: 200

            model: filesModel
            delegate:
                RowLayout {
                Label {
                    text: username + description
                    wrapMode: Label.Wrap
                }
                DownloadButton {
                    onDownloadButtonClicked: {
                        showFilesInRoomDialog.downloadFile(url)
                        console.log("Click on button" + url)
                        //TODO messageMain.downloadAttachment(model.modelData.link)
                    }
                }
            }
        }
    }
}
