/*
 * Copyright (C) 2019 Laurent Montel <montel@kde.org>
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
import org.kde.kirigami 2.7 as Kirigami
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Layouts 1.12
import KDE.Ruqola.ExtraColors 1.0
import KDE.Ruqola.DebugCategory 1.0
import "common"

Kirigami.BasicListItem {
    reserveSpaceForIcon: false
    reserveSpaceForLabel: false
    property string i_filename
    property string i_description
    property string i_username
    property string i_timestamp
    property string i_url

    RowLayout {
        width: filesInRoomDelegateItem.width
        ColumnLayout {
            QQC2.Label {
                text: i_filename
                textFormat: Text.PlainText
                elide: Text.ElideRight
                wrapMode: QQC2.Label.Wrap
            }
            QQC2.Label {
                text: i_description
                visible: i_description !== ""
                wrapMode: QQC2.Label.Wrap
                Component.onCompleted: {
                    font.italic = true
                }
            }
            QQC2.Label {
                text: i_username
                wrapMode: QQC2.Label.NoWrap
                elide: Text.ElideRight
                color: Kirigami.Theme.disabledTextColor
                Component.onCompleted: {
                    font.italic = true
                }
            }
            QQC2.Label {
                text: i_timestamp
                textFormat: Text.PlainText
                wrapMode: QQC2.Label.NoWrap
                color: Kirigami.Theme.disabledTextColor
                Component.onCompleted: {
                    font.italic = true
                }
            }
        }
        Item {
            Layout.fillWidth: true
        }
        DownloadButton {
            onDownloadButtonClicked: {
                showFilesInRoomDialog.downloadFile(i_url)
            }
        }
        DeleteButton {
            visible: canbedeleted
            onDeleteButtonClicked: {
                deleteFileAttachmentDialog.fileId = fileid;
                deleteFileAttachmentDialog.open();
            }
        }
    }
}
