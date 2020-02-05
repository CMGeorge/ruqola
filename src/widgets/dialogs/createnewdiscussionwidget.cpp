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

#include "createnewdiscussionwidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <KLineEdit>
#include <KLocalizedString>
#include <QTextEdit>

CreateNewDiscussionWidget::CreateNewDiscussionWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *channelLabel = new QLabel(i18n("Channel or Group parent"), this);
    channelLabel->setObjectName(QStringLiteral("channelLabel"));
    mainLayout->addWidget(channelLabel);

    mChannelNameLineEdit = new KLineEdit(this);
    mChannelNameLineEdit->setObjectName(QStringLiteral("mChannelNameLineEdit"));
    mChannelNameLineEdit->setTrapReturnKey(true);
    mChannelNameLineEdit->setClearButtonEnabled(true);
    connect(mChannelNameLineEdit, &KLineEdit::textChanged, this, [this](const QString &str) {
        Q_EMIT updateOkButton(!str.trimmed().isEmpty());
    });
    mainLayout->addWidget(mChannelNameLineEdit);

    QLabel *discussionName = new QLabel(i18n("Discussion Name"), this);
    discussionName->setObjectName(QStringLiteral("discussionName"));
    mainLayout->addWidget(discussionName);

    mDiscussionNameLineEdit = new KLineEdit(this);
    mDiscussionNameLineEdit->setObjectName(QStringLiteral("mDiscussionNameLineEdit"));
    mDiscussionNameLineEdit->setTrapReturnKey(true);
    mDiscussionNameLineEdit->setClearButtonEnabled(true);
    mainLayout->addWidget(mDiscussionNameLineEdit);

    QLabel *usersLabel = new QLabel(i18n("Invite Users"), this);
    discussionName->setObjectName(QStringLiteral("usersLabel"));
    mainLayout->addWidget(usersLabel);

    //TODO use completer lineedit
    mUsersLineEdit = new KLineEdit(this);
    mUsersLineEdit->setObjectName(QStringLiteral("mUsersLineEdit"));
    mUsersLineEdit->setTrapReturnKey(true);
    mUsersLineEdit->setClearButtonEnabled(true);
    mainLayout->addWidget(mUsersLineEdit);

    QLabel *messageLabel = new QLabel(i18n("Message"), this);
    messageLabel->setObjectName(QStringLiteral("messageLabel"));
    mainLayout->addWidget(messageLabel);

    //TODO use ktextedit ? QPlainTextEdit ?
    mMessageTextEdit = new QTextEdit(this);
    mMessageTextEdit->setAcceptRichText(false);
    mMessageTextEdit->setObjectName(QStringLiteral("mMessageTextEdit"));
    mainLayout->addWidget(mMessageTextEdit);
}

CreateNewDiscussionWidget::~CreateNewDiscussionWidget()
{
}

void CreateNewDiscussionWidget::setChannelName(const QString &name)
{
    mChannelNameLineEdit->setText(name);
    mChannelNameLineEdit->setReadOnly(true);
}

QString CreateNewDiscussionWidget::channelName() const
{
    return mChannelNameLineEdit->text();
}

void CreateNewDiscussionWidget::setDiscussionName(const QString &name)
{
    mDiscussionNameLineEdit->setText(name);
}

QString CreateNewDiscussionWidget::discussionName() const
{
    return mDiscussionNameLineEdit->text();
}

void CreateNewDiscussionWidget::setMessage(const QString &name)
{
    mMessageTextEdit->setText(name);
}

QString CreateNewDiscussionWidget::message() const
{
    return mMessageTextEdit->toPlainText();
}

void CreateNewDiscussionWidget::setUsers(const QStringList &users)
{
    mUsersLineEdit->setText(users.join(QLatin1Char(',')));
}

QStringList CreateNewDiscussionWidget::users() const
{
    //Use , or ; ?
    return mUsersLineEdit->text().split(QLatin1Char(','));
}
