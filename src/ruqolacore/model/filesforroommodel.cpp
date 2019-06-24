/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#include "filesforroommodel.h"
#include "rocketchataccount.h"
#include "ruqola_debug.h"

FilesForRoomModel::FilesForRoomModel(RocketChatAccount *account, QObject *parent)
    : QAbstractListModel(parent)
    , mRochetChantAccount(account)
{
    mFileAttachments = new FileAttachments;
}

FilesForRoomModel::~FilesForRoomModel()
{
    delete mFileAttachments;
}

void FilesForRoomModel::addMoreFileAttachments(const QJsonObject &fileAttachmentsObj)
{
    const int numberOfElement = mFileAttachments->fileAttachments().count();
    mFileAttachments->parseMoreFileAttachments(fileAttachmentsObj);
    beginInsertRows(QModelIndex(), numberOfElement, mFileAttachments->fileAttachments().count() - 1);
    endInsertRows();
}

void FilesForRoomModel::initialize()
{
    mRoomId.clear();
}

void FilesForRoomModel::parseFileAttachments(const QJsonObject &fileAttachmentsObj, const QString &roomId)
{
    mRoomId = roomId;
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mFileAttachments->fileAttachments().count() - 1);
        mFileAttachments->clear();
        endRemoveRows();
    }
    mFileAttachments->parseFileAttachments(fileAttachmentsObj);
    if (!mFileAttachments->isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mFileAttachments->fileAttachments().count() - 1);
        endInsertRows();
    }
}

QString FilesForRoomModel::roomId() const
{
    return mRoomId;
}

void FilesForRoomModel::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

void FilesForRoomModel::setFiles(const QVector<File> &files)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mFileAttachments->fileAttachments().count() - 1);
        mFileAttachments->clear();
        endRemoveRows();
    }
    if (!files.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, files.count() - 1);
        mFileAttachments->setFileAttachments(files);
        endInsertRows();
    }
}

int FilesForRoomModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mFileAttachments->fileAttachments().count();
}

QVariant FilesForRoomModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mFileAttachments->fileAttachments().count()) {
        return QVariant();
    }

    const File file = mFileAttachments->fileAttachments().at(index.row());
    switch (role) {
    case FileName:
        return file.fileName();
    case UserId:
        return file.userId();
    case MimeType:
        return file.mimeType();
    case Url:
        return file.url();
    case Description:
        return file.description();
    case CanBeDeleted:
        return mRochetChantAccount->userID() == file.userId();
    case FileId:
        return file.fileId();
    case TimeStamp:
        return file.uploadedDateTimeStr();
    case UserName:
        return file.userName();
    case SortByTimeStamp:
        return file.uploadedAt();
    }
    return {};
}

QHash<int, QByteArray> FilesForRoomModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[FileName] = QByteArrayLiteral("filename");
    roles[UserId] = QByteArrayLiteral("userid");
    roles[MimeType] = QByteArrayLiteral("mimetype");
    roles[Url] = QByteArrayLiteral("url");
    roles[Description] = QByteArrayLiteral("description");
    roles[CanBeDeleted] = QByteArrayLiteral("canbedeleted");
    roles[FileId] = QByteArrayLiteral("fileid");
    roles[TimeStamp] = QByteArrayLiteral("timestamp");
    roles[UserName] = QByteArrayLiteral("username");
    return roles;
}

FileAttachments *FilesForRoomModel::fileAttachments() const
{
    return mFileAttachments;
}
