import QtQuick 2.0

Rectangle {
    id:imgDelegate
    width:340
    height:180
    z:PathView.zOrder
    scale:PathView.imgScale

//    Column{
//        id:delegate
//        spacing:7
    Image{
        id:originImg
        width:imgDelegate.width
        source: imgSource
        scale:imgDelegate.PathView.imgScale
    }

//    Image{
//        id:shadeImg
//        width:imgDelegate.width
//        height:imgDelegate.height/3
//        source:originImg.source
//        opacity:0.3
//        transform: Scale{
//            yScale:-1
//            origin.y:imgBox.height-originImg.height
//        }

//    }

    MouseArea{
        anchors.fill: parent
        onClicked:{
            console.log("nice");
            var aproductId = imgSource.split("_");
            var bproductId = aproductId[1].split(".");
            console.log("wrapper.imgsource================",bproductId[0]);
            BannerData.bannerProId = bproductId[0];
            BannerData.toDtPage();
        }
    }

    transform: Rotation{
        id:rotation
        origin.x:0
        origin.y:0
        axis{
            x:0
            y:1
            z:0
        }
        angle:imgDelegate.PathView.transAngle
    }
    smooth:true
//    }

}
