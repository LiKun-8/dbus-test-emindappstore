#ifndef SHAREDATA_H
#define SHAREDATA_H
#include <QString>
#include <QMap>

//proStatus:(1:下载 2:更新 3:打开 4:重新下载 5:重新更新 6:正在卸载 7:正在下载 8:正在更新 9:卸载)
#define DOWNLOAD 1
#define UPDATE 2
#define OPEN 3
#define REDOWNLOAD 4
#define REUPDATE 5
#define UNINSTALLING 6
#define DOWNLOADING 7
#define UPDATING 8
#define UNINSTALL 9

//分类项结构体
struct CLASSSTRUCT{
    int category;
    int releaseId;
    int proStatus;
    QString proImage;
    QString proName;
    double proStar;
    QString proDescription;
    QString exeFile;
    QString packageName;
    CLASSSTRUCT(int cate,int releaseid,int prostatus,const QString & image,const QString & name,double star,const QString & desc,const QString &exe,const QString &packagename):
        category(cate),releaseId(releaseid),proStatus(prostatus),proImage(image),proName(name),proStar(star),proDescription(desc),exeFile(exe),packageName(packagename){}
};

//更新数据结构体
struct UPDATESTRUCT{
    int productId;
    QString version;
    QString proImage;
    QString proName;
    QString changeLog;
    QString downloadUrl;
    double packageSize;
    QString exeFile;
    QString packageName;
    UPDATESTRUCT(int id,QString ver,const QString &image,const QString &name,const QString & changelog,const QString & downloadurl,double packagesize,const QString & exe,const QString &packagename):
        productId(id),version(ver),proImage(image),proName(name),changeLog(changelog),downloadUrl(downloadurl),packageSize(packagesize),exeFile(exe),packageName(packagename){}
};

//产品截图结构体
struct SCREENIMAGE{
    int imageId;
    int releaseId;
    QString imageUrl;
    SCREENIMAGE(int imageid,int releaseid,const QString & imageurl):
        imageId(imageid),releaseId(releaseid),imageUrl(imageurl){}
};

//评论结构体
struct COMMENT{
    int commentId;
    int releaseId;
    int userId;
    QString commentText;
    double commentGrade;
    QString CommentDate;
    COMMENT(int commentid,int releaseid,int userid,const QString & commenttext,double commentgrade,const QString & commentdate):
        commentId(commentid),releaseId(releaseid),userId(userid),commentText(commenttext),commentGrade(commentgrade),CommentDate(commentdate){}
};

//横幅结构体
struct BANNER{
    int Id;
    int priority;
    QString imageUrl;
    BANNER(int id,int prio,const QString & imageurl):
        Id(id),priority(prio),imageUrl(imageurl){}
};

//用户结构体
struct USER{
    int Id;
    QString userName;
    QString userAvatarURL;//用户头像URL
    QString userMailURL;//用户邮件地址
    USER(int id,const QString & username,const QString & useravatarurl,const QString & usermailurl):
        Id(id),userName(username),userAvatarURL(useravatarurl),userMailURL(usermailurl){}
};

//int 代表分类项编号，QString代表分类名
typedef QMap<int,QString> CATEGORYMAP;

//int 代表product id
typedef QMap<int,CLASSSTRUCT> CLASSSTRUCTMAP;

//第一个int代表分类id，第二个代表每个分类有多少软件元素
typedef QMap<int,int> ELEMENTNUMBERMAP;

//int 代表release id
typedef QMap<int,UPDATESTRUCT> UPDATESTRUCTMAP;

//int 代表product id，第二个int代表优先级
typedef QMap<int,int> RECOMMENDMAP;

//int 代表product id
typedef QMap<int,SCREENIMAGE> SCREENIMAGEMAP;

//int 代表第几条评论
typedef QMap<int,COMMENT> COMMENTMAP;

//int 代表第几条横幅
typedef QMap<int,BANNER> BANNERMAP;

typedef QMap<int,USER> USERMAP;

class ShareData
{
public:
    CATEGORYMAP categoryMap;    //软件分类名map
    CLASSSTRUCTMAP classStrMap;     //软件分类map
    ELEMENTNUMBERMAP classElementNumMap;    //每个分类包含软件数map
    UPDATESTRUCTMAP updateStrMap;   //更新(release)数据map
    RECOMMENDMAP recommendMap;  //热门推荐map
    SCREENIMAGEMAP screenImageMap;  //产品截图map
    COMMENTMAP commentMap;      //评论map
    BANNERMAP bannerMap;        //横幅图片map
    USERMAP userMap;    //用户map
};

#endif // SHAREDATA_H
