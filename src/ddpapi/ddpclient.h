/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef DDPCLIENT_H
#define DDPCLIENT_H

#include "rocketchatmessage.h"
#include "libruqolacore_export.h"
#include <functional>
#include <QQueue>

class QJsonObject;
class QJsonDocument;
class RocketChatMessage;
class AbstractWebSocket;
class RocketChatAccount;
class LIBRUQOLACORE_EXPORT DDPClient : public QObject
{
    Q_OBJECT
public:
    enum LoginStatus {
        NotConnected,
        LoggingIn,
        LoggedIn,
        LoginFailed,
        LoggedOut
    };
    Q_ENUM(LoginStatus)

    enum MessageType {
        Persistent,
        Ephemeral
    };

    explicit DDPClient(RocketChatAccount *account = nullptr, QObject *parent = nullptr);
    ~DDPClient();

    /**
    * @brief Call a method with name @param method and parameters @param params and @param messageType with an empty callback
    *
    * @param method The name of the method to call Rocket.Chat API for
    * @param params The parameters
    * @param messageType The type of message
    * @return unsigned int, the ID of the called method
    */
    quint64 method(const QString &method, const QJsonDocument &params, DDPClient::MessageType messageType = DDPClient::Ephemeral);

    /**
    * @brief Send message over network
    *
    * @param method The name of the method to call Rocket.Chat API for
    * @param params The parameters
    * @param callback The pointer to callback function
    * @param messageType The type of message
    * @return unsigned int, the ID of the called method
    */
    quint64 method(const QString &method, const QJsonDocument &params, std::function<void(QJsonObject, RocketChatAccount *)> callback, DDPClient::MessageType messageType = DDPClient::Ephemeral);

    quint64 method(const RocketChatMessage::RocketChatMessageResult &result, std::function<void(QJsonObject, RocketChatAccount *)> callback, DDPClient::MessageType messageType = DDPClient::Ephemeral);

    /**
    * @brief Subscribes to a collection with name @param collection and parameters @param params
    *
    * @param collection The name of the collection
    * @param params The parameters
    */
    void subscribe(const QString &collection, const QJsonArray &params);

    /**
    * @brief Calls method to log in the user with valid username and password
    */
    Q_INVOKABLE void login();

    /**
    * @brief Closes the websocket connection
    */
    void logOut();

    /**
    * @brief Check whether websocket is connected at url
    *
    * @return true if connected, else false
    */
    bool isConnected() const;

    /**
    * @brief Check whether user is logged in
    *
    * @return true if user is logged in, else false
    */
    bool isLoggedIn() const;

    /**
    * @brief Reconnects the websocket to new url
    */
    void onServerURLChange();

    /**
    * @brief Returns the queue used to cache unsent messages
    *
    *@return QQueue<QPair<QString,QJsonDocument>>, The m_messageQueue object
    */
    QQueue<QPair<QString, QJsonDocument> > messageQueue() const;

    /**
    * @brief Returns standard cache path
    *
    *@def QString path
    */
    QString cachePath() const;

    quint64 leaveRoom(const QString &roomID);
    quint64 hideRoom(const QString &roomID);
    quint64 clearUnreadMessages(const QString &roomID);

    quint64 informTypingStatus(const QString &room, bool typing, const QString &userName);

    void setServerUrl(const QString &url);
    void start();
    void setLoginJobId(quint64 jobid);

    LoginStatus loginStatus() const;

    quint64 toggleFavorite(const QString &roomID, bool favorite);
    quint64 createChannel(const QString &name, const QStringList &userList, bool readOnly);
    quint64 createPrivateGroup(const QString &name, const QStringList &userList);
    quint64 joinRoom(const QString &roomId, const QString &joinCode);
    quint64 openDirectChannel(const QString &userId);
    void subscribeRoomMessage(const QString &roomId);
    quint64 setDefaultStatus(User::PresenceStatus status);
    quint64 listEmojiCustom();
    quint64 createJitsiConfCall(const QString &roomId);
    quint64 userAutocomplete(const QString &pattern, const QString &exception);
    quint64 deleteMessage(const QString &messageId);
    quint64 eraseRoom(const QString &roomID);
    quint64 setRoomName(const QString &roomId, const QString &name);
    quint64 setRoomTopic(const QString &roomId, const QString &topic);
    quint64 setRoomDescription(const QString &roomId, const QString &description);
    quint64 setRoomAnnouncement(const QString &roomId, const QString &announcement);
    quint64 starMessage(const QString &messageId, const QString &rid, bool starred);
    quint64 setRoomIsReadOnly(const QString &roomId, bool readOnly);
    quint64 archiveRoom(const QString &roomId);
    quint64 setReaction(const QString &emoji, const QString &messageId);
    quint64 getUsersOfRoom(const QString &roomId, bool showAll);
    quint64 loadHistory(const QJsonArray &params);
    quint64 channelAndPrivateAutocomplete(const QString &pattern, const QString &exception);
    quint64 roomFiles(const QString &roomId);
    quint64 addUserToRoom(const QString &userId, const QString &roomId);
    quint64 inputChannelAutocomplete(const QString &pattern, const QString &exceptions);
    quint64 inputUserAutocomplete(const QString &pattern, const QString &exceptions);
    quint64 login(const QString &username, const QString &password);
    quint64 loginProvider(const QString &credentialToken, const QString &credentialSecret);
    quint64 messageSearch(const QString &rid, const QString &pattern);
    quint64 unBlockUser(const QString &rid, const QString &userId);
    quint64 blockUser(const QString &rid, const QString &userId);
    quint64 disableNotifications(const QString &roomId, bool disabled);
    quint64 hideUnreadStatus(const QString &roomId, bool disabled);
Q_SIGNALS:
    void connectedChanged();
    void loginStatusChanged();
    void disconnected();
    void added(const QJsonObject &item);
    void changed(const QJsonObject &item);
    void removed(const QJsonObject &item);

    /**
     * @brief Emitted whenever a result is received
     *
     * @param id The ID received in the method() call
     * @param result The response sent by server
     */
    void result(quint64 id, const QJsonDocument &result);

private slots:
    void onWSConnected();
    void onTextMessageReceived(const QString &message);
    void onWSclosed();

private:
    Q_DISABLE_COPY(DDPClient)
    void initializeWebSocket();

    QUrl adaptUrl(const QString &url);

    void setLoginStatus(LoginStatus l);

    void pong();
    void executeSubsCallBack(const QJsonObject &root);

    QString mUrl;
    AbstractWebSocket *mWebSocket = nullptr;

    /**
     * @brief Unique message ID for each message sent over network
     */
    quint64 m_uid = 0;

    /**
     * @brief Stores callback function associated with each message
     *
     * @def QHash unsigned messageID and std::function<void (QJsonDocument)> pointer to callback
     */
    QHash <quint64, std::function<void(QJsonObject, RocketChatAccount *)> > m_callbackHash;

    quint64 m_loginJob = 0;
    LoginStatus m_loginStatus;

    bool m_connected = false;

    bool m_attemptedPasswordLogin = false;
    bool m_attemptedTokenLogin = false;

    /**
    * @brief Abstract queue for all requests regarding network management
    *
    * @def QPair QString method and QJsonDocument params
    */
    QQueue<QPair<QString, QJsonDocument> > m_messageQueue;

    friend class Ruqola;
    RocketChatMessage *mRocketChatMessage = nullptr;
    RocketChatAccount *mRocketChatAccount = nullptr;
};

#endif // DDPCLIENT_H
