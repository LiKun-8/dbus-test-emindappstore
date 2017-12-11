#ifndef DETAILWIDGET_H
#define DETAILWIDGET_H

#include <QObject>
#include <QWidget>
#include <qglobal.h>
#include <QSpacerItem>
#include "commentlistmodel.h"
#include <packagekitqt5/PackageKit/Transaction>
#include <packagekitqt5/PackageKit/Daemon>
#include <Details>

class QQuickWidget;
class QLabel;
class QPushButton;
class QTextEdit;
class QHBoxLayout;
class QVBoxLayout;
class QFormLayout;
class QQmlEngine;
class QProgressBar;
class QGridLayout;
class QScrollArea;
class ShareData;
class QNetworkAccessManager;
class QNetworkReply;
class JSONFUNC;
class QQmlContext;
class DataManager;


class DetailWidget : public QWidget
{
    Q_OBJECT
public:
    DetailWidget(QWidget *parent,ShareData* shareData,JSONFUNC *jsonFunc);
    ~DetailWidget();

    QString octetsToString( qint64 octets );


signals:

    void sigDownload(QString productName,int releaseId);
    void sigUpdate(QString productName,int releaseId);
    void sigReDownload(QString productName,int releaseId);
    void sigReUpdate(QString productName,int releaseId);
    void updatePackage(QString productName,int releaseId);


public slots:
    void refreshPage(int);
    void toPage(int);
    void replyFinished(QNetworkReply *reply);
    void updatePackageStatus(QString productName, bool bo,int flag);
    void onBtnStateClicked();
    bool updateJsonOk();
    void commentJsonOk();
    void userJsonOk();
    void screenImageJsonOk();
    void submitComment();
    void cancelComment();
    void reGetComment(qint32);
    void getCurrVerComm();
    void getHisVerComm();
    void showMore(bool);
    double calMark();

    void calMark0();
    void calMark1();
    void calMark2();
    void calMark3();
    void calMark4();


    void resetPackId(PackageKit::Transaction::Info, QString packageID, QString summar);
    void onGetDetails(const PackageKit::Details &value);

private:
    void initialWidgets();
    void initCommentStar();
    void initCommProgressBar();
    void layoutWidgets();
    void getImage(QString headUrl);
    void initStar(QLabel* lbStar,double num);
    void openApp();
    void passComment();
    void loadData();
    void createUserCommBox();
    void setAppDetailInfo(const QString& infoText);
    void resolveNameToId(QString packName);
    void setInstallSize(QString pacId);
    QString transPackSize(const double &size);
    void resetMarkBtn();
    void checkBtnState();


private:
    CommentListModel *commentModel;
    DataManager *screenImgData;
//    CommentData *commData;
    CommentData commData;
    QString mAppDetailInfo;
    QString mStyStr0;
    QString mStyStr50;
    QString mStyStr100;
    PackageKit::Transaction *detailTransaction;
    QString pkgId;
    int mProstatus;
    double productMark;
    int mBtnNumber;

    QQmlContext *ctx;
    QQmlContext *ctx1;

    qint32 mReleaseId[2];
    qint32 modelIndex;
    qint32 mCommentSum;
    qint32 mCounter;
    qint32 mCurrReleaseId;

    QLabel *lbProductImg;
    QLabel *lbProductName;
    QLabel *lbComment;
    QLabel *lbCommentStar;
    QLabel *lbCommentMark;
    QLabel *lbProductVersion;
    QLabel *lbVersionNo;
    QLabel *lbProductSize;
    QLabel *lbSizeValue;
    QPushButton *btnState;
    QLabel *lbAppInfo;
    QLabel *lbAppInfoDetail;
    QPushButton *btnMoreInfo;

    QLabel *lbUserComment;
    QLabel *lbUserCommMark;
    QLabel *lbUserCommStar;
    QLabel *lbTotalCommenters;
    qint32 totalCommenters;
    QLabel *lbStarLevel;


    QProgressBar *pbStarLevel;
    QProgressBar *pbStarLevel1;
    QProgressBar *pbStarLevel2;
    QProgressBar *pbStarLevel3;
    QProgressBar *pbStarLevel4;
    QProgressBar *pbStarLevel5;
    QLabel *lbLevel1Users;
    QLabel *lbLevel2Users;
    QLabel *lbLevel3Users;
    QLabel *lbLevel4Users;
    QLabel *lbLevel5Users;

    QLabel *lbMyMark;
    QPushButton *btnMyStarLevel;
//    QLabel *lbmyStarLevel;
    QLabel *lbmyComment;
    QTextEdit *teCommContent;
    QPushButton *btnSubmit;
    QPushButton *btnCancel;

    QQuickWidget *userCommBoxer;
    QQuickWidget *imgBoxer;


    QHBoxLayout *hly1;
    QHBoxLayout *hly2;
    QHBoxLayout *hly3;
    QHBoxLayout *hly4;
    QHBoxLayout *hly5;
    QHBoxLayout *hly6;
    QHBoxLayout *hly7;
    QHBoxLayout *hly8;
    QHBoxLayout *hly9;
    QHBoxLayout *hly10;
    QHBoxLayout *hly11;
    QHBoxLayout *hly12;
    QHBoxLayout *hly13;
    QHBoxLayout *hly14;
    QHBoxLayout *hly15;
    QHBoxLayout *hly16;
    QHBoxLayout *hly17;

    QVBoxLayout *vly1;
    QVBoxLayout *vly2;
    QVBoxLayout *vly3;
    QVBoxLayout *vly4;
    QVBoxLayout *vly5;
    QVBoxLayout *vly6;
    QVBoxLayout *vly;
    QGridLayout *gly;

    QSpacerItem *spItem;

    QQmlEngine *qmlEnginer;
    QQmlEngine *qmlEnginer1;

    QString productName;
    QString productCommMark;
    QString productVersion;
    QString productSize;
    QString userCounts;

    ShareData *shareData;
    JSONFUNC *jsonFunc;
    QNetworkAccessManager *imageManager;
    int mProductId;
    QString mAppName;
//    QPixmap pixmap;

    QScrollArea *scrollArea;
    QWidget *detailArea;


};

#endif // DETAILWIDGET_H
