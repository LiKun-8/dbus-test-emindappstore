var servIp = null;
var mproductId = 1;
function get(surl,imgUrl){
    console.log("surl================",surl);
    servIp = imgUrl;
//    if(shttp == null){
        shttp = new XMLHttpRequest();
        shttp.onreadystatechange = onImageReady;
        console.log("shttp created");
//    }else{
//        console.log("shttp not created");
//    }

    if(shttp.readyState == 0){
        shttp.open("GET",surl,true);
        shttp.setRequestHeader("Content-Type","application/json");
        shttp.setRequestHeader("User-Agent","QML");
        shttp.send();
        console.log("http request sent");
    }
//    console.log("readyState=========================",shttp.readyState);
}


function onImageReady(){
    imgModel.clear();
    var imgAddr = servIp + "icon/"
//    console.log("onImageReady=========================");
    if(shttp.readyState == XMLHttpRequest.DONE){
        var jsonData = JSON.parse(shttp.responseText);
        for(var i=0;i < jsonData["screen_image"].length; i++){
            imgModel.append({"imgSource":imgAddr + jsonData["screen_image"][i]["image_url"]});
//            console.log("parsing jsonData===========",jsonData["screen_image"][i]["image_url"]);
        }
    }
}
