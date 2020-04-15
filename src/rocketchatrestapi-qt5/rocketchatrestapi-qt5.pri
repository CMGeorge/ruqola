QT += network websockets
INCLUDEPATH += $$PWD \
                $$PWD/channels \
                $$PWD/authentication \
                $$PWD/chat \
                $$PWD/groups \
                $$PWD/rooms \
                $$PWD/commands \
                $$PWD/users

SOURCES += \
    $$PWD/abstractlogger.cpp \
    $$PWD/authentication/facebookauthjob.cpp \
    $$PWD/authentication/googleauthjob.cpp \
    $$PWD/authentication/loginjob.cpp \
    $$PWD/authentication/logoutjob.cpp \
    $$PWD/authentication/twitterauthjob.cpp \
    $$PWD/autotranslate/getsupportedlanguagesjob.cpp \
    $$PWD/autotranslate/translatemessagejob.cpp \
    $$PWD/autotranslate/translatesavesettingsjob.cpp \
    $$PWD/channellistjob.cpp \
    $$PWD/channels/archivechanneljob.cpp \
    $$PWD/channels/changechannelannouncementjob.cpp \
    $$PWD/channels/changechanneldescriptionjob.cpp \
    $$PWD/channels/changechannelencryptedjob.cpp \
    $$PWD/channels/changechannelnamejob.cpp \
    $$PWD/channels/changechannelreadonlyjob.cpp \
    $$PWD/channels/changechanneltopicjob.cpp \
    $$PWD/channels/channeladdleaderjob.cpp \
    $$PWD/channels/channeladdmoderatorjob.cpp \
    $$PWD/channels/channeladdownerjob.cpp \
    $$PWD/channels/channelbasejob.cpp \
    $$PWD/channels/channelclosejob.cpp \
    $$PWD/channels/channeldeletejob.cpp \
    $$PWD/channels/channelfilesjob.cpp \
    $$PWD/channels/channelgetallusermentionsjob.cpp \
    $$PWD/channels/channelhistoryjob.cpp \
    $$PWD/channels/channelinfojob.cpp \
    $$PWD/channels/channelinvitejob.cpp \
    $$PWD/channels/channeljoinjob.cpp \
    $$PWD/channels/channelkickjob.cpp \
    $$PWD/channels/channelmembersjob.cpp \
    $$PWD/channels/channelremoveleaderjob.cpp \
    $$PWD/channels/channelremovemoderatorjob.cpp \
    $$PWD/channels/channelremoveownerjob.cpp \
    $$PWD/channels/channelsmoderatorsjob.cpp \
    $$PWD/channels/createchanneljob.cpp \
    $$PWD/channels/getchannelrolesjob.cpp \
    $$PWD/channels/leavechanneljob.cpp \
    $$PWD/channels/setchanneltypejob.cpp \
    $$PWD/channels/setjoincodechanneljob.cpp \
    $$PWD/chat/deletemessagejob.cpp \
    $$PWD/chat/followmessagejob.cpp \
    $$PWD/chat/getmessagejob.cpp \
    $$PWD/chat/getpinnedmessagesjob.cpp \
    $$PWD/chat/getsnippetedmessagesjob.cpp \
    $$PWD/chat/getstarredmessagesjob.cpp \
    $$PWD/chat/getthreadmessagesjob.cpp \
    $$PWD/chat/getthreadsjob.cpp \
    $$PWD/chat/ignoreuserjob.cpp \
    $$PWD/chat/pinmessagejob.cpp \
    $$PWD/chat/postmessagejob.cpp \
    $$PWD/chat/reactonmessagejob.cpp \
    $$PWD/chat/reportmessagejob.cpp \
    $$PWD/chat/searchmessagejob.cpp \
    $$PWD/chat/sendmessagejob.cpp \
    $$PWD/chat/starmessagejob.cpp \
    $$PWD/chat/syncthreadmessagesjob.cpp \
    $$PWD/chat/unfollowmessagejob.cpp \
    $$PWD/chat/updatemessagejob.cpp \
    $$PWD/commands/getcommandsjob.cpp \
    $$PWD/commands/listcommandsjob.cpp \
    $$PWD/commands/runcommandjob.cpp \
    $$PWD/custom/customsoundsjob.cpp \
    $$PWD/custom/customuserstatusjob.cpp \
    $$PWD/directmessage/createdmjob.cpp \
    $$PWD/directmessage/opendmjob.cpp \
    $$PWD/directmessage/settopicdmjob.cpp \
    $$PWD/downloadfilejob.cpp \
    $$PWD/e2e/fetchmykeysjob.cpp \
    $$PWD/e2e/setuserpublicandprivatekeysjob.cpp \
    $$PWD/emoji/deleteemojicustomjob.cpp \
    $$PWD/emoji/loademojicustomjob.cpp \
    $$PWD/groups/archivegroupsjob.cpp \
    $$PWD/groups/changegroupsannouncementjob.cpp \
    $$PWD/groups/changegroupsdescriptionjob.cpp \
    $$PWD/groups/changegroupsencryptedjob.cpp \
    $$PWD/groups/changegroupsnamejob.cpp \
    $$PWD/groups/changegroupsreadonlyjob.cpp \
    $$PWD/groups/changegroupstopicjob.cpp \
    $$PWD/groups/creategroupsjob.cpp \
    $$PWD/groups/getgrouprolesjob.cpp \
    $$PWD/groups/groupaddleaderjob.cpp \
    $$PWD/groups/groupaddmoderatorjob.cpp \
    $$PWD/groups/groupaddownerjob.cpp \
    $$PWD/groups/groupremoveleaderjob.cpp \
    $$PWD/groups/groupremovemoderatorjob.cpp \
    $$PWD/groups/groupremoveownerjob.cpp \
    $$PWD/groups/groupsdeletejob.cpp \
    $$PWD/groups/groupsinfojob.cpp \
    $$PWD/groups/groupsinvitejob.cpp \
    $$PWD/groups/groupskickjob.cpp \
    $$PWD/groups/leavegroupsjob.cpp \
    $$PWD/groups/setgrouptypejob.cpp \
    $$PWD/invite/findorcreateinvitejob.cpp \
    $$PWD/misc/owninfojob.cpp \
    $$PWD/misc/settingsoauthjob.cpp \
    $$PWD/permissions/listpermissionsjob.cpp \
    $$PWD/restapiabstractjob.cpp \
    $$PWD/restapimethod.cpp \
    $$PWD/restapirequest.cpp \
    $$PWD/restapiutil.cpp \
    $$PWD/rooms/getdiscussionsjob.cpp \
    $$PWD/rooms/getroomsjob.cpp \
    $$PWD/rooms/roomfavoritejob.cpp \
    $$PWD/rooms/roomleavejob.cpp \
    $$PWD/rooms/roomsautocompletechannelandprivatejob.cpp \
    $$PWD/rooms/roomsinfojob.cpp \
    $$PWD/rooms/roomstartdiscussionjob.cpp \
    $$PWD/rooms/savenotificationjob.cpp \
    $$PWD/serverinfojob.cpp \
    $$PWD/settings/privateinfojob.cpp \
    $$PWD/spotlightjob.cpp \
    $$PWD/subscriptions/markroomasreadjob.cpp \
    $$PWD/subscriptions/markroomasunreadjob.cpp \
    $$PWD/uploadfilejob.cpp \
    $$PWD/users/forgotpasswordjob.cpp \
    $$PWD/users/getavatarjob.cpp \
    $$PWD/users/getpresencejob.cpp \
    $$PWD/users/getusernamesuggestionjob.cpp \
    $$PWD/users/resetavatarjob.cpp \
    $$PWD/users/setavatarjob.cpp \
    $$PWD/users/setstatusjob.cpp \
    $$PWD/users/userbasejob.cpp \
    $$PWD/users/userinfojob.cpp \
    $$PWD/users/usersautocompletejob.cpp \
    $$PWD/users/userspresencejob.cpp \
    $$PWD/video-conference/videoconfupdatejitsitimeoutjob.cpp

