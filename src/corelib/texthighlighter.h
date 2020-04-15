/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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
#ifndef TEXTHIGHLIGHTER_H
#define TEXTHIGHLIGHTER_H

//#ifdef Q_OS_WINDOWS
#include <QObject>
//#else
//#include <KSyntaxHighlighting/AbstractHighlighter>
//#endif

class QTextStream;

//#ifdef Q_OS_WINDOWS
//class TextHighlighter : public QObject
//#else
class TextHighlighter : public QObject//public KSyntaxHighlighting::AbstractHighlighter
//#endif
{
public:
    explicit TextHighlighter(QTextStream *stream);
    ~TextHighlighter() override = default;

    void highlight(const QString &str);

#if !defined(Q_OS_WINDOWS) && !defined(Q_OS_MAC) && !defined(Q_OS_ANDROID)
protected:
    void applyFormat(int offset, int length, const KSyntaxHighlighting::Format &format) override;
#endif

private:
    QString mCurrentLine;
    QTextStream *mStream;
};

#endif // TEXTHIGHLIGHTER_H
