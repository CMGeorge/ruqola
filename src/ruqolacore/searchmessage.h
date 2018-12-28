/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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

#ifndef SEARCHMESSAGE_H
#define SEARCHMESSAGE_H

#include <QDebug>
#include <QJsonObject>
#include "libruqola_private_export.h"
#include "message.h"

class LIBRUQOLACORE_TESTS_EXPORT SearchMessage
{
    Q_GADGET
public:
    SearchMessage();

    void parseResult(const QJsonObject &obj, bool restApi);
    Q_REQUIRED_RESULT QString text() const;
    Q_REQUIRED_RESULT qint64 timeStamp() const;
private:
    Message mMessage;
};
Q_DECLARE_METATYPE(SearchMessage)
Q_DECLARE_TYPEINFO(SearchMessage, Q_MOVABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator <<(QDebug d, const SearchMessage &t);

#endif // SEARCHMESSAGE_H
