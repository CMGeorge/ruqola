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

#include "inputtextmanager.h"
#include "ruqola_debug.h"
#include "model/inputcompletermodel.h"

InputTextManager::InputTextManager(QObject *parent)
    : QObject(parent)
{
    mInputCompleterModel = new InputCompleterModel(this);
}

InputTextManager::~InputTextManager()
{
}

QString InputTextManager::replaceWord(const QString &newWord, const QString &text, int position)
{
    if (newWord.isEmpty()) {
        qCDebug(RUQOLA_LOG) << "InputTextManager::replaceWord Empty newWord";
        return text;
    }
    if (text.isEmpty()) {
        qCDebug(RUQOLA_LOG) << "InputTextManager::replaceWord Empty text";
        return text;
    }
    //Cursor position can be at the end of word => text.length
    if ((position > text.length()) || (position < 0)) {
        qCDebug(RUQOLA_LOG) << "InputTextManager::replaceWord Invalid position" << position;
        return text;
    }

    int start = 0;
    for (int i = position - 1; i >= 0; i--) {
        if (i == 0) {
            start = 1;
            break;
        }
        if (!text.at(i).isSpace()) {
            continue;
        }
        //Don't replace # or @
        start = i + 2;
        break;
    }
    int end = text.length() - 1;
    for (int i = position; i < text.length(); i++) {
        if (!text.at(i).isSpace()) {
            continue;
        }
        end = i - 1;
        break;
    }
    QString replaceText = text;
    replaceText.replace(start, end - start + 1, newWord);
    return replaceText;
}

QString InputTextManager::searchWord(const QString &text, int position)
{
    if (text.isEmpty()) {
        return {};
    }
    int start = 0;
    for (int i = position; i > 0; i--) {
        if (!text.at(i-1).isSpace()) {
            continue;
        }
        start = i;
        break;
    }
    int end = text.length() - 1;
    for (int i = position; i < text.length(); i++) {
        if (!text.at(i).isSpace()) {
            continue;
        }
        end = i;
        break;
    }

    QString word = text.mid(start, end - start + 1);
    if (!word.isEmpty() && word.at(word.length() -1).isSpace()) {
        word.chop(1);
    }
    //qDebug() << "position" << position << " word " << word << " text " << text << " start " << start << " end " << end;
    return word;
}

void InputTextManager::setInputTextChanged(const QString &text, int position)
{
    if (text.isEmpty()) {
        clearCompleter();
        return;
    }
    const QString word = searchWord(text, position);
    const QString str = word.right(word.length()-1);
    //qDebug() << " str :" << str << ": word :" << word << ":";
    if (word.isEmpty()) {
        clearCompleter();
    } else {
        if (word.startsWith(QLatin1Char('@'))) {
            //FIXME word without @ ? and exception!
            Q_EMIT inputCompleter(str, QString(), InputTextManager::CompletionForType::User);
        } else if (word.startsWith(QLatin1Char('#'))) {
            //FIXME word without @ ? and exception!
            Q_EMIT inputCompleter(str, QString(), InputTextManager::CompletionForType::Channel);
        } else {
            clearCompleter();
        }
    }
}

void InputTextManager::clearCompleter()
{
    mInputCompleterModel->clear();
}

InputCompleterModel *InputTextManager::inputCompleterModel() const
{
    return mInputCompleterModel;
}

void InputTextManager::inputTextCompleter(const QJsonObject &obj)
{
    mInputCompleterModel->parseChannels(obj);
}
