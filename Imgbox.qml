import QtQuick 2.5
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.3
import "httpData.js" as Datajs
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

Item {
    id:imgBox
    width: 950
    height:220
    property int margin : 10
    property var imgUrlArray : null
    property var xmlhttp : null
    property int btnWidth:20
    property int btnHeight: 20
    //    property double iyaxis:0
    //    property double itransAngle:0
    property var servIp: null


    Component.onCompleted: {
        servIp = BannerData.serverIp;
        console.log("servIp===================",servIp);
        var httpUrl = servIp + "banners";
        Datajs.get(httpUrl,servIp);
        timer.start();
    }


//    RowLayout{
//        id: rlyMain
//        anchors.fill: parent
//        z:1
//        //        anchors.centerIn:   parent
//        spacing: 800
//        Button{
//            Layout.preferredWidth: 50
//            Layout.preferredHeight: 50
//            anchors.left: parent.left
//            iconSource: "qrc:/image/return_black.png"
//            onClicked: imgView.decrementCurrentIndex()
//            opacity:0.3
//        }
//        Button{
//            Layout.preferredWidth: 50
//            Layout.preferredHeight: 50
//            anchors.right: parent.right
//            iconSource:"qrc:/image/forward_black.png"
//            onClicked: imgView.incrementCurrentIndex()
//            opacity:0.3

//        }
//    }

    Timer{
        id:timer
        interval: 5000
        running: true
        repeat:true
        onTriggered:imgView.incrementCurrentIndex()
    }

    RowLayout{
        z:1
        id:rly1
        x: 300
        y: 170
        width: 150
        height: 50
        anchors.horizontalCenter: parent.horizontalCenter
        transformOrigin: Item.Bottom
        spacing:2
        Button{
            id:btn1
            Layout.fillWidth: false
            clip: false
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            transformOrigin: Item.Bottom
            Layout.preferredWidth: btnWidth
            Layout.preferredHeight:btnHeight
            onClicked: {
                btn1.style = circleBtnSty2;
                btn5.style = circleBtnSty;
                btn2.style = circleBtnSty;
                btn3.style = circleBtnSty;
                btn4.style = circleBtnSty;
                imgView.positionViewAtIndex(0,ListView.Center)
            }
            style:circleBtnSty
        }
        Button{
            id:btn2
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredWidth: btnWidth
            Layout.preferredHeight:btnHeight
            onClicked: {
                btn2.style = circleBtnSty2;
                btn1.style = circleBtnSty;
                btn5.style = circleBtnSty;
                btn3.style = circleBtnSty;
                btn4.style = circleBtnSty;
                imgView.positionViewAtIndex(1,ListView.Center)
            }
            style:circleBtnSty

        }
        Button{
            id:btn3
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredWidth: btnWidth
            Layout.preferredHeight:btnHeight
            onClicked: {
                btn3.style = circleBtnSty2;
                btn1.style = circleBtnSty;
                btn2.style = circleBtnSty;
                btn5.style = circleBtnSty;
                btn4.style = circleBtnSty;
                imgView.positionViewAtIndex(2,ListView.Center)
            }
            style:circleBtnSty
        }
        Button{
            id:btn4
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredWidth: btnWidth
            Layout.preferredHeight:btnHeight
            onClicked: {
                btn4.style = circleBtnSty2;
                btn1.style = circleBtnSty;
                btn2.style = circleBtnSty;
                btn3.style = circleBtnSty;
                btn5.style = circleBtnSty;
                imgView.positionViewAtIndex(3,ListView.Center)
            }
            style:circleBtnSty
        }
        Button{
            id:btn5
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredWidth: btnWidth
            Layout.preferredHeight:btnHeight
            onClicked: {
                btn5.style = circleBtnSty2;
                btn1.style = circleBtnSty;
                btn2.style = circleBtnSty;
                btn3.style = circleBtnSty;
                btn4.style = circleBtnSty;
                imgView.positionViewAtIndex(4,ListView.Center)
            }
            style:circleBtnSty
        }
        Component{
            id:circleBtnSty2
            ButtonStyle{
                background: Rectangle{
                    implicitWidth: 10
                    implicitHeight: 10
                    radius: 10
                    color:"#027aff"
                }
            }
        }
        Component{
            id:circleBtnSty
            ButtonStyle{
                background: Rectangle{
                    implicitWidth: 10
                    implicitHeight: 10
                    radius: 10
                    color:"#e0e0e0"
                }
            }
        }

    }



    ListModel{
        id:pModel
    }

//    Component{
//        id:iDelegate

//        Rectangle{
//            height: 192
//            id:imgRect
//            width: wapper.width
////            z:PathView.zOrder
//            color:"transparent"
////            property double rotXX:PathView.transAngle
////            property double yyAxis:PathView.yaxis
////            transform: Rotation{
////                origin.x:0
////                origin.y:0
////                axis{
////                    x:0
////                    y:1
////                    z:0
////                }
////                angle: wrapper.rotXX
////            }
//            MouseArea{
//                anchors.fill: parent
//                onClicked:{
//                    console.log("nice");
//                    var aproductId = wrapper.source.split("_");
//                    var bproductId = aproductId[1].split(".");
//                    console.log("wrapper.imgsource================",bproductId[0]);
//                    BannerData.bannerProId = bproductId[0];
//                    BannerData.toDtPage();
//                }
//            }
//        }
//        Column{
//            spacing:10

//            Image{
//                id:wrapper
//                z:PathView.zOrder
//                height: 192
//                source: imgSource
//                scale:PathView.imgScale
//                property double rotX:PathView.transAngle

//                transform: Rotation{
//                    id:rotation
//                    origin.x:0
//                    origin.y:0
//                    axis{
//                        x:0
//                        y:1
//                        z:0
//                    }
//                    angle:wrapper.rotX
//                }
//                smooth:true
//            }



//                Image{
//                    width:wrapper.width
//                    height:wrapper.height/3
//                    id:reflectImg
//                    source:wrapper.source
//                    opacity:0.4
//                    transform: Scale{
//                        yScale: -0.2
//                        origin.y:imgBox.height-wrapper.height
//                    }
//                }

//                LinearGradient{
//                    width:wrapper.width
//                    height:wrapper.height/3
//                    gradient:Gradient{
//                        GradientStop{position: 0.0;color:Qt.rgba(0,0,0,0.0)}
//                        GradientStop{position: 0.1;color:Qt.rgba(0,0,0,0.1)}
//                    }
//                }

//        }


//        ShaderEffect {
//            anchors.top: wrapper.bottom
//            width: wrapper.width
//            height: wrapper.height
//            anchors.left: wrapper.left

//            property variant source: wrapper
//            property size sourceSize: Qt.size(0.5 / wrapper.width, 0.5 / wrapper.height)

//            fragmentShader: "
//                    varying highp vec2 qt_TexCoord0;
//                    uniform lowp sampler2D source;
//                    uniform lowp vec2 sourceSize;
//                    uniform lowp float qt_Opacity;
//                    void main() {

//                        lowp vec2 tc = qt_TexCoord0 * vec2(1, -1) + vec2(0, 1);
//                        lowp vec4 col = 0.25 * (texture2D(source, tc + sourceSize)
//                                                + texture2D(source, tc- sourceSize)
//                                                + texture2D(source, tc + sourceSize * vec2(1, -1))
//                                                + texture2D(source, tc + sourceSize * vec2(-1, 1))
//                                               );
//                        gl_FragColor = col * qt_Opacity * (1.0 - qt_TexCoord0.y) * 0.2;
//                    }"
//        }

//    }

    PathView {
        id:imgView
//        anchors.fill: parent
        width:imgBox.width
        height: imgBox.height
        model: pModel
//        delegate: iDelegate
        delegate:DelegateItem{}
        pathItemCount:3
//        resizeMode:ajust

        path: Path {
            startX: 0; startY: imgBox.height/2
            PathAttribute{name:"zOrder";value:0}
            PathAttribute{name:"transAngle";value:40.0}
//            PathAttribute{name:"yaxis";value:0}
//            PathAttribute{name:"iWidth";value:212.0}
            PathAttribute{name:"imgScale";value:1.0}

            PathLine{
                x:imgBox.width/2
                y:imgBox.height/2
            }
            PathAttribute{name:"zOrder";value:10}
            PathAttribute{name:"transAngle";value:0}
//            PathAttribute{name:"yaxis";value:-1.0}
//            PathAttribute{name:"iWidth";value:524.0}
            PathAttribute{name:"imgScale";value:1.0}
//            PathPercent{value:1.0}


            PathLine{
                x:  imgBox.width
                y:  imgBox.height/3
            }
            PathAttribute{name:"zOrder";value:0}
            PathAttribute{name:"transAngle";value:-70.0}
//            PathAttribute{name:"yaxis";value:0}
//            PathAttribute{name:"iWidth";value:212.0}
            PathAttribute{name:"imgScale";value:0.8}

//            PathPercent{value:1.0}

        }
        focus:true
        Keys.onLeftPressed: decrementCurrentIndex()
        Keys.onRightPressed: incrementCurrentIndex()
        preferredHighlightBegin:0.5
        preferredHighlightEnd:0.5
    }

}



