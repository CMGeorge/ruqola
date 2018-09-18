/*
 * Copyright (C) 2018 Laurent Montel <montel@kde.org>
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

import org.kde.kirigami 2.4 as Kirigami
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Layouts 1.1
Repeater {
    id: repearterReactions
    signal deleteReaction(string emoji)
    Row {
        QQC2.Label {
            id: numberOfReact
            renderType: Text.NativeRendering
            textFormat: Text.RichText
            text: model.modelData.convertedReactionName
            wrapMode: QQC2.Label.Wrap
            anchors.leftMargin: Kirigami.Units.smallSpacing
            anchors.rightMargin: Kirigami.Units.smallSpacing
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton | Qt.LeftButton

                onClicked: {
                    repearterReactions.deleteReaction(model.modelData.reactionName);
                }
            }
        }
    }
}