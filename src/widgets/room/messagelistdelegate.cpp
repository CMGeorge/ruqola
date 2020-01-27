/*
   Copyright (c) 2020 David Faure <faure@kde.org>

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

#include "messagelistdelegate.h"
#include "messagedelegatehelperbase.h"
#include "messagedelegatehelpertext.h"
#include "messagedelegatehelperimage.h"
#include "model/messagemodel.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "rocketchataccount.h"
#include "emoticons/emojimanager.h"
#include "emoticons/unicodeemoticon.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPixmapCache>

MessageListDelegate::MessageListDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mEmojiFont(QStringLiteral("NotoColorEmoji"))
    , mHelperText(new MessageDelegateHelperText)
    , mHelperImage(new MessageDelegateHelperImage)
{
}

MessageListDelegate::~MessageListDelegate()
{
    delete mHelperText;
    delete mHelperImage;
}

void MessageListDelegate::setRocketChatAccount(RocketChatAccount *rcAccount)
{
//    if (mRocketChatAccount) {
//        disconnect(mRocketChatAccount, 0, this, 0);
//    }
    mRocketChatAccount = rcAccount;
}

static qreal basicMargin()
{
    return 8;
}

static QString makeSenderText(const QModelIndex &index)
{
    return QLatin1Char('@') + index.data(MessageModel::Username).toString();
}

static QString makeTimeStampText(const QModelIndex &index)
{
    return index.data(MessageModel::Timestamp).toString();
}

static QSize timeStampSize(const QString &timeStampText, const QStyleOptionViewItem &option)
{
    // This gives incorrect results (too small bounding rect), no idea why!
    //const QSize timeSize = painter->fontMetrics().boundingRect(timeStampText).size();
    return QSize(option.fontMetrics.horizontalAdvance(timeStampText), option.fontMetrics.height());
}

static void drawTimestamp(QPainter *painter, const QModelIndex &index, const QStyleOptionViewItem &option, QRect *timeRect)
{
    const qreal margin = basicMargin();

    const QString timeStampText = makeTimeStampText(index);
    const QSize timeSize = timeStampSize(timeStampText, option);

    *timeRect = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignRight | Qt::AlignVCenter, timeSize, option.rect.adjusted(0, 0, -margin/2, 0));
    const QPen oldPen = painter->pen();
    QColor col = painter->pen().color();
    col.setAlpha(128); // TimestampText.qml had opacity: .5
    painter->setPen(col);
    painter->drawText(*timeRect, timeStampText);
    painter->setPen(oldPen);
}

QPixmap MessageListDelegate::makeAvatarPixmap(const QModelIndex &index, int maxHeight) const
{
    const QString userId = index.data(MessageModel::UserId).toString();
    const QString iconUrlStr = mRocketChatAccount->avatarUrl(userId);
    QPixmap pix;
    if (!iconUrlStr.isEmpty() && !QPixmapCache::find(iconUrlStr, &pix)) {
        const QUrl iconUrl(iconUrlStr);
        Q_ASSERT(iconUrl.isLocalFile());
        if (pix.load(iconUrl.toLocalFile())) {
            pix = pix.scaledToHeight(maxHeight);
            QPixmapCache::insert(iconUrlStr, pix);
        } else {
            qCWarning(RUQOLAWIDGETS_LOG) << "Could not load" << iconUrl.toLocalFile();
        }
    }
    return pix;
}

// [margin] <pixmap> [margin] <sender>
MessageListDelegate::PixmapAndSenderLayout MessageListDelegate::layoutPixmapAndSender(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    PixmapAndSenderLayout layout;
    layout.senderText = makeSenderText(index);
    layout.senderFont = option.font;
    layout.senderFont.setBold(true);
    const QFontMetricsF senderFontMetrics(layout.senderFont);
    const QSizeF senderTextSize = senderFontMetrics.boundingRect(layout.senderText).size();

    layout.avatarPixmap = makeAvatarPixmap(index, senderTextSize.height());
    layout.ascent = senderFontMetrics.ascent();

    const qreal margin = basicMargin();
    layout.senderRect = QRectF(option.rect.x() + layout.avatarPixmap.width() + 2 * margin, option.rect.y(),
                               senderTextSize.width(), senderTextSize.height());
    layout.avatarX = option.rect.x() + margin;
    return layout;
}

MessageDelegateHelperBase *MessageListDelegate::helper(const Message *message) const
{
    switch (message->messageType()) {
    case Message::Image:
        return mHelperImage;
    case Message::NormalText:
    default: // #### for now
        return mHelperText;
    }
    Q_UNREACHABLE();
    return nullptr;
}

QVector<MessageListDelegate::ReactionLayout> MessageListDelegate::layoutReactions(const QVector<Reaction> &reactions, const qreal messageX, const QStyleOptionViewItem &option) const
{
    QVector<MessageListDelegate::ReactionLayout> layouts;
    layouts.reserve(reactions.count());
    auto *emojiManager = mRocketChatAccount->emojiManager();
    QFontMetricsF emojiFontMetrics(mEmojiFont);
    const qreal margin = basicMargin();
    const qreal smallMargin = margin/2.0;
    const qreal height = qMax<qreal>(emojiFontMetrics.height(), option.fontMetrics.height()) + margin - 1;
    qreal x = messageX + margin;

    for (const Reaction &reaction : reactions) {
        ReactionLayout layout;
        layout.emojiString = emojiManager->unicodeEmoticonForEmoji(reaction.reactionName()).unicode();
        qreal emojiWidth;
        if (layout.emojiString.isEmpty()) {
            layout.emojiString = reaction.reactionName(); // ugly fallback: ":1md"
            emojiWidth = option.fontMetrics.horizontalAdvance(layout.emojiString) + smallMargin;
            layout.useEmojiFont = false;
        } else {
            emojiWidth = emojiFontMetrics.horizontalAdvance(layout.emojiString);
            layout.useEmojiFont = true;
        }
        layout.countStr = QString::number(reaction.count());
        const int countWidth = option.fontMetrics.horizontalAdvance(layout.countStr) + smallMargin;
        layout.reactionRect = QRectF(x, option.rect.bottom() - height, emojiWidth + countWidth + margin, height);
        layout.emojiOffset = margin / 2 + 1;
        layout.countRect = layout.reactionRect.adjusted(layout.emojiOffset + emojiWidth, smallMargin, 0, 0);
        layout.reaction = reaction;

        layouts.append(layout);
        x += layout.reactionRect.width() + margin;
    }
    return layouts;
}

void MessageListDelegate::drawReactions(QPainter *painter, const QModelIndex &index, const QRect &messageRect, const QStyleOptionViewItem &option) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    const QVector<Reaction> reactions = message->reactions().reactions();
    if (reactions.isEmpty()) {
        return;
    }

    const QVector<ReactionLayout> layout = layoutReactions(reactions, messageRect.x(), option);

    const QPen origPen = painter->pen();
    const QBrush origBrush = painter->brush();
    const QPen buttonPen(option.palette.color(QPalette::Highlight).darker());
    QColor backgroundColor = option.palette.color(QPalette::Highlight);
    backgroundColor.setAlpha(60);
    const QBrush buttonBrush(backgroundColor);
    const qreal smallMargin = basicMargin()/2.0;
    for (const ReactionLayout &reactionLayout : layout) {
        if (!reactionLayout.emojiString.isEmpty()) {
            const QRectF reactionRect = reactionLayout.reactionRect;

            // Rounded rect
            painter->setPen(buttonPen);
            painter->setBrush(buttonBrush);
            painter->drawRoundedRect(reactionRect, 5, 5);
            painter->setBrush(origBrush);
            painter->setPen(origPen);

            // Emoji
            if (reactionLayout.useEmojiFont) {
                painter->setFont(mEmojiFont);
            }
            painter->drawText(reactionRect.adjusted(reactionLayout.emojiOffset, smallMargin, 0, 0), reactionLayout.emojiString);

            // Count
            painter->setFont(option.font);
            painter->drawText(reactionLayout.countRect, reactionLayout.countStr);
        }
    }
}

void MessageListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    drawBackground(painter, option, index);

    const QPen origPen = painter->pen();

    // Compact mode : <pixmap> <sender> <message> <smiley> <timestamp>

    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    // Sender and pixmap (calculate size, but don't draw it yet, we need to align vertically to the first line of the message)
    const PixmapAndSenderLayout leftLayout = layoutPixmapAndSender(option, index);

    // Timestamp
    QRect timeRect;
    drawTimestamp(painter, index, option, &timeRect);

    // Message
    QRect messageRect = option.rect;
    messageRect.setLeft(leftLayout.senderRect.right());
    messageRect.setRight(timeRect.left() - 1);
    qreal baseLine = option.rect.y() + option.fontMetrics.ascent(); // default value, modified by HelperText

    helper(message)->draw(painter, messageRect, index, option, &baseLine);

    // Now draw the pixmap
    painter->drawPixmap(leftLayout.avatarX, baseLine - leftLayout.ascent, leftLayout.avatarPixmap);
    // If we need support for drawing as selected, we might want to do this:
    //QRect decorationRect(option.rect.x(), topOfFirstLine, avatarPixmap.width(), avatarPixmap.height());
    //drawDecoration(painter, option, decorationRect, avatarPixmap);

    // Now draw the sender
    painter->setFont(leftLayout.senderFont);
    painter->drawText(leftLayout.senderRect.x(), baseLine, leftLayout.senderText);

    // Reactions
    drawReactions(painter, index, messageRect, option);

    painter->setFont(option.font);
    painter->setPen(origPen);

    //drawFocus(painter, option, displayRect);
}

QSize MessageListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const qreal margin = basicMargin();
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();

    // Avatar pixmap and sender text
    const PixmapAndSenderLayout leftLayout = layoutPixmapAndSender(option, index);

    // Timestamp
    const QString timeStampText = makeTimeStampText(index);
    const QSize timeSize = timeStampSize(timeStampText, option);

    // Message (using the rest of the available width)
    const int widthBeforeMessage = leftLayout.senderRect.right();
    const int widthAfterMessage = timeSize.width() + margin / 2;
    const int maxWidth = qMax(30, option.rect.width() - widthBeforeMessage - widthAfterMessage);
    const QSize size = helper(message)->sizeHint(index, maxWidth, option);

    int additionalHeight = 0;
    if (!message->reactions().isEmpty()) {
        QFontMetricsF emojiFontMetrics(mEmojiFont);
        additionalHeight += emojiFontMetrics.height() + margin;
    }

    // hopefully the width below is never more than option.rect.width() or we'll get a scrollbar
    return QSize(widthBeforeMessage + size.width() + widthAfterMessage,
                 qMax<int>(leftLayout.senderRect.height(), size.height()) + additionalHeight);
}

bool MessageListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mev = static_cast<QMouseEvent *>(event);
        const QPoint pos = mev->pos();
        const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
        const PixmapAndSenderLayout leftLayout = layoutPixmapAndSender(option, index);
        if (!message->reactions().isEmpty()) {
            const QVector<ReactionLayout> layout = layoutReactions(message->reactions().reactions(), leftLayout.senderRect.right(), option);
            for (const ReactionLayout &reactionLayout : layout) {
                if (reactionLayout.reactionRect.contains(pos)) {
                    const Reaction &reaction = reactionLayout.reaction;
                    const bool doAdd = !reaction.userNames().contains(mRocketChatAccount->userName());
                    mRocketChatAccount->reactOnMessage(index.data(MessageModel::MessageId).toString(), reaction.reactionName(), doAdd);
                    return true;
                }
            }
        }
        if (helper(message)->handleMouseEvent(mev, leftLayout.senderRect, option, index)) {
            return true;
        }
    }
    return QItemDelegate::editorEvent(event, model, option, index);
}
