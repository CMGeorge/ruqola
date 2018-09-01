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

import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2 as QQC2

import org.kde.kirigami 2.4 as Kirigami

QQC2.ScrollView {
    property QtObject personsModel : undefined

    contentHeight: authorsColumn.height
    contentWidth: 300

    ColumnLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.topMargin: Kirigami.Units.smallSpacing
        spacing: Kirigami.Units.largeSpacing

        Column {
            id: authorsColumn
            spacing: Kirigami.Units.largeSpacing
            height: childrenRect.height

            Repeater {
                id: authorList
                model: personsModel

                Column {
                    id: authorItem
                    spacing: Kirigami.Units.smallSpacing
                    QQC2.Label {
                        text: "<b>" + model.username + "</b>"
                        wrapMode: QQC2.Label.Wrap
                        horizontalAlignment: Text.AlignHCenter
                        renderType: Text.NativeRendering
                        textFormat: Text.RichText
                        font.bold: true
                    }
                    QQC2.Label {
                        text: "<i>" + model.task + "<i>"
                        wrapMode: QQC2.Label.Wrap
                        renderType: Text.NativeRendering
                        textFormat: Text.RichText
                    }
                    QQC2.Label {
                        text: model.email
                        wrapMode: QQC2.Label.Wrap
                        horizontalAlignment: Text.AlignHCenter
                        renderType: Text.NativeRendering
                        textFormat: Text.RichText
                        onLinkActivated: {
                            aboutDialog.openurl(link);
                        }
                    }
                }
            }
        }
    }
}
