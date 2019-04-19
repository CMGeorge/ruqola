/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#include "unicodeemoticonparser.h"
#include <QJsonObject>
#include <QJsonArray>

UnicodeEmoticonParser::UnicodeEmoticonParser()
{
}

UnicodeEmoticonParser::~UnicodeEmoticonParser()
{
}

QVector<UnicodeEmoticon> UnicodeEmoticonParser::parse(const QJsonObject &o) const
{
    QVector<UnicodeEmoticon> lstEmoticons;
    for (const QString &key: o.keys()) {
        UnicodeEmoticon emoticon;
        QJsonObject emojiObj = o[key].toObject();
        emoticon.setUnicode(emojiObj[QStringLiteral("unicode")].toString());
        emoticon.setCategory(emojiObj[QStringLiteral("category")].toString());
        emoticon.setIdentifier(emojiObj[QStringLiteral("shortname")].toString());
        emoticon.setOrder(emojiObj[QStringLiteral("emoji_order")].toInt());
        const QJsonArray aliasArray = emojiObj[QStringLiteral("aliases_ascii")].toArray();
        if (!aliasArray.isEmpty()) {
            QStringList lst;
            const int aliasArrayCount = aliasArray.count();
            lst.reserve(aliasArrayCount);
            for (int i = 0; i < aliasArrayCount; ++i) {
                lst.append(aliasArray.at(i).toString());
            }
            emoticon.setAliases(lst);
        }
        if (emoticon.isValid()) {
            lstEmoticons.append(emoticon);
        }
    }
    return lstEmoticons;
}
