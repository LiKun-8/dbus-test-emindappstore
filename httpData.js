var serverIp = null;
function get(url,surl){
    serverIp = surl;
    if(xmlhttp == null){
        xmlhttp = new XMLHttpRequest();
        xmlhttp.onreadystatechange = onResultReady;
        console.log("xmlhttp created");
    }

    if(xmlhttp.readyState == 0){
        xmlhttp.open("GET",url,true);
        xmlhttp.setRequestHeader("Content-Type","application/json");
        xmlhttp.setRequestHeader("User-Agent","QML");
        xmlhttp.send();
        console.log("http request sent");
    }
}

function onResultReady(){
//    console.log("onResultReady");
    var imgAddr = serverIp + "icon/"
    var obj = { key:'banner'};
    if(xmlhttp.readyState == XMLHttpRequest.DONE){
//        console.log("response:",xmlhttp.responseText);
        var jsonData = JSON.parse(xmlhttp.responseText);
//        console.log("parsing jsonData===========",jsonData["banner"][1]["avatar_url"]);
        for(var i=0;i < jsonData["banner"].length; i++){
            pModel.append({"imgSource":imgAddr + jsonData["banner"][i]["avatar_url"]});
//            modelWorker.sendMessage({"imgSource":imgAddr + jsonData["banner"][i]["avatar_url"]});
        }
    }

}

//WorkerScript.onMessage = function(msg){
//    console.log("syncing-------------------");
//    msg.model.sync();
//}


