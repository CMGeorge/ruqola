TEMPLATE = lib
#CONFIG += plugin
QT+=network websockets quick
DEFINES += IS_LIBRUQOLACORE_EXPORT
#DEFINES += LIBROCKETCHATRESTAPI_QT5_EXPORT
DISTFILES += \
    emoticons/emoji.json \
    icons/systray.png \
    settings/ruqolaglobalconfig.kcfg \
    settings/ruqolaglobalconfig.kcfgc



#INCLUDEPATH += $$OUT_PWD/../../include/rocketchatrestapi-qt5/
#LIBS+=-L$$OUT_PWD/../../lib/ -lrocketchatrestapi-qt5
#error($$INCLUDEPATH)
include($$PWD/../rocketchatrestapi-qt5/rocketchatrestapi-qt5.pri)

#INCLUDEPATH += $$PWD/../plugins/
#INCLUDEPATH += $$PWD/KDE/

HEADERS += \
    KDE/KPluginFactory_p.h \
    KDE/desktopfileparser_p.h \
    KDE/kaboutdata.h \
    KDE/kcomponentdata.h \
    KDE/kcomponentdata_p.h \
    KDE/kcoreaddons_debug.h \
    KDE/kcoreaddons_export.h \
    KDE/kexportplugin.h \
    KDE/kpluginfactory.h \
    KDE/kpluginloader.h \
    KDE/kpluginmetadata.h \
    abstractwebsocket.h \
    accountmanager.h \
    accountroomsettings.h \
    authenticationinfo.h \
    authenticationmanager.h \
    autotranslatelanguage.h \
    autotranslatelanguages.h \
    avatarmanager.h \
    changetemporarystatus.h \
    channel.h \
    clipboardproxy.h \
    command.h \
    commands.h \
    convertertextjob/convertertextabstractjob.h \
    convertertextjob/translatetextjob.h \
    ddpapi/ddpclient.h \
    ddpapi/ruqola_ddpapi_command_debug.h \
    ddpapi/ruqola_ddpapi_debug.h \
    discussion.h \
    discussions.h \
    emoticoncategory.h \
    emoticons/emoji.h \
    emoticons/emojimanager.h \
    emoticons/unicodeemoticon.h \
    emoticons/unicodeemoticonparser.h \
    file.h \
    fileattachments.h \
    inputtextmanager.h \
    libruqola_private_export.h \
    libruqolacore_export.h \
    listmessages.h \
    loadrecenthistorymanager.h \
    lrucache.h \
    managerdatapaths.h \
#    messagecache.h \
    messagedownloadmanager.h \
    messagequeue.h \
    messages/message.h \
    messages/messageattachment.h \
    messages/messagepinned.h \
    messages/messagestarred.h \
    messages/messagetranslation.h \
    messages/messageurl.h \
    messages/reaction.h \
    messages/reactions.h \
    model/accountschannelsmodel.h \
    model/autotranslatelanguagesmodel.h \
    model/commandsmodel.h \
    model/discussionsfilterproxymodel.h \
    model/discussionsmodel.h \
    model/emoticoncategoriesmodel.h \
    model/emoticonfiltermodel.h \
    model/emoticonmodel.h \
    model/filesforroomfilterproxymodel.h \
    model/filesforroommodel.h \
    model/inputcompletermodel.h \
    model/listmessagesmodel.h \
    model/listmessagesmodelfilterproxymodel.h \
    model/loginmethodmodel.h \
    model/messagemodel.h \
    model/notificationdesktopdurationpreferencemodel.h \
    model/notificationdesktopsoundpreferencemodel.h \
    model/notificationpreferencemodel.h \
    model/rocketchataccountfilterproxymodel.h \
    model/rocketchataccountmodel.h \
    model/roomfilterproxymodel.h \
    model/roommodel.h \
    model/searchchannelfilterproxymodel.h \
    model/searchchannelmodel.h \
    model/searchmessagefilterproxymodel.h \
    model/searchmessagemodel.h \
    model/statusmodel.h \
    model/threadmessagemodel.h \
    model/usercompleterfilterproxymodel.h \
    model/usercompletermodel.h \
    model/usersforroomfilterproxymodel.h \
    model/usersforroommodel.h \
    model/usersmodel.h \
#    notification.h \
    notificationoptions.h \
    notificationoptionswrapper.h \
    notificationpreferences.h \
    otr.h \
    otrmanager.h \
    plugins/pluginauthentication.h \
    plugins/pluginauthenticationinterface.h \
    receivetypingnotificationmanager.h \
    rocketchataccount.h \
    rocketchataccountsettings.h \
    rocketchatbackend.h \
    rocketchatcache.h \
    rocketchatmessage.h \
    role.h \
    roles.h \
    room.h \
    roomwrapper.h \
    ruqola.h \
    ruqola_debug.h \
    ruqola_message_debug.h \
    ruqola_unknown_collectiontype_debug.h \
    ruqolalogger.h \
    ruqolaregisterengine.h \
    ruqolaserverconfig.h \
    ruqolautils.h \
    ruqolawebsocket.h \
    serverconfiginfo.h \
    syntaxhighlightingmanager.h \
