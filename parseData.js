
function parseMark(mark){
    if(mark < 0)
        return "qrc:/image/star_0.png";
    else if(mark === 0)
        return "qrc:/image/star_50.png";
    else if(mark > 0 && mark < 1)
        return "qrc:/image/star_100.png";
    else if(mark >= 1)
        return "qrc:/image/star_100.png";

}
