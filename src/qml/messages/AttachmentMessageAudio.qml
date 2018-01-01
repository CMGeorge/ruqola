/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

import QtQuick.Controls 2.2 as QQC2
import org.kde.kirigami 2.1 as Kirigami
//We need QtMultimedia module for playing audio file
import QtMultimedia 5.8
import QtQuick.Layouts 1.1

import KDE.Ruqola.DebugCategory 1.0

import "../js/message.js" as MessageScript;
import "../js/convert.js" as ConvertScript;

MessageBase {
    id: attachmentAudio

    MediaPlayer {
        id: audioPlayer
        autoPlay: false
        onPaused: {
            playerButton.source = "qrc:/icons/media-playback-start.svg"
        }
        onPlaying: {
            playerButton.source = "qrc:/icons/media-playback-pause.svg"
        }
        onStopped: {
            playerButton.source = "qrc:/icons/media-playback-start.svg"
            playerSlider.value=0
        }
        onPositionChanged: {
            playerSlider.sync = true
            playerSlider.value = audioPlayer.position / audioPlayer.duration
            playerSlider.sync = false
            timeLabel.text = ConvertScript.convertTimeString(audioPlayer.position) + "/" + ConvertScript.convertTimeString(audioPlayer.duration)
        }
    }

    RowLayout {
        AvatarImage {
            id: avatarRect
            avatarurl: i_avatar
            aliasname: i_aliasname
            username: i_username
        }

        Image {
            id: playerButton
            source: "qrc:/icons/media-playback-start.svg"
            width: 100
            height: 100
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log(RuqolaDebugCategorySingleton.category, "Click on download audio file");
                    if (audioPlayer.source !== "") {
                        if (audioPlayer.playbackState === MediaPlayer.PlayingState) {
                            audioPlayer.pause()
                        } else {
                            audioPlayer.play()
                        }
                    } else {
                        console.log(RuqolaDebugCategorySingleton.category, "Audio file no found");
                    }
                }
            }
        }
        QQC2.Slider {
            id: playerSlider
            Layout.fillWidth: true

            property bool sync: false

            onValueChanged: {
                if (!sync) {
                    audioPlayer.seek(value * audioPlayer.duration)
                }
            }
        }

        QQC2.Label {
            id: timeLabel
            text: "00:00/00:00"
        }

        DownloadButton {
            id: download
            onDownloadButtonClicked: {
                //TODO messageMain.downloadAttachment(model.modelData.link)
            }
        }

        TimestampText {
            id: timestampText
            timestamp: i_timestamp
        }
    }
}
