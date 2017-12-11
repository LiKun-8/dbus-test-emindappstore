import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import "parseData.js" as ParseData

Item {
    id:commBox
    implicitWidth:  910
    implicitHeight:  800
    property int btnWidth : 30
    property int btnHeight : 30
    property int btnMark:0
    property double commMark:0.0
//    property string starImg0: "qrc:/image/star_0.png"
//    property string starImg50: "qrc:/image/star_50.png"
//    property string starImg100: "qrc:/image/star_100.png"
    property int btnNo:0

    signal sigCommPgIndex(int pIndex)
    signal sigHighlightBtn(int btnIndex)


    Rectangle{
        id:commentArea
        anchors.fill: parent
        height:800
        ColumnLayout{
            x: 0
            y: 55
            Layout.fillHeight: true
            Layout.fillWidth: true
            RowLayout{
                Layout.preferredWidth: 180
                Text{
                    x: 5
                    text:qsTr("user comment")
                    font.pixelSize: 24
                }

                Item{
                    id:spacer5
                    width:15
                }

                Button{
                    id:btnCurrentVer
                    anchors.left: spacer5.right
                    style:btnStyClicked
                    onClicked: {
                        btnMark = 0;
                        btnCurrentVer.style = btnStyClicked;
                        btnHistoryVer.style = btnStyUnClicked;
                        commModel.sigCommCurrentVer();
                    }
                }

                Item{
                    id:spacer6
                    width:3
                }
                Button{
                    id:btnHistoryVer
                    anchors.left:spacer6.right
                    style:btnStyUnClicked
                    onClicked: {
                        btnMark = 1;
                        btnHistoryVer.style = btnStyClicked;
                        btnCurrentVer.style = btnStyUnClicked;
                        commModel.sigCommHistoryVer();
                    }
                }

                Component{
                    id:btnStyClicked
                    ButtonStyle{
                        background: Rectangle{
                            implicitWidth: 72
                            implicitHeight: 24
                            Text{
                                anchors.centerIn: parent
                                text:btnMark == 0?qsTr("current version"):qsTr("history version")
                                font.pixelSize: 12
                                color:"white"
                            }
                            border.width: 1
                            radius: 2
                            color:"#027aff"
                        }
                    }
                }
                Component{
                    id:btnStyUnClicked
                    ButtonStyle{
                        background: Rectangle{
                            implicitWidth: 72
                            implicitHeight: 24
                            Text{
                                anchors.centerIn: parent
                                text:btnMark == 1?qsTr("current version"):qsTr("history version")
                                font.pixelSize: 12
                            }
                            border.width: 1
                            radius: 2
                        }
                    }
                }
            }


            //Comment Area //////////////////////////////////////////////////////////
            RowLayout{
                id:lyView

                WorkerScript{
                    id:modelWorker
                    source:"httpData.js"
                }

                Rectangle{
                    Layout.preferredWidth: 1200
                    Layout.preferredHeight: 750
                    GridView{
                        id:gridView
                        anchors.fill: parent
                        model:commModel
                        delegate: cdelegate
                        cellHeight: 150
                        cellWidth: 1200
                        flow:GridView.TopToBottom
                        interactive: false
                        contentX:listView.contentX

                    }

                    //                    Connections{
                    //                        target:commModel
                    //                        onSigClear:{
                    ////                            modelWorker.sendMessage({'model':commModel});
                    //                            gridView.children = "";
                    ////                            gridView.model = commModel;
                    //                        }
                    //                    }
                    //                    Connections{
                    //                        target:commModel
                    //                        onSigClear:{
                    ////                            modelWorker.sendMessage({'model':commModel});
                    //                        }
                    //                    }

                    Connections{
                        target:commModel
                        onSigSetCount:{
                            console.log("commModel.commCount===================",commModel.commCount);
                            for(var i=0;i<commModel.commCount/5;i++)
                                listModel.append({});
                        }

                    }

                    ListView{
                        id:listView
                        anchors.fill: parent
                        model:listModel
                        delegate:listDel
                        orientation:ListView.Horizontal
                        snapMode:ListView.SnapOneItem
                    }

                    Component{
                        id:listDel
                        Rectangle{
                            width:listView.width
                            height:listView.height
                            color:"transparent"

                        }
                    }

                    ListModel{
                        id:listModel
                    }

                    Component{
                        id:cdelegate

                        Rectangle{
                            width: 920
                            height: 150
                            ColumnLayout{

                                Item{
                                    height:32
                                }

                                RowLayout{
                                    spacing:10

                                    Text{
                                        text:userHeadImg
                                        Layout.maximumWidth:  60
                                        wrapMode:Text.WordWrap
                                    }

                                    Item{
                                        width:8
                                        height:50
                                    }

                                    ColumnLayout{

                                        RowLayout{
                                            Layout.preferredWidth:  900
                                            Text{
                                                text:userName
                                                font.pixelSize: 16
                                                color:"#969696"
                                            }
                                            Item{
                                                width:16
                                            }

                                            RowLayout{
                                                Image{
                                                    source: ParseData.parseMark(commentMark-0.5)
                                                }
                                                Image{
                                                    source:ParseData.parseMark(commentMark-1.5)
                                                }
                                                Image{
                                                    source:ParseData.parseMark(commentMark-2.5)
                                                }
                                                Image{
                                                    source:ParseData.parseMark(commentMark-3.5)
                                                }
                                                Image{
                                                    source:ParseData.parseMark(commentMark-4.5)
                                                }
                                                Item{
                                                    Layout.minimumWidth: 590
                                                }

                                                Text{
                                                    text:date
                                                    font.pixelSize: 14
                                                    color:"#969696"
                                                }

//                                                function getMarkStar(){
//                                                    if(commentMark>0 && commentMar<1)
//                                                        return "qrc:/image/star_50.png";
//                                                    else
//                                                        return "qrc:/image/star_0.png";
//                                                }

                                            }

                                        }
                                        ColumnLayout{
                                            Item{
                                                height:16
                                            }

                                            Text{
                                                text:comment
                                                font.pixelSize: 16
                                                wrapMode: Text.WordWrap
                                            }
                                        }



                                    }


                                }

                                ColumnLayout{
                                    Item{
                                        height: 32
                                    }

                                    Rectangle{
                                        width:1200
                                        height:1
                                        color:"#dcdcdc"
                                    }
                                }
                            }
                        }
                    }
                }
            }


            Connections{
                target:commBox
                onSigCommPgIndex:{
                    if(listView.count>1){
                        pIndex -= 1;
                        listView.contentX = 1200 * pIndex;
//                        listView.positionViewAtIndex(pIndex, ListView.Beginning);
                    console.log("pIndex===========",pIndex);
                    }
                }
            }

            //button area   ////////////////////////////////////////
            ColumnLayout{
                width: 910
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignBottom

                Item{
                    id:spacer1
                    width: 910
                    height:32
                }

                Rectangle{
                    id: rectangle1
                    //                    x: commModel.pageNumber>5?910-btnWidth*(5+2):910-btnWidth*(commModel.pageNumber+2)
                    Layout.leftMargin: commModel.pageNumber>5?900-btnWidth*(5+2):900-btnWidth*(commModel.pageNumber+2)
                    Layout.fillWidth: true
                    //                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    Layout.minimumWidth: commModel.pageNumber<5 ? btnWidth*commModel.pageNumber : btnWidth*5
                    Layout.maximumWidth: btnWidth*5
                    Layout.maximumHeight: btnHeight
                    ListView{
                        id:underView
                        z:-1
                        header: prevComponent
                        footer: nextComponent
                        anchors.fill: parent
                        orientation: ListView.Horizontal
                        model:commModel.pageNumber>5?5:commModel.pageNumber
                        delegate: Component{
                            Rectangle{
                                width: btnWidth
                                height:btnHeight
                                Button{
                                    text:index
                                    visible:false
                                }
                                Item{
                                    width:4
                                }
                            }
                        }
                    }

                    ListView{
                        id:btnPageView
                        anchors.top: parent.bottom
                        z: 0
                        anchors.fill: parent
                        //                        Layout.leftMargin: 30
                        Layout.minimumWidth: commModel.pageNumber<5 ? btnWidth*commModel.pageNumber : btnWidth*5
                        Layout.maximumWidth: btnWidth*5
                        Layout.maximumHeight: btnHeight
                        delegate: btnDelegate
                        model:commModel.pageNumber>5?5:commModel.pageNumber
                        orientation: ListView.Horizontal
                        interactive: false
                        highlightFollowsCurrentItem: false
                        header:Component{
                            Item{
                                z:0
                                width:30
                            }
                        }
                        headerPositioning:ListView.InlineHeader
                        property int pgIncrement:0

                    }

                    Component{
                        id:prevComponent

                        Button{
                            id:btnPrev
                            width:  btnWidth
                            height: btnHeight
                            visible: commModel.pageNumber > 1? true:false
                            text: "<"
                            style:ButtonStyle{
                                background: Rectangle{
                                    implicitWidth:btnWidth
                                    implicitHeight: btnHeight
                                    color:control.pressed?"#027aff":"#f8f8f8"
                                    border.color: "#dcdcdc"
                                }
                            }
                            onClicked: {
                                if(btnPageView.pgIncrement>0)
                                    btnPageView.pgIncrement--;
                                if(btnNo>=1){
                                    btnNo--;
                                    console.log("listView.currentIndex==================",listView.currentIndex);
                                    listView.decrementCurrentIndex();
                                    sigHighlightBtn(btnNo);
                                }
                            }


                        }

                    }
                    Component{
                        id:nextComponent
                        Button{
                            id:btnNext
                            width:  btnWidth
                            height: btnHeight
                            text: ">"
                            onClicked: {
                                if(btnPageView.pgIncrement+5<commModel.pageNumber){
                                    btnPageView.pgIncrement++;
                                }
                                if((btnPageView.pgIncrement+5)%6 == 0 ){
                                    commModel.sigMorePage();
                                }
                                if(btnNo<commModel.pageNumber){
                                    btnNo++;
                                    console.log("listview.currentIndex====",listView.currentIndex);
                                    console.log("btnNo====",btnNo);
                                    listView.incrementCurrentIndex();
                                    sigHighlightBtn(btnNo);
                                }
                            }
                            visible: commModel.pageNumber > 1? true:false
                            style:ButtonStyle{
                                background: Rectangle{
                                    implicitWidth:btnWidth
                                    implicitHeight: btnHeight
                                    color:control.pressed?"#027aff":"#f8f8f8"
                                    border.color: "#dcdcdc"
                                }
                            }
                        }


                    }

                    ListModel{
                        id:btnModel
                    }

                    Component{
                        id:btnDelegate
                        RowLayout{
                            id:bdelegate
                            Button{
                                id:btnpg
                                Layout.maximumWidth:  btnWidth
                                Layout.maximumHeight:btnHeight
                                text:index+1+btnPageView.pgIncrement
                                style: btnpgStyle0
                                onClicked: {
                                    btnpg.style = btnpgStyle;
                                    btnTimer.start();
                                    var sigIndex = index+1+btnPageView.pgIncrement;
                                    sigCommPgIndex(sigIndex);
//                                    if(sigIndex == btnpg.text){
//                                        btnpg.style = btnpgStyle;
//                                    }else
//                                        btnpg.style = btnpgStyle1;
                                    btnNo = index+1+btnPageView.pgIncrement;
                                    listView.currentIndex = btnNo-1;
                                    console.log("btnNo=========",btnNo);
                                }

                            }

                            Timer{
                                id:btnTimer
                                interval: 100
                                onTriggered: btnpg.style = btnpgStyle1;
                            }
                            Timer{
                                id:btnTimer1
                                interval: 200
                                onTriggered: btnpg.style = btnpgStyle1;
                            }

                            Connections{
                                target:commBox
                                onSigHighlightBtn:{
//                                    console.log("onSigHighlightBtn===================",btnIndex);
                                    if(btnIndex == btnpg.text){
                                        btnpg.style = btnpgStyle;
                                        btnTimer1.start();
                                    }else
                                        btnpg.style = btnpgStyle1;
                                }
                            }
                            Component{
                                id:btnpgStyle0
                                ButtonStyle{
                                    background: Rectangle{
                                        implicitWidth:btnWidth
                                        implicitHeight: btnHeight
                                        color:control.pressed?"#027aff":"#f8f8f8"
                                        border.color: "#dcdcdc"
                                    }
                                }
                            }

                            Component{
                                id:btnpgStyle
                                ButtonStyle{
                                    background: Rectangle{
                                        implicitWidth:btnWidth
                                        implicitHeight: btnHeight
                                        color:"#027aff"
                                        border.color: "#dcdcdc"
                                    }
                                }
                            }

                            Component{
                                id:btnpgStyle1
                                ButtonStyle{
                                    background: Rectangle{
                                        implicitWidth:btnWidth
                                        implicitHeight: btnHeight
                                        color:"#f8f8f8"
                                        border.color: "#dcdcdc"
                                    }
                                }
                            }

//                            Item{
//                                width:1
//                            }


                        }

                    }
                }
            }
        }
    }



}


