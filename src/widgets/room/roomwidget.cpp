/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "roomwidget.h"
#include "roomheaderwidget.h"
#include "messagelistview.h"
#include "messagelinewidget.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "roomwrapper.h"
#include "readonlylineeditwidget.h"
#include "messagelineedit.h"
#include "ruqolawidgets_debug.h"

#include <KLocalizedString>

#include <QKeyEvent>
#include <QHBoxLayout>
#include <QApplication>
#include <QStackedWidget>
#include <QMimeData>

RoomWidget::RoomWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mRoomHeaderWidget = new RoomHeaderWidget(this);
    mRoomHeaderWidget->setObjectName(QStringLiteral("mRoomHeaderWidget"));
    mainLayout->addWidget(mRoomHeaderWidget);

    mMessageListView = new MessageListView(this);
    mMessageListView->setObjectName(QStringLiteral("mMessageListView"));
    mainLayout->addWidget(mMessageListView);

    mStackedWidget = new QStackedWidget(this);
    mStackedWidget->setObjectName(QStringLiteral("mStackedWidget"));
    mainLayout->addWidget(mStackedWidget);

    mMessageLineWidget = new MessageLineWidget(this);
    mMessageLineWidget->setObjectName(QStringLiteral("mMessageLineWidget"));
    mStackedWidget->addWidget(mMessageLineWidget);

    mReadOnlyLineEditWidget = new ReadOnlyLineEditWidget(this);
    mReadOnlyLineEditWidget->setObjectName(QStringLiteral("mReadOnlyLineEditWidget"));
    mStackedWidget->addWidget(mReadOnlyLineEditWidget);

    mStackedWidget->setCurrentWidget(mMessageLineWidget);
    mStackedWidget->setMaximumHeight(mMessageLineWidget->height());

    connect(this, &RoomWidget::channelSelected, this, &RoomWidget::setChannelSelected);
    connect(mMessageLineWidget, &MessageLineWidget::sendMessage, this, &RoomWidget::slotSendMessage);
    connect(mMessageLineWidget, &MessageLineWidget::sendFile, this, &RoomWidget::slotSendFile);
    connect(mMessageLineWidget, &MessageLineWidget::textEditing, this, &RoomWidget::slotTextEditing);
    connect(mMessageLineWidget->messageLineEdit(), &MessageLineEdit::keyPressed, this, &RoomWidget::keyPressedInLineEdit);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::favoriteChanged, this, &RoomWidget::slotChangeFavorite);
    connect(mRoomHeaderWidget, &RoomHeaderWidget::encryptedChanged, this, &RoomWidget::slotEncryptedChanged);

    connect(mMessageListView, &MessageListView::editMessageRequested, this, &RoomWidget::slotEditMessage);

    setAcceptDrops(true);
}

RoomWidget::~RoomWidget()
{
    delete mRoomWrapper;
}

void RoomWidget::slotTextEditing(bool clearNotification)
{
    mCurrentRocketChatAccount->textEditing(mRoomId, clearNotification);
}

void RoomWidget::slotSendFile(const UploadFileDialog::UploadFileInfo &uploadFileInfo)
{
    mCurrentRocketChatAccount->uploadFile(mRoomId, uploadFileInfo.description, QString(), uploadFileInfo.fileUrl);
}

void RoomWidget::slotSendMessage(const QString &msg)
{
    if (mMessageIdBeingEdited.isEmpty()) {
        mCurrentRocketChatAccount->sendMessage(mRoomId, msg);
    } else {
        mCurrentRocketChatAccount->updateMessage(mRoomId, mMessageIdBeingEdited, msg);
        mMessageIdBeingEdited.clear();
    }
    mMessageLineWidget->setMode(MessageLineWidget::EditingMode::NewMessage);
}

void RoomWidget::slotEditMessage(const QString &messageId, const QString &text)
{
    mMessageIdBeingEdited = messageId;
    mMessageLineWidget->setMode(MessageLineWidget::EditingMode::EditMessage);
    mMessageLineWidget->setText(text);
    mMessageLineWidget->setFocus();
}

void RoomWidget::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        event->accept();
    }
}

void RoomWidget::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        const QList<QUrl> urls = mimeData->urls();
        for (const QUrl &url : urls) {
            if (url.isLocalFile()) {
                QPointer<UploadFileDialog> dlg = new UploadFileDialog(this);
                dlg->setFileUrl(url);
                if (dlg->exec()) {
                    const UploadFileDialog::UploadFileInfo uploadFileInfo = dlg->fileInfo();
                    slotSendFile(uploadFileInfo);
                }
            }
        }
    }
}

