import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import "screenImgData.js" as ImgJs
//import QtGraphicalEffects 1.0

Item {
    id:imgBoxer
    width:980
    height:300
    property var shttp : null
    property int productId: 1
    property var imgUrl: null

    Connections{
        target:sImgData
        onSigGetScreenImg:{
            console.log("onSigGetScreenImg=======================");
            productId = sImgData.sproductId;
            imgUrl = sImgData.serverIp;
            var httpUrl = sImgData.serverIp + "screen_image/" + productId;
            ImgJs.get(httpUrl,imgUrl);
        }
    }

    ListModel{
        id:imgModel
    }

    ListView{
        anchors.fill: parent
        model: imgModel
        delegate: imgDelegate
        orientation: ListView.Horizontal
    }

    Component{
        id:imgDelegate

        Rectangle{
            width:310
            height:310
            //            gradient: Gradient {
            //                GradientStop { position: 0; color: "#000000" }
            //                GradientStop { position: 1; color: "#000000" }
            //            }
            //            Layout.preferredWidth: 300
            //            Layout.preferredHeight:  300
            RowLayout{
                ColumnLayout{
                    Image {
                        id:img
                        width:300
                        height:300
                        source: imgSource
                    }

                    //                    Rectangle{
                    //                        id:imgRect
                    //                        anchors.top:img.bottom
                    ////                        color:"#grey"
                    //                        Layout.preferredWidth: img.width
                    //                        Layout.preferredHeight: img.height
                    ////                        Gradient{
                    ////                            GradientStop { position: 0; color: "#000000FF" }
                    ////                            GradientStop { position: 0.33; color: "#00000022" }
                    ////                            GradientStop { position: 1; color: "#00000000" }
                    ////                        }
                    //                        LinearGradient {
                    //                            source:imgMirror
                    //                            anchors.fill: parent
                    //                            start: Qt.point(0, 0)
                    //                            end: Qt.point(0, 300)
                    //                            gradient: Gradient {
                    //                                GradientStop { position: 0.05; color: "#000000FF" }
                    //                                GradientStop { position: 0.1; color: "#000000DD" }
                    //                                GradientStop { position: 0.15; color: "#00000099" }
                    //                                GradientStop { position: 0.2; color: "#00000000" }
                    //                                GradientStop { position: 1.0; color: "#000000FF" }
                    //                            }
                    //                        }
                    //                        Image{
                    //                            id:imgMirror
                    ////                            anchors.top:img.bottom
                    //                            source:img.source
                    //                            mirror:true
                    //                            verticalAlignment: img.AlignTop
                    //                            rotation: 180
                    //                        }

                    //                    }
                    //                }

                    ShaderEffect {
                        anchors.top: img.bottom
                        Layout.preferredWidth:  img.width
                        Layout.preferredHeight: img.height
                        anchors.left: img.left

                        property variant source: img
                        property size sourceSize: Qt.size(0.5 / img.width, 0.5 / img.height)
                        fragmentShader: "
                                            varying highp vec2 qt_TexCoord0;
                                            uniform lowp sampler2D source;
                                            uniform lowp vec2 sourceSize;
                                            uniform lowp float qt_Opacity;
                                            void main() {

                                                lowp vec2 tc = qt_TexCoord0 * vec2(1, -1) + vec2(0, 1);
                                                lowp vec4 col = 0.25 * (texture2D(source, tc + sourceSize)
                                                                        + texture2D(source, tc- sourceSize)
                                                                        + texture2D(source, tc + sourceSize * vec2(1, -1))
                                                                        + texture2D(source, tc + sourceSize * vec2(-1, 1))
                                                                       );
                                                gl_FragColor = col * qt_Opacity * (1.0 - qt_TexCoord0.y) * 0.2;
                                            }"
                    }
                }


                Item{
                    width:8

                }
            }
        }

    }
}
