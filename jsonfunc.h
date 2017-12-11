#ifndef JSONFUNCH
#define JSONFUNCH

#include <QObject>
#include <QProcess>
#include <QByteArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include "sharedata.h"
#include <QMutex>

#define CATEGORIES  1
#define RELEASE     2
#define PRODUCTS    3
#define RECOMMEND   4
#define SCRIMAGE    5
#define COMMENTDATA 6
#define BANNERFLAG  7

class JSONFUNC : public QObject
{
    Q_OBJECT

public:
    JSONFUNC(ShareData *shareData);
    void getCategoryNum();  //获取分类相关数据
    void getUpdateRelease(int *array, int size); //获取软件release相关信息（array代表releaseid数组，size代表数组长度）
    void setAppname();  //获取product表数据
    void getRecommend();    //获取热门推荐相关数据
    void getScreenImage(int id);    //获取软件截图相关数据（id代表releaseid）
    void getComment(int *array);    //获取评论相关数据（array代表releaseid数组）
    void getCommentHistory(int *array);    //获取评论相关数据（array代表releaseid数组）
    void getBanner();   //获取横幅相关数据
    void getUser(int id); //获取用户相关数据（id代表用户id）
    void sendComment(QString &data);    //上传评论（data代表要上传的评论数据）

protected slots:
    void jsonAnalysis(QNetworkReply *reply);    //数据解析

signals:
    void productIsOk(); //获取product 表数据完毕
    void categoryIsOk(int num); //获取category 表数据完毕（num代表多少个分类）
    void updateIsOk();  //获取release 表数据完毕
    void recommendIsOk();   //获取recommend 表数据完毕
    void commentIsOk(); //获取comment 表数据完毕
    void bannerIsOk();  //获取banner 表数据完毕
    void screenImageIsOk(); //获取screenImage 表数据完毕
    void userIsOk();    //获取user 表数据完毕
    void jsonError();

private:
    int jsonFlag;
    int categoryNum;
    QByteArray testArray;
    QNetworkAccessManager *manager;
    void getCategoryNum(QJsonObject obj);
    void getProducts(QJsonObject obj);
    void getUpdateRelease(QJsonObject obj);
    void getRecommend(QJsonObject obj);
    void getScreenImage(QJsonObject obj);
    void getComment(QJsonObject obj);
    void getBanner(QJsonObject obj);
    void getUser(QJsonObject obj);
    ShareData *jsonData;

    QString ipString;
    QString iconString;
};

#endif // JSONFUNCH
