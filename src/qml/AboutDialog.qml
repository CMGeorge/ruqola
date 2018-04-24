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
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Window 2.0
import KDE.Ruqola.RuqolaAboutDataAuthorModel 1.0
import KDE.Ruqola.RuqolaAboutData 1.0

import org.kde.kirigami 2.1 as Kirigami
QQC2.Dialog {
    id: aboutDialog

    title: i18n("About Ruqola")

    standardButtons: QQC2.Dialog.Close

    signal openurl(string link)

    property QtObject applicationData

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    width: Math.min(bar.contentWidth + Kirigami.Units.smallSpacing * 2, 500)
    height: 600
    modal: true

    Item {
        id: contentRect
        anchors.fill: parent
        anchors.leftMargin: Kirigami.Units.smallSpacing * 2
        anchors.rightMargin: Kirigami.Units.smallSpacing * 2
        clip: true

        QQC2.TabBar {
            id: bar
            width: parent.width

            QQC2.TabButton {
                text: i18n("About")
            }
            QQC2.TabButton {
                text: i18n("Libraries")
            }
            QQC2.TabButton {
                text: i18n("Author")
            }
            QQC2.TabButton {
                text: i18n("Thanks To")
                visible: applicationData.creditsModel.rowCount() > 0
            }
            QQC2.TabButton {
                text: i18n("Translation")
                visible: applicationData.translatorModel.rowCount() > 0
            }
        }

        StackLayout {
            anchors.top: bar.bottom
            anchors.bottom: parent.bottom
            width: parent.width
            currentIndex: bar.currentIndex

            Item {
                id: aboutTab
                ColumnLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    QQC2.Label {
                        text: applicationData.title
                        wrapMode: QQC2.Label.Wrap
                        textFormat: Qt.RichText
                    }
                    QQC2.Label {
                        text: applicationData.about
                        wrapMode: QQC2.Label.Wrap
                        textFormat: Qt.RichText
                    }
                    QQC2.Label {
                        text: applicationData.licenses
                        wrapMode: QQC2.Label.Wrap
                        textFormat: Qt.RichText
                        onLinkActivated: {
                            licenseDialog.licenseText = applicationData.licenseText(link);
                            licenseDialog.open();
                        }
                    }
                }
            }
            Item {
                id: librariesTab
                ColumnLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.topMargin: Kirigami.Units.smallSpacing

                    QQC2.Label {
                        text: applicationData.libraries
                        wrapMode: QQC2.Label.Wrap
                        textFormat: Qt.RichText
                    }
                }
            }
            Item {
                id: authorTab
                anchors.fill: parent

                QQC2.Label {
                    id: reportBugsLink
                    anchors.top: parent.top
                    anchors.topMargin: Kirigami.Units.smallSpacing
                    visible: applicationData.reportBugs !== ""
                    text: applicationData.reportBugs
                    wrapMode: QQC2.Label.Wrap
                    textFormat: Qt.RichText
                    onLinkActivated: {
                        aboutDialog.openurl(link);
                    }
                }
                QQC2.ScrollView {
                    anchors.top: reportBugsLink.bottom
                    anchors.topMargin: Kirigami.Units.largeSpacing
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: Kirigami.Units.largeSpacing
                    anchors.rightMargin: Kirigami.Units.largeSpacing

                    contentHeight: authorsColumn.height
                    contentWidth: width

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
                                model: applicationData.authorModel

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
            }
            Item {
                id: thanksToTab
                ColumnLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.topMargin: Kirigami.Units.smallSpacing

                    QQC2.ScrollView {
                        id: thanksToTabview
                        width: thanksToTab.width
                        height: thanksToTab.height

                        Column {
                            Repeater {
                                id: creditList

                                model: applicationData.creditsModel
                                Column {
                                    QQC2.Label {
                                        text: username
                                        wrapMode: QQC2.Label.Wrap
                                        anchors.leftMargin: Kirigami.Units.smallSpacing
                                        anchors.rightMargin: Kirigami.Units.smallSpacing
                                        renderType: Text.NativeRendering
                                        textFormat: Text.RichText
                                        font.bold: true
                                    }
                                    QQC2.Label {
                                        text: task
                                        wrapMode: QQC2.Label.Wrap
                                        anchors.leftMargin: Kirigami.Units.smallSpacing
                                        anchors.rightMargin: Kirigami.Units.smallSpacing
                                        renderType: Text.NativeRendering
                                        textFormat: Text.RichText
                                    }
                                    QQC2.Label {
                                        text: email
                                        wrapMode: QQC2.Label.Wrap
                                        anchors.leftMargin: Kirigami.Units.smallSpacing
                                        anchors.rightMargin: Kirigami.Units.smallSpacing
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
            }
            Item {
                id: translatorToTab
                ColumnLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.topMargin: Kirigami.Units.smallSpacing

                    QQC2.ScrollView {
                        id: translatorToTabView
                        width: translatorToTab.width
                        height: translatorToTab.height

                        Column {
                            Repeater {
                                id: translatorList

                                model: applicationData.translatorModel
                                Column {
                                    QQC2.Label {
                                        text: username
                                        wrapMode: QQC2.Label.Wrap
                                        anchors.leftMargin: Kirigami.Units.smallSpacing
                                        anchors.rightMargin: Kirigami.Units.smallSpacing
                                        renderType: Text.NativeRendering
                                        textFormat: Text.RichText
                                        font.bold: true
                                    }
                                    QQC2.Label {
                                        text: task
                                        wrapMode: QQC2.Label.Wrap
                                        anchors.leftMargin: Kirigami.Units.smallSpacing
                                        anchors.rightMargin: Kirigami.Units.smallSpacing
                                        renderType: Text.NativeRendering
                                        textFormat: Text.RichText
                                    }
                                    QQC2.Label {
                                        text: email
                                        wrapMode: QQC2.Label.Wrap
                                        anchors.leftMargin: Kirigami.Units.smallSpacing
                                        anchors.rightMargin: Kirigami.Units.smallSpacing
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
            }
        }
    }

    LicenseDialog {
        id: licenseDialog
    }
}