#    textconverter.h \
#    texthighlighter.h \
    typingnotification.h \
#    unityservicemanager.h \
    user.h \
    utils.h

SOURCES += \
    KDE/desktopfileparser.cpp \
    KDE/kaboutdata.cpp \
    KDE/kcomponentdata.cpp \
    KDE/kpluginfactory.cpp \
    KDE/kpluginloader.cpp \
    KDE/kpluginmetadata.cpp \
    abstractwebsocket.cpp \
    accountmanager.cpp \
    accountroomsettings.cpp \
    authenticationinfo.cpp \
    authenticationmanager.cpp \
    autotranslatelanguage.cpp \
    autotranslatelanguages.cpp \
    avatarmanager.cpp \
    changetemporarystatus.cpp \
    channel.cpp \
    clipboardproxy.cpp \
    command.cpp \
    commands.cpp \
    convertertextjob/convertertextabstractjob.cpp \
    convertertextjob/translatetextjob.cpp \
    ddpapi/ddpclient.cpp \
    discussion.cpp \
    discussions.cpp \
    emoticoncategory.cpp \
    emoticons/emoji.cpp \
    emoticons/emojimanager.cpp \
    emoticons/unicodeemoticon.cpp \
    emoticons/unicodeemoticonparser.cpp \
    file.cpp \
    fileattachments.cpp \
    inputtextmanager.cpp \
    listmessages.cpp \
    loadrecenthistorymanager.cpp \
    managerdatapaths.cpp \
#    messagecache.cpp \
    messagedownloadmanager.cpp \
    messagequeue.cpp \
    messages/message.cpp \
    messages/messageattachment.cpp \
    messages/messagepinned.cpp \
    messages/messagestarred.cpp \
    messages/messagetranslation.cpp \
    messages/messageurl.cpp \
    messages/reaction.cpp \
    messages/reactions.cpp \
    model/accountschannelsmodel.cpp \
    model/autotranslatelanguagesmodel.cpp \
    model/commandsmodel.cpp \
    model/discussionsfilterproxymodel.cpp \
    model/discussionsmodel.cpp \
    model/emoticoncategoriesmodel.cpp \
    model/emoticonfiltermodel.cpp \
    model/emoticonmodel.cpp \
    model/filesforroomfilterproxymodel.cpp \
    model/filesforroommodel.cpp \
    model/inputcompletermodel.cpp \
    model/listmessagesmodel.cpp \
    model/listmessagesmodelfilterproxymodel.cpp \
    model/loginmethodmodel.cpp \
    model/messagemodel.cpp \
    model/notificationdesktopdurationpreferencemodel.cpp \
    model/notificationdesktopsoundpreferencemodel.cpp \
    model/notificationpreferencemodel.cpp \
    model/rocketchataccountfilterproxymodel.cpp \
    model/rocketchataccountmodel.cpp \
    model/roomfilterproxymodel.cpp \
    model/roommodel.cpp \
    model/searchchannelfilterproxymodel.cpp \
    model/searchchannelmodel.cpp \
    model/searchmessagefilterproxymodel.cpp \
    model/searchmessagemodel.cpp \
    model/statusmodel.cpp \
    model/threadmessagemodel.cpp \
    model/usercompleterfilterproxymodel.cpp \
    model/usercompletermodel.cpp \
    model/usersforroomfilterproxymodel.cpp \
    model/usersforroommodel.cpp \
    model/usersmodel.cpp \
#    notification.cpp \
    notificationoptions.cpp \
    notificationoptionswrapper.cpp \
    notificationpreferences.cpp \
    otr.cpp \
    otrmanager.cpp \
    plugins/pluginauthentication.cpp \
    plugins/pluginauthenticationinterface.cpp \
    receivetypingnotificationmanager.cpp \
    rocketchataccount.cpp \
    rocketchataccountsettings.cpp \
    rocketchatbackend.cpp \
    rocketchatcache.cpp \
    rocketchatmessage.cpp \
    role.cpp \
    roles.cpp \
    room.cpp \
    roomwrapper.cpp \
    ruqola.cpp \
    ruqolalogger.cpp \
    ruqolaregisterengine.cpp \
    ruqolaserverconfig.cpp \
    ruqolautils.cpp \
    ruqolawebsocket.cpp \
    serverconfiginfo.cpp \
    syntaxhighlightingmanager.cpp \
#    textconverter.cpp \
#    texthighlighter.cpp \
    typingnotification.cpp \
#    unityservicemanager.cpp \
    user.cpp \
    utils.cpp

RESOURCES += \
    ruqolacore.qrc


CMAKE_MODULE_TESTS = '-'
load(qt_build_config)
VERSION = 1.0.0
load(qt_module)
