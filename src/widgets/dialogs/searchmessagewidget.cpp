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

#include "ruqola.h"
#include "rocketchataccount.h"
#include "searchmessagewidget.h"
#include "room/messagelistview.h"
#include "model/searchmessagefilterproxymodel.h"
#include <KLineEdit>
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

SearchMessageWidget::SearchMessageWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mSearchLineEdit = new KLineEdit(this);
    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setClearButtonEnabled(true);
    mSearchLineEdit->setTrapReturnKey(true);
    mSearchLineEdit->setPlaceholderText(i18n("Search Word... (You can use regular expression as /^text$/i)"));
    mainLayout->addWidget(mSearchLineEdit);

    mSearchLabel = new QLabel(this);
    mSearchLabel->setObjectName(QStringLiteral("mSearchLabel"));
    mainLayout->addWidget(mSearchLabel);

    mResultListWidget = new MessageListView(MessageListView::Mode::Viewing, this);
    mResultListWidget->setObjectName(QStringLiteral("mResultListWidget"));
    mainLayout->addWidget(mResultListWidget);
    connect(mSearchLineEdit, &QLineEdit::returnPressed, this, &SearchMessageWidget::slotSearchMessages);
    mResultListWidget->setModel(Ruqola::self()->rocketChatAccount()->searchMessageFilterProxyModel());
    connect(mResultListWidget, &MessageListView::goToMessageRequested,
            this, &SearchMessageWidget::goToMessageRequested);

}

SearchMessageWidget::~SearchMessageWidget()
{
    Ruqola::self()->rocketChatAccount()->messageSearch(QString(), mRoomId);
}

void SearchMessageWidget::slotSearchMessages()
{
    Ruqola::self()->rocketChatAccount()->messageSearch(mSearchLineEdit->text(), mRoomId);
}

QString SearchMessageWidget::roomId() const
{
    return mRoomId;
}

void SearchMessageWidget::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}


void SearchMessageWidget::updateLabel()
{
//    if (mDiscussionModel->loadMoreDiscussionsInProgress()) {
//        mDiscussionInfoLabel->setText(i18n("Loading..."));
//    } else {
//        mDiscussionInfoLabel->setText(mDiscussionModel->rowCount() == 0 ? i18n("No Discussion found") : displayShowDiscussionInRoom());
//    }
}

QString SearchMessageWidget::displayShowSearch() const
{
    QString displayMessageStr;
    //displayMessageStr = i18np("%1 Message in room (Total: %2)", "%1 Messages in room (Total: %2)", mDiscussionModel->rowCount(), mDiscussionModel->total());
//    if (!mDiscussionModel->hasFullList()) {
//        displayMessageStr += QStringLiteral(" <a href=\"loadmoreelement\">%1</a>").arg(i18n("(Click here for Loading more...)"));
//    }
    return displayMessageStr;
}