HEADERS += \
    $$PWD/abstractlogger.h \
    $$PWD/authentication/facebookauthjob.h \
    $$PWD/authentication/googleauthjob.h \
    $$PWD/authentication/loginjob.h \
    $$PWD/authentication/logoutjob.h \
    $$PWD/authentication/twitterauthjob.h \
    $$PWD/autotranslate/getsupportedlanguagesjob.h \
    $$PWD/autotranslate/translatemessagejob.h \
    $$PWD/autotranslate/translatesavesettingsjob.h \
    $$PWD/channellistjob.h \
    $$PWD/channels/archivechanneljob.h \
    $$PWD/channels/changechannelannouncementjob.h \
    $$PWD/channels/changechanneldescriptionjob.h \
    $$PWD/channels/changechannelencryptedjob.h \
    $$PWD/channels/changechannelnamejob.h \
    $$PWD/channels/changechannelreadonlyjob.h \
    $$PWD/channels/changechanneltopicjob.h \
    $$PWD/channels/channeladdleaderjob.h \
    $$PWD/channels/channeladdmoderatorjob.h \
    $$PWD/channels/channeladdownerjob.h \
    $$PWD/channels/channelbasejob.h \
    $$PWD/channels/channelclosejob.h \
    $$PWD/channels/channeldeletejob.h \
    $$PWD/channels/channelfilesjob.h \
    $$PWD/channels/channelgetallusermentionsjob.h \
    $$PWD/channels/channelhistoryjob.h \
    $$PWD/channels/channelinfojob.h \
    $$PWD/channels/channelinvitejob.h \
    $$PWD/channels/channeljoinjob.h \
    $$PWD/channels/channelkickjob.h \
    $$PWD/channels/channelmembersjob.h \
    $$PWD/channels/channelremoveleaderjob.h \
    $$PWD/channels/channelremovemoderatorjob.h \
    $$PWD/channels/channelremoveownerjob.h \
    $$PWD/channels/channelsmoderatorsjob.h \
    $$PWD/channels/createchanneljob.h \
    $$PWD/channels/getchannelrolesjob.h \
    $$PWD/channels/leavechanneljob.h \
    $$PWD/channels/setchanneltypejob.h \
    $$PWD/channels/setjoincodechanneljob.h \
    $$PWD/chat/deletemessagejob.h \
    $$PWD/chat/followmessagejob.h \
    $$PWD/chat/getmessagejob.h \
    $$PWD/chat/getpinnedmessagesjob.h \
    $$PWD/chat/getsnippetedmessagesjob.h \
    $$PWD/chat/getstarredmessagesjob.h \
    $$PWD/chat/getthreadmessagesjob.h \
    $$PWD/chat/getthreadsjob.h \
    $$PWD/chat/ignoreuserjob.h \
    $$PWD/chat/pinmessagejob.h \
    $$PWD/chat/postmessagejob.h \
    $$PWD/chat/reactonmessagejob.h \
    $$PWD/chat/reportmessagejob.h \
    $$PWD/chat/searchmessagejob.h \
    $$PWD/chat/sendmessagejob.h \
    $$PWD/chat/starmessagejob.h \
    $$PWD/chat/syncthreadmessagesjob.h \
    $$PWD/chat/unfollowmessagejob.h \
    $$PWD/chat/updatemessagejob.h \
    $$PWD/commands/getcommandsjob.h \
    $$PWD/commands/listcommandsjob.h \
    $$PWD/commands/runcommandjob.h \
    $$PWD/custom/customsoundsjob.h \
    $$PWD/custom/customuserstatusjob.h \
    $$PWD/directmessage/createdmjob.h \
    $$PWD/directmessage/opendmjob.h \
    $$PWD/directmessage/settopicdmjob.h \
    $$PWD/downloadfilejob.h \
    $$PWD/e2e/fetchmykeysjob.h \
    $$PWD/e2e/setuserpublicandprivatekeysjob.h \
    $$PWD/emoji/deleteemojicustomjob.h \
    $$PWD/emoji/loademojicustomjob.h \
    $$PWD/groups/archivegroupsjob.h \
    $$PWD/groups/changegroupsannouncementjob.h \
    $$PWD/groups/changegroupsdescriptionjob.h \
    $$PWD/groups/changegroupsencryptedjob.h \
    $$PWD/groups/changegroupsnamejob.h \
    $$PWD/groups/changegroupsreadonlyjob.h \
    $$PWD/groups/changegroupstopicjob.h \
    $$PWD/groups/creategroupsjob.h \
    $$PWD/groups/getgrouprolesjob.h \
    $$PWD/groups/groupaddleaderjob.h \
    $$PWD/groups/groupaddmoderatorjob.h \
    $$PWD/groups/groupaddownerjob.h \
    $$PWD/groups/groupremoveleaderjob.h \
    $$PWD/groups/groupremovemoderatorjob.h \
    $$PWD/groups/groupremoveownerjob.h \
    $$PWD/groups/groupsdeletejob.h \
    $$PWD/groups/groupsinfojob.h \
    $$PWD/groups/groupsinvitejob.h \
    $$PWD/groups/groupskickjob.h \
    $$PWD/groups/leavegroupsjob.h \
    $$PWD/groups/setgrouptypejob.h \
    $$PWD/invite/findorcreateinvitejob.h \
    $$PWD/librestapi_private_export.h \
    $$PWD/librocketchatrestapi-qt5_export.h \
    $$PWD/misc/owninfojob.h \
    $$PWD/misc/settingsoauthjob.h \
    $$PWD/permissions/listpermissionsjob.h \
    $$PWD/restapiabstractjob.h \
    $$PWD/restapimethod.h \
    $$PWD/restapirequest.h \
    $$PWD/restapiutil.h \
    $$PWD/rocketchatqtrestapi_debug.h \
    $$PWD/rooms/getdiscussionsjob.h \
    $$PWD/rooms/getroomsjob.h \
    $$PWD/rooms/roomfavoritejob.h \
    $$PWD/rooms/roomleavejob.h \
    $$PWD/rooms/roomsautocompletechannelandprivatejob.h \
    $$PWD/rooms/roomsinfojob.h \
    $$PWD/rooms/roomstartdiscussionjob.h \
    $$PWD/rooms/savenotificationjob.h \
    $$PWD/serverinfojob.h \
    $$PWD/settings/privateinfojob.h \
    $$PWD/spotlightjob.h \
    $$PWD/subscriptions/markroomasreadjob.h \
    $$PWD/subscriptions/markroomasunreadjob.h \
    $$PWD/uploadfilejob.h \
    $$PWD/users/forgotpasswordjob.h \
    $$PWD/users/getavatarjob.h \
    $$PWD/users/getpresencejob.h \
    $$PWD/users/getusernamesuggestionjob.h \
    $$PWD/users/resetavatarjob.h \
    $$PWD/users/setavatarjob.h \
    $$PWD/users/setstatusjob.h \
    $$PWD/users/userbasejob.h \
    $$PWD/users/userinfojob.h \
    $$PWD/users/usersautocompletejob.h \
    $$PWD/users/userspresencejob.h \
    $$PWD/video-conference/videoconfupdatejitsitimeoutjob.h