void RoomWidget::setChannelSelected(const QModelIndex &index)
{
    if (mMessageLineWidget->text().isEmpty()) {
        mPendingTypedTexts.remove(mRoomId);
    } else {
        PendingTypedInfo info;
        info.text = mMessageLineWidget->text();
        info.messageIdBeingEdited = mMessageIdBeingEdited;
        mPendingTypedTexts[mRoomId] = info;
    }

    const QString roomId = index.data(RoomModel::RoomID).toString();
    setRoomId(roomId);
    setRoomType(index.data(RoomModel::RoomType).toString());
    const PendingTypedInfo currentPendingInfo = mPendingTypedTexts.value(roomId);
    mMessageLineWidget->setText(currentPendingInfo.text);
    mMessageIdBeingEdited = currentPendingInfo.messageIdBeingEdited;
    mMessageLineWidget->setMode(mMessageIdBeingEdited.isEmpty() ? MessageLineWidget::EditingMode::NewMessage : MessageLineWidget::EditingMode::EditMessage);

    mMessageLineWidget->setFocus();
}

void RoomWidget::updateRoomHeader()
{
    if (mRoomWrapper) {
        mRoomHeaderWidget->setRoomName(mRoomWrapper->name());
        mRoomHeaderWidget->setRoomAnnouncement(mRoomWrapper->announcement());
        mRoomHeaderWidget->setRoomTopic(mRoomWrapper->topic());
        mRoomHeaderWidget->setFavoriteStatus(mRoomWrapper->favorite());
        mRoomHeaderWidget->setEncypted(mRoomWrapper->encrypted());
        //TODO Description ?

        if (mRoomWrapper->roomMessageInfo().isEmpty()) {
            mStackedWidget->setCurrentWidget(mMessageLineWidget);
        } else {
            mStackedWidget->setCurrentWidget(mReadOnlyLineEditWidget);
            mReadOnlyLineEditWidget->setMessage(mRoomWrapper->roomMessageInfo());
        }
    } else {
        //Hide it
    }
}

QString RoomWidget::roomId() const
{
    return mRoomId;
}

void RoomWidget::setRoomType(const QString &roomType)
{
    mRoomType = roomType;
}

RoomWrapper *RoomWidget::roomWrapper() const
{
    return mRoomWrapper;
}

void RoomWidget::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
    delete mRoomWrapper;
    mRoomWrapper = mCurrentRocketChatAccount->roomWrapper(mRoomId);
    connectRoomWrapper();
    mMessageListView->setChannelSelected(roomId);
}

void RoomWidget::connectRoomWrapper()
{
    if (mRoomWrapper) {
        connect(mRoomWrapper, &RoomWrapper::announcementChanged, this, [this]() {
            mRoomHeaderWidget->setRoomAnnouncement(mRoomWrapper->announcement());
        });
        connect(mRoomWrapper, &RoomWrapper::topicChanged, this, [this]() {
            mRoomHeaderWidget->setRoomTopic(mRoomWrapper->topic());
        });
        connect(mRoomWrapper, &RoomWrapper::nameChanged, this, [this]() {
            mRoomHeaderWidget->setRoomName(mRoomWrapper->name());
        });
        connect(mRoomWrapper, &RoomWrapper::favoriteChanged, this, [this]() {
            mRoomHeaderWidget->setFavoriteStatus(mRoomWrapper->favorite());
        });
        connect(mRoomWrapper, &RoomWrapper::encryptedChanged, this, [this]() {
            mRoomHeaderWidget->setEncypted(mRoomWrapper->encrypted());
        });
    }
    updateRoomHeader();
}

void RoomWidget::slotClearNotification()
{
    mCurrentRocketChatAccount->clearUnreadMessages(mRoomId);
}

void RoomWidget::slotEncryptedChanged(bool b)
{
    qCWarning(RUQOLAWIDGETS_LOG) << "change encrypted not supported yet";
    //TODO mCurrentRocketChatAccount->slot
}

void RoomWidget::slotChangeFavorite(bool b)
{
    mCurrentRocketChatAccount->changeFavorite(mRoomId, b);
}

void RoomWidget::keyPressedInLineEdit(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Escape) {
        if (!mMessageIdBeingEdited.isEmpty()) {
            mMessageIdBeingEdited.clear();
            mMessageLineWidget->setText(QString());
        } else {
            slotClearNotification();
        }
        ev->accept();
    } else {
        mMessageListView->handleKeyPressEvent(ev);
    }
}

QString RoomWidget::roomType() const
{
    return mRoomType;
}

void RoomWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mCurrentRocketChatAccount = account;
    mMessageLineWidget->setCurrentRocketChatAccount(account);
    mRoomId.clear(); //Clear it otherwise if we switch between two account with same roomId (as "GENERAL") we will see incorrect room.
}
