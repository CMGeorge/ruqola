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

#ifndef CHANNELLISTWIDGET_H
#define CHANNELLISTWIDGET_H

#include <QWidget>
#include "libruqolawidgets_private_export.h"
#include "user.h"
class StatusCombobox;
class ChannelListView;
class QLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelListWidget(QWidget *parent = nullptr);
    ~ChannelListWidget();

Q_SIGNALS:
    void channelSelected(const QModelIndex &index);

private:
    void setUserStatusUpdated(User::PresenceStatus status);
    void slotStatusChanged();
    StatusCombobox *mStatusComboBox = nullptr;
    ChannelListView *mChannelView = nullptr;
    QLineEdit *mSearchRoom = nullptr;
};

#endif // CHANNELLISTWIDGET_H
