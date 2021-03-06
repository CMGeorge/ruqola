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

#ifndef THREADMESSAGEWIDGET_H
#define THREADMESSAGEWIDGET_H

#include <QWidget>
#include "libruqolawidgets_private_export.h"
class MessageListView;
class MessageLineWidget;
class RocketChatAccount;
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ThreadMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ThreadMessageWidget(QWidget *parent = nullptr);
    ~ThreadMessageWidget();

    Q_REQUIRED_RESULT QString threadMessageId() const;
    void setThreadMessageId(const QString &threadMessageId);

    void setCurrentRocketChatAccount(RocketChatAccount *account);

    void setRoomId(const QString &roomId);

    void setThreadPreview(const QString &preview);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void slotSendMessage(const QString &msg);

    QString mThreadMessageId;
    QLabel *mThreadPreview = nullptr;
    MessageListView *mMessageListView = nullptr;
    MessageLineWidget *mMessageLineWidget = nullptr;
};

#endif // THREADMESSAGEWIDGET_H
