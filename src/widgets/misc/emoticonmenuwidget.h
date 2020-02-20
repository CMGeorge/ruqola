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

#ifndef EMOTICONMENUWIDGET_H
#define EMOTICONMENUWIDGET_H
#include <QWidget>

#include "libruqolawidgets_private_export.h"

class QTabWidget;
class QLineEdit;
class RocketChatAccount;
class KCategorizedSortFilterProxyModel;
class EmoticonCategorizedModel;

class LIBRUQOLAWIDGETS_TESTS_EXPORT EmoticonMenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EmoticonMenuWidget(QWidget *parent = nullptr);
    ~EmoticonMenuWidget() override;

    void setCurrentRocketChatAccount(RocketChatAccount *account);
Q_SIGNALS:
    void insertEmoticons(const QString &emoticon);
private:
    //void createTabs(RocketChatAccount *account);

    QTabWidget *mTabWidget = nullptr;
    QLineEdit *mFilterLineEdit = nullptr;
    KCategorizedSortFilterProxyModel *mProxyModel = nullptr;
    EmoticonCategorizedModel *mEmoticonModel = nullptr;
};

#endif // EMOTICONMENUWIDGET_H
