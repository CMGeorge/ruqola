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

#include "channelactionpopupmenu.h"
#include "roomwrapper.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QMenu>

ChannelActionPopupMenu::ChannelActionPopupMenu(QObject *parent)
    : QObject(parent)
{
    mMenu = new QMenu;
    mMenu->setObjectName(QStringLiteral("mMenu"));
    connect(mMenu, &QMenu::aboutToShow, this, &ChannelActionPopupMenu::slotUpdateMenu);
    createMenu();
}

ChannelActionPopupMenu::~ChannelActionPopupMenu()
{
    delete mMenu;
}

void ChannelActionPopupMenu::createMenu()
{
    mShowMentions = new QAction(i18n("Show Mentions..."), this);
    mMenu->addAction(mShowMentions);
    connect(mShowMentions, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowMentions);
    });

    mShowPinnedMessages = new QAction(i18n("Show Pinned Messages..."), this);
    mMenu->addAction(mShowPinnedMessages);
    connect(mShowPinnedMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowPinned);
    });

    mShowStarredMessages = new QAction(i18n("Show Starred Messages..."), this);
    mMenu->addAction(mShowStarredMessages);
    connect(mShowStarredMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowStarred);
    });

    mShowSnipperedMessages = new QAction(i18n("Show Snippered Messages..."), this);
    mMenu->addAction(mShowSnipperedMessages);
    connect(mShowSnipperedMessages, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowSnippered);
    });

    mShowFileAttachments = new QAction(QIcon::fromTheme(QStringLiteral("document-send-symbolic")), i18n("Show File Attachments..."), this);
    mMenu->addAction(mShowFileAttachments);
    connect(mShowFileAttachments, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowAttachment);
    });

    mShowDiscussions = new QAction(i18n("Show Discussions..."), this);
    mMenu->addAction(mShowDiscussions);
    connect(mShowDiscussions, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowDiscussions);
    });

    mShowThreads = new QAction(i18n("Show Threads..."), this);
    mMenu->addAction(mShowThreads);
    connect(mShowThreads, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::ShowThreads);
    });

    createSeparator();

    mConfigureNotification = new QAction(QIcon::fromTheme(QStringLiteral("preferences-desktop-notification")), i18n("Configure Notification..."), this);
    mMenu->addAction(mConfigureNotification);
    connect(mConfigureNotification, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::Notification);
    });

    mAutoTranslateSeparator = createSeparator();
    mAutoTranslate = new QAction(i18n("Configure Auto-Translate..."), this);
    mMenu->addAction(mAutoTranslate);
    connect(mAutoTranslate, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::AutoTranslate);
    });

    mInviteUsersGenerateUrlSeparator = createSeparator();
    mMenu->addAction(mInviteUsersGenerateUrlSeparator);
    mInviteUsersGenerateUrl = new QAction(i18n("Invite Users"), this);
    mMenu->addAction(mInviteUsersGenerateUrl);
    connect(mInviteUsersGenerateUrl, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::InviteUsers);
    });

    mAddUserInRoomsSeparator = createSeparator();
    mAddUserInRooms = new QAction(i18n("Add Users in Channel..."), this);
    mMenu->addAction(mAddUserInRoomsSeparator);
    connect(mAddUserInRooms, &QAction::triggered, this, [this]() {
        Q_EMIT actionRequested(RoomHeaderWidget::AddUsersInRoom);
    });
    mMenu->addAction(mAddUserInRooms);
}

QAction *ChannelActionPopupMenu::createSeparator()
{
    return mMenu->addSeparator();
}

QMenu *ChannelActionPopupMenu::menu() const
{
    return mMenu;
}

void ChannelActionPopupMenu::setRoomWrapper(RoomWrapper *roomWrapper)
{
    mRoomWrapper = roomWrapper;
}

void ChannelActionPopupMenu::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mCurrentRocketChatAccount = account;
}

void ChannelActionPopupMenu::slotUpdateMenu()
{
    mShowPinnedMessages->setVisible(mCurrentRocketChatAccount->hasPinnedMessagesSupport() && mCurrentRocketChatAccount->allowMessagePinningEnabled());
    mShowStarredMessages->setVisible(mCurrentRocketChatAccount->hasStarredMessagesSupport() && mCurrentRocketChatAccount->allowMessageStarringEnabled());
    mShowSnipperedMessages->setVisible(mCurrentRocketChatAccount->hasSnippetedMessagesSupport() && mCurrentRocketChatAccount->allowMessageSnippetingEnabled());
    mAutoTranslate->setVisible(mCurrentRocketChatAccount->autoTranslateEnabled());
    mAutoTranslateSeparator->setVisible(mCurrentRocketChatAccount->autoTranslateEnabled());
    //TODO fix me. We can't generate it if we are not owner
    mInviteUsersGenerateUrl->setVisible(mCurrentRocketChatAccount->hasInviteUserSupport());
    mInviteUsersGenerateUrlSeparator->setVisible(mCurrentRocketChatAccount->hasInviteUserSupport());

    mAddUserInRoomsSeparator->setVisible(mRoomWrapper && mRoomWrapper->canBeModify());
    mAddUserInRooms->setVisible(mRoomWrapper && mRoomWrapper->canBeModify());
}
