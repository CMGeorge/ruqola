/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef THREADSMODEL_H
#define THREADSMODEL_H

#include "libruqola_private_export.h"
#include "threads.h"
#include <QAbstractListModel>

class LIBRUQOLACORE_TESTS_EXPORT ThreadsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ThreadRoles {
        ThreadMessageId = Qt::UserRole + 1,
        Description,
        NumberOfMessages,
        LastMessage,
        TimeStamp
    };
    Q_ENUM(ThreadRoles)

    explicit ThreadsModel(QObject *parent = nullptr);
    ~ThreadsModel() override;

    Q_REQUIRED_RESULT Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    Q_REQUIRED_RESULT QHash<int, QByteArray> roleNames() const override;

    void setThreads(const Threads &threads);
private:
    Q_DISABLE_COPY(ThreadsModel)
    Threads mThreads;
};

#endif // DISCUSSIONSMODEL_H
