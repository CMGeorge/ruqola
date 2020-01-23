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

#include "uploadfilewidget.h"
#include <KLocalizedString>
#include <KUrlRequester>

#include <QFormLayout>
#include <QLineEdit>

UploadFileWidget::UploadFileWidget(QWidget *parent)
    : QWidget(parent)
{
    QFormLayout *layout = new QFormLayout(this);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins(0, 0, 0, 0);

    mDescription = new QLineEdit(this);
    mDescription->setObjectName(QStringLiteral("mDescription"));
    mDescription->setClearButtonEnabled(true);
    layout->addRow(i18n("Description:"), mDescription);

    mMessage = new QLineEdit(this);
    mMessage->setObjectName(QStringLiteral("mMessage"));
    mMessage->setClearButtonEnabled(true);
    layout->addRow(i18n("Message:"), mMessage);

    mSelectFile = new KUrlRequester(this);
    mSelectFile->setObjectName(QStringLiteral("mSelectFile"));
    layout->addRow(i18n("File:"), mSelectFile);
}

UploadFileWidget::~UploadFileWidget()
{
}

QString UploadFileWidget::description() const
{
    return mDescription->text();
}

QString UploadFileWidget::message() const
{
    return mMessage->text();
}

QUrl UploadFileWidget::fileUrl() const
{
    return mSelectFile->url();
}