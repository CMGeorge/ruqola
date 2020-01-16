/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "emoticons/emojimanager.h"
#include "emoticons/unicodeemoticonparser.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include "ruqola_debug.h"

//TODO cache emoji ?

EmojiManager::EmojiManager(QObject *parent, bool loadUnicode)
    : QObject(parent)
{
    if (loadUnicode) {
        loadUnicodeEmoji();
    }
}

EmojiManager::~EmojiManager()
{
}

void EmojiManager::loadUnicodeEmoji()
{
    UnicodeEmoticonParser unicodeParser;
    QFile file(QStringLiteral(":/emoji.json"));
    if (!file.open(QFile::ReadOnly)) {
        qCWarning(RUQOLA_LOG) << "Impossible to open file: " << file.errorString();
        return;
    }
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    const QJsonObject obj = doc.object();
    mUnicodeEmojiList = unicodeParser.parse(obj);
}

QMap<QString, QVector<UnicodeEmoticon> > EmojiManager::unicodeEmojiList() const
{
    return mUnicodeEmojiList;
}

void EmojiManager::loadCustomEmoji(const QJsonObject &obj, bool restApi)
{
    mCustomEmojiList.clear();
    if (!restApi) {
        const QJsonArray result = obj.value(QLatin1String("result")).toArray();
        for (int i = 0; i < result.size(); i++) {
            const QJsonObject emojiJson = result.at(i).toObject();
            Emoji emoji;
            emoji.parseEmoji(emojiJson, restApi);
            if (emoji.isValid()) {
                mCustomEmojiList.append(emoji);
            }
        }
    } else {
        const QJsonObject result = obj.value(QLatin1String("emojis")).toObject();
        const QJsonArray array = result.value(QLatin1String("update")).toArray();
        //TODO add support for remove when we store it in local
        for (int i = 0; i < array.size(); i++) {
            const QJsonObject emojiJson = array.at(i).toObject();
            Emoji emoji;
            emoji.parseEmoji(emojiJson, restApi);
            if (emoji.isValid()) {
                mCustomEmojiList.append(emoji);
            }
        }
    }
}

int EmojiManager::count() const
{
    return mCustomEmojiList.count() + mUnicodeEmojiList.count();
}

bool EmojiManager::isAnimatedImage(const QString &emojiIdentifier) const
{
    if (emojiIdentifier.startsWith(QLatin1Char(':')) && emojiIdentifier.endsWith(QLatin1Char(':'))) {
        for (int i = 0, total = mCustomEmojiList.size(); i < total; ++i) {
            const Emoji emoji = mCustomEmojiList.at(i);
            if (emoji.hasEmoji(emojiIdentifier)) {
                return emoji.isAnimatedImage();
            }
        }
    }
    return false;
}

QString EmojiManager::replaceEmojiIdentifier(const QString &emojiIdentifier, bool isReaction)
{
    if (mServerUrl.isEmpty()) {
        qCWarning(RUQOLA_LOG) << "Server Url not defined";
        return emojiIdentifier;
    }
    if (emojiIdentifier.startsWith(QLatin1Char(':')) && emojiIdentifier.endsWith(QLatin1Char(':'))) {
        for (int i = 0, total = mCustomEmojiList.size(); i < total; ++i) {
            if (mCustomEmojiList.at(i).hasEmoji(emojiIdentifier)) {
                QString cachedHtml = mCustomEmojiList.at(i).cachedHtml();
                if (cachedHtml.isEmpty()) {
                    Emoji emoji = mCustomEmojiList[i];
                    //For the moment we can't support animated image as emoticon in text. Only as Reaction.
                    if (emoji.isAnimatedImage() && isReaction) {
                        cachedHtml = emoji.generateAnimatedUrlFromCustomEmoji(mServerUrl);
                    } else {
                        cachedHtml = emoji.generateHtmlFromCustomEmoji(mServerUrl);
                    }
                    mCustomEmojiList.replace(i, emoji);
                }
                return cachedHtml;
            }
        }
        QMap<QString, QVector<UnicodeEmoticon> >::const_iterator emojiId = mUnicodeEmojiList.constBegin();
        while (emojiId != mUnicodeEmojiList.constEnd()) {
            const QVector<UnicodeEmoticon> lst = emojiId.value();
            for (int i = 0, total = lst.size(); i < total; ++i) {
                if (lst.at(i).hasEmoji(emojiIdentifier)) {
                    return lst.at(i).unicodeDisplay();
                }
            }
            ++emojiId;
        }
    } else {
        qCWarning(RUQOLA_LOG) << "Emoji identifier is not correct :" << emojiIdentifier;
    }
    return emojiIdentifier;
}

QString EmojiManager::serverUrl() const
{
    return mServerUrl;
}

void EmojiManager::setServerUrl(const QString &serverUrl)
{
    if (mServerUrl != serverUrl) {
        mServerUrl = serverUrl;
        clearCustomEmojiCachedHtml();
    }
}

void EmojiManager::clearCustomEmojiCachedHtml()
{
    for (int i = 0, total = mCustomEmojiList.size(); i < total; ++i) {
        const QString &cachedHtml = mCustomEmojiList.at(i).cachedHtml();
        if (!cachedHtml.isEmpty()) {
            Emoji emoji = mCustomEmojiList[i];
            emoji.clearCachedHtml();
            mCustomEmojiList.replace(i, emoji);
        }
    }
}
