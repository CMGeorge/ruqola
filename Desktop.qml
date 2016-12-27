
// Skeleton from https://github.com/achipa/outqross_blog.git
// Almost everything has been re-adapted

import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import Qt.labs.settings 1.0
import QtGraphicalEffects 1.0

import KDE.Ruqola.UserData 1.0
import KDE.Ruqola.DDPClient 1.0

// import "Log.js" as Log
// import "Data.js" as Data

ApplicationWindow {
    property int margin: 11
    property string statusText
    
    property string lightGreen: "#6ab141";
    property string darkGreen: "#00613a";
    
    property string selectedRoomID: "";
    
    id: appid
    title: qsTr("Ruqola")
    width: 800
    height: 600
    visible: true
    
    Shortcut {
        sequence: StandardKey.Quit
        context: Qt.ApplicationShortcut
        onActivated: Qt.quit()
    }
    
    Login {
        id: loginTab
        visible: (UserData.loginStatus == DDPClient.LoginFailed || UserData.loginStatus == DDPClient.LoggedOut)
        
        anchors.fill:parent
        z: 10
        serverURL: UserData.serverURL
        username: UserData.userName
        onAccepted: {
            UserData.password = loginTab.password;
            UserData.userName = loginTab.username;
            UserData.serverURL = loginTab.serverURL;
            UserData.tryLogin();
        }        
    }
    
    BusyIndicator {
        id: busy
        anchors.centerIn: parent
        visible: UserData.loginStatus == DDPClient.LoggingIn
    }

    Item {
        id: mainWidget
        anchors.fill: parent
        visible: !loginTab.visible

        Rectangle {
            id: userBox
            anchors.top: parent.top
            width: parent.width
            anchors.left: parent.left
            anchors.right: roomsList.right
            height: 40
            color: darkGreen
            Text {
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                anchors.rightMargin: 10
                anchors.fill: parent
                font.pointSize: 12
                color: "white"
                text: "Hello, " + UserData.userName
            }
            
        }
            
        RoomsView {
            anchors.top: userBox.bottom
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.margins: 0            
            width: 200
            height: appid.height

            id: roomsList
            
            model: UserData.roomModel();
//     model: RoomModelTest {}

            visible: parent.visible
            
            selectedRoomID: appid.selectedRoomID;
            
            onRoomSelected: {
                if (roomID == selectedRoomID) {
                    return;
                }
                console.log("Choosing room", roomID);
                appid.selectedRoomID = roomID;
                activeChat.model = UserData.getModelForRoom(roomID)
            }
            
            onCountChanged: {
                console.log("We have", roomsList.count, "rooms")
            }
            
            LinearGradient {
                id: greenGradient
                anchors.fill: parent
                start: Qt.point(0, 0)
                end: Qt.point(roomsList.width, 0)
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#6ab141" }
                    GradientStop { position: 1.0; color: "#00613a" }
                }
                z: -1;
                
            }
        }
        
        
        Item {
            
            anchors.right: parent.right
            anchors.left: roomsList.right
            anchors.top: parent.top
            anchors.bottom: messageLine.top
            
            Item {
                anchors.fill: parent
                id: greeter
                visible: false
//                 visible: selectedRoomID.empty
                Text {
                    text: "Welcome to Ruqola!";
                }
            }
            
            
                
            ScrollView {
                anchors.fill:parent
                verticalScrollBarPolicy: Qt.ScrollBarAlwaysOn
//                 visible: parent.visible && (UserData.loginStatus != DDPClient.LoggingIn)
                visible: !greeter.visible

         
                ListView {
                    id: activeChat
//                     model: UserData.getModelForRoom(selectedRoomID)

                    onCountChanged: {
    //                     console.log("changed")
    //                     var newIndex = count - 1 // last index
    //                     positionViewAtEnd()
                        positionViewAtIndex(count - 1, ListView.Beginning)

    //                     currentIndex = newIndex
                    }
    //                 Component.onCompleted: positionViewAtEnd()
                    Component.onCompleted: positionViewAtIndex(count - 1, ListView.Beginning)


    //                 onSelectedRoomIDChanged: { console.log("CHANGED"); activeChat.positionViewAtEnd(); }

    //                 model: myModel
                    anchors.fill:parent
                    visible : count > 0
                    
                    
                    z: -1
                    
        //             ScrollBar.vertical: ScrollBar { }
                    
                    delegate: Message {
                                i_messageText: messageText
                                i_username: username
                                i_systemMessage: systemMessage
                                i_systemMessageType: type
                                width: parent.width
                            }
                }
            }
        }
        TextField {
            id: messageLine
            anchors.right: parent.right
            anchors.left: roomsList.right
            anchors.bottom: parent.bottom
            placeholderText: qsTr("Enter message")
            height: 2.7*font.pixelSize
//             font.pointSize: 12
            
            onAccepted: {
                if (text != "") {
                    UserData.sendMessage(selectedRoomID, text);
                    text = "";
                }
            }
        }
    }
    
    Rectangle {
        z: -10
        anchors.fill: parent
        color: "white"
    }
    
    onClosing: {
        console.log("Minimizing to systray...");
        hide();
    }
    
    function toggleShow(reason) {
//         console.log ("Showing");
        
        if (visible) {
            hide();
        } else {
            show();
            raise();
            requestActivate();
        }
    }
    Component.onCompleted: {
        systrayIcon.activated.connect(toggleShow);
//         roomsList.model = UserData.roomModel();
//         systrayIcon.showMessage("Connected", "We are CONNECTED!");
    
        timer.start();
        timer.fire();
    }

/*    
    Timer {
        id: timer
        interval: 1000
        onTriggered: {
//             console.log("FIRE");
            switch (UserData.loginStatus) {
                case UserData.NotConnected:
                    statusText = qsTr("Not connected.");
                    break;
                case UserData.LoggedIn:
                        statusText = qsTr("Connected to " + UserData.serverURL);
                        break;
                    
            }
        }
        repeat: true
    }*/

    onStatusTextChanged: timer.restart();
}
