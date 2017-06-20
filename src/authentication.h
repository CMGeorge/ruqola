#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H


#include <QtNetworkAuth>
#include <QOAuth2AuthorizationCodeFlow>

class Authentication : public QObject
{
    Q_OBJECT
public:
    Authentication();
    void getDataFromJson();
    void OAuthLogin();
    void sendApiRequest();

private slots:
    void onGranted();


private:
    bool m_authGranted;
    QString m_client_id;
    QString m_client_secret;
    QOAuth2AuthorizationCodeFlow * m_google;

};




#endif // AUTHENTICATION_H
