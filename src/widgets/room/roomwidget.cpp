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
#include "roomlistview.h"
#include "messagelineedit.h"
#include <QHBoxLayout>
#include <KLocalizedString>
#include "ruqola.h"
#include "rocketchataccount.h"

RoomWidget::RoomWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mRoomHeaderWidget = new RoomHeaderWidget(this);
    mRoomHeaderWidget->setObjectName(QStringLiteral("mRoomHeaderWidget"));
    mainLayout->addWidget(mRoomHeaderWidget);

    mRoomListView = new RoomListView(this);
    mRoomListView->setObjectName(QStringLiteral("mRoomListView"));
    mainLayout->addWidget(mRoomListView);

    mMessageLineEdit = new MessageLineEdit(this);
    mMessageLineEdit->setObjectName(QStringLiteral("mMessageLineEdit"));
    mainLayout->addWidget(mMessageLineEdit);

    connect(this, &RoomWidget::channelSelected, this, &RoomWidget::setChannelSelected);
    connect(mMessageLineEdit, &MessageLineEdit::sendMessage, this, &RoomWidget::slotSendMessage);
}

RoomWidget::~RoomWidget()
{
}

void RoomWidget::slotSendMessage(const QString &msg)
{
    Ruqola::self()->rocketChatAccount()->sendMessage(mRoomId, msg);
}

void RoomWidget::setChannelSelected(const QModelIndex &index)
{
    setRoomId(index.data(RoomModel::RoomID).toString());
    mRoomHeaderWidget->setRoomName(index.data(RoomModel::RoomFName).toString());
}

QString RoomWidget::roomId() const
{
    return mRoomId;
}

void RoomWidget::setRoomId(const QString &roomId)
{
    if (mRoomId != roomId) {
        mRoomId = roomId;
        mRoomListView->setChannelSelected(roomId);
    }
}
