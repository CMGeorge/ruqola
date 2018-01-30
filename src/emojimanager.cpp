/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "emojimanager.h"

#include <QJsonObject>
#include "ruqola_debug.h"

//TODO cache emoji ?

EmojiManager::EmojiManager(QObject *parent)
    : QObject(parent)
{
}

EmojiManager::~EmojiManager()
{
}

void EmojiManager::loadEmoji(const QJsonObject &obj)
{
    mEmojiList.clear();
    const QJsonArray result = obj.value(QLatin1String("result")).toArray();
    for (int i = 0; i < result.size(); i++) {
        const QJsonObject emojiJson = result.at(i).toObject();
        Emoji emoji;
        emoji.parseEmoji(emojiJson);
        if (emoji.isValid()) {
            mEmojiList.append(emoji);
        }
    }
}

int EmojiManager::count() const
{
    return mEmojiList.count();
}

QString EmojiManager::html(const QString &emojiIdentifier)
{
    if (emojiIdentifier.startsWith(QLatin1Char(':')) && emojiIdentifier.endsWith(QLatin1Char(':'))) {
        for (int i = 0, total = mEmojiList.size(); i < total; ++i) {
            if (mEmojiList.at(i).emojiIdentifier() == emojiIdentifier) {
                Emoji emoji = mEmojiList[i];
                const QString htmlStr = emoji.html(/* server url */ QString());
                mEmojiList.replace(i, emoji);
                return htmlStr;
            }
        }
    } else {
        qCWarning(RUQOLA_LOG) << "Emoji identifier is not correct :" << emojiIdentifier;
    }
    return emojiIdentifier;
}
