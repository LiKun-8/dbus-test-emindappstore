#include "detailwidget.h"
#include <QQuickWidget>
#include <QLabel>
#include <QPushButton>
#include <QQuickWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QQmlEngine>
#include <QProgressBar>
#include <QGridLayout>
#include <QScrollArea>
#include "sharedata.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "jsonfunc.h"
#include <QCoreApplication>
#include <QtMath>
#include <QScrollArea>
#include <QScrollBar>
#include <QtQml>
#include <QStandardPaths>
#include <QMessageBox>
#include <QFontMetrics>
#include "datamanager.h"



DetailWidget::DetailWidget(QWidget *parent,ShareData *sdata,JSONFUNC   *jsonData) : QWidget(parent)
{
    shareData = sdata;
    jsonFunc = jsonData;
    commentModel = new CommentListModel(this);
    screenImgData = new DataManager(this);
    //    commData = new CommentData();
    initialWidgets();
    layoutWidgets();

    connect(jsonFunc,SIGNAL(updateIsOk()),this,SLOT(updateJsonOk()));
    connect(jsonFunc,SIGNAL(commentIsOk()),this,SLOT(commentJsonOk()));
    connect(jsonFunc,SIGNAL(userIsOk()),this,SLOT(userJsonOk()));
    connect(jsonFunc,SIGNAL(screenImageIsOk()),this,SLOT(screenImageJsonOk()));
    connect(commentModel,SIGNAL(nextPage(int)),this,SLOT(reGetComment(int)));
    connect(commentModel,SIGNAL(sigCommCurrentVer()),this,SLOT(getCurrVerComm()));
    connect(commentModel,SIGNAL(sigCommHistoryVer()),this,SLOT(getHisVerComm()));

    userCommBoxer = NULL;
    productMark = 0;
}

DetailWidget::~DetailWidget()
{
    delete[] lbCommentStar;
    delete[] lbUserCommStar;
    delete[] btnMyStarLevel;
    delete[] lbStarLevel;
    delete[] pbStarLevel;
    delete userCommBoxer;
    delete imgBoxer;
}

void DetailWidget::initialWidgets()
{
    detailArea = new QWidget(this);
    detailArea->setObjectName("detailArea");
    //    detailArea->setMaximumSize(1200,3000);
    detailArea->setMaximumWidth(1200);
    scrollArea  = new QScrollArea(this);

    lbProductImg = new QLabel(detailArea);
    lbProductImg->setFixedSize(128,128);

    lbProductName = new QLabel(detailArea);
    lbProductName->setObjectName("lbProductName");
    lbProductName->setWordWrap(true);
    lbProductName->setMaximumWidth(400);

    lbComment = new QLabel(tr("Comment:"),detailArea);
    lbCommentStar = new QLabel[5];
    lbCommentMark = new QLabel(detailArea);
    lbProductVersion = new QLabel(tr("Version:"),detailArea);
    lbVersionNo = new QLabel(detailArea);
    lbProductSize = new QLabel(tr("Size:"),detailArea);
    lbSizeValue = new QLabel(detailArea);
    btnState = new QPushButton(detailArea);
    btnState->setFixedSize(144,48);
    btnState->setObjectName("btnState");
    connect(btnState,SIGNAL(clicked()),this,SLOT(onBtnStateClicked()));

    lbAppInfo = new QLabel(detailArea);
    lbAppInfo->setText(tr("Application Introduction"));
    lbAppInfo->setObjectName("lbAppInfo");
    lbAppInfoDetail = new QLabel(detailArea);
    lbAppInfoDetail->setObjectName("lbAppInfoDetail");
    lbAppInfoDetail->setMinimumSize(900,42);
    lbAppInfoDetail->setWordWrap(true);
    lbAppInfoDetail->setAlignment(Qt::AlignTop);
    lbAppInfoDetail->setScaledContents(true);

    btnMoreInfo = new QPushButton(tr("More"),detailArea);
    btnMoreInfo->setCheckable(true);
    connect(btnMoreInfo,SIGNAL(toggled(bool)),this,SLOT(showMore(bool)));
    btnMoreInfo->setObjectName("btnMoreInfo");
    btnMoreInfo->setFixedSize(120,50);


    lbUserComment = new QLabel(detailArea);
    lbUserCommMark = new QLabel(detailArea);
    lbUserCommMark->setObjectName("lbUserCommMark");
    lbUserCommStar = new QLabel[5];
    for(int i=0;i<5;i++){
        lbUserCommStar[i].resize(16,16);
        lbUserCommStar[i].setPixmap(QPixmap(":/image/star_0.png"));
    }

    lbTotalCommenters = new QLabel(detailArea);
    lbStarLevel = new QLabel[25];

    pbStarLevel = new QProgressBar[5];

    lbLevel1Users = new QLabel(detailArea);
    lbLevel2Users = new QLabel(detailArea);
    lbLevel3Users = new QLabel(detailArea);
    lbLevel4Users = new QLabel(detailArea);
    lbLevel5Users = new QLabel(detailArea);

    lbMyMark = new QLabel(detailArea);
    btnMyStarLevel = new QPushButton[5];


    initCommentStar();


    //    lbmyStarLevel = new QLabel(this);
    lbmyComment = new QLabel(detailArea);
    lbmyComment->setFixedWidth(30);
    teCommContent = new QTextEdit(detailArea);
    teCommContent->setFixedHeight(200);
    teCommContent->setObjectName("teCommContent");
    teCommContent->setPlaceholderText(tr("Please Put Your Comment Here..."));
    btnSubmit = new QPushButton(tr("Submit"),detailArea);
    //    btnSubmit->setFixedSize(80,32);
    btnSubmit->setObjectName("btnSubmit");
    btnCancel = new QPushButton(tr("Cancel"),detailArea);
    btnCancel->setObjectName("btnCancel");
    //    btnCancel->setFixedSize(80,32);
    connect(btnSubmit,SIGNAL(clicked()),this,SLOT(submitComment()));
    connect(btnCancel,SIGNAL(clicked()),this,SLOT(cancelComment()));


    qmlEnginer = new QQmlEngine;
    qmlEnginer1 = new QQmlEngine;
    imgBoxer = new QQuickWidget(qmlEnginer1,detailArea);
    ctx1 = imgBoxer->rootContext();
    ctx1->setContextProperty("sImgData",screenImgData);
    imgBoxer->setSource(QUrl("qrc:/Imgboxer.qml"));
    imgBoxer->setMinimumHeight(320);
//    imgBoxer->setFixedHeight(320);


    userCommBoxer = new QQuickWidget(qmlEnginer,detailArea);
    ctx = userCommBoxer->rootContext();
    ctx->setContextProperty("commModel",commentModel);
    userCommBoxer->setSource(QUrl("qrc:/Comment.qml"));
    userCommBoxer->setFixedHeight(920);
    userCommBoxer->setMaximumWidth(1200);


    spItem = new QSpacerItem(100,200,QSizePolicy::Expanding,QSizePolicy::Expanding);


    hly1 = new QHBoxLayout;
    hly2 = new QHBoxLayout;
    hly3 = new QHBoxLayout;
    hly4 = new QHBoxLayout;
    hly5 = new QHBoxLayout;
    hly6 = new QHBoxLayout;
    hly7 = new QHBoxLayout;
    hly8 = new QHBoxLayout;
    hly9 = new QHBoxLayout;
    hly10 = new QHBoxLayout;
    hly11 = new QHBoxLayout;
    hly12 = new QHBoxLayout;
    hly13 = new QHBoxLayout;
    hly14 = new QHBoxLayout;
    hly15 = new QHBoxLayout;
    hly16 = new QHBoxLayout;
    hly17 = new QHBoxLayout;
    vly1 = new QVBoxLayout;
    vly2 = new QVBoxLayout;
    vly3 = new QVBoxLayout;
    vly4 = new QVBoxLayout;
    vly5 = new QVBoxLayout;
    vly6 = new QVBoxLayout;
    vly = new QVBoxLayout;
    gly = new QGridLayout;

}

void DetailWidget::initCommentStar()
{
    mStyStr0 = "QPushButton {border-image:url(:/image/star_0.png)}";
    mStyStr50 = "QPushButton {border-image:url(:/image/star_50.png)}";
    mStyStr100 = "QPushButton {border-image:url(:/image/star_100.png)}";

    connect(&btnMyStarLevel[0],SIGNAL(clicked()),this,SLOT(calMark0()));
    connect(&btnMyStarLevel[1],SIGNAL(clicked()),this,SLOT(calMark1()));
    connect(&btnMyStarLevel[2],SIGNAL(clicked()),this,SLOT(calMark2()));
    connect(&btnMyStarLevel[3],SIGNAL(clicked()),this,SLOT(calMark3()));
    connect(&btnMyStarLevel[4],SIGNAL(clicked()),this,SLOT(calMark4()));

    for(int i=0;i<5;i++){
        btnMyStarLevel[i].setStyleSheet(mStyStr0);
        btnMyStarLevel[i].setFixedSize(16,16);
    }

    //    for(int i=0;i<5;i++){
    //        btnMyStarLevel[i].setStyleSheet(mStyStr0);
    //        btnMyStarLevel[i].setFixedSize(16,16);
    //        QStateMachine *stMachine = new QStateMachine;

    //        QState *st0 = new QState();
    //        st0->assignProperty(&btnMyStarLevel[i],"styleSheet",mStyStr0);
    //        connect(st0,SIGNAL(propertiesAssigned()),this,SLOT(calMark0()));

    //        QState *st50 = new QState();
    //        st50->assignProperty(&btnMyStarLevel[i],"styleSheet",mStyStr50);
    //        connect(st50,SIGNAL(propertiesAssigned()),this,SLOT(calMark1()));

    //        QState *st100 = new QState();
    //        st100->assignProperty(&btnMyStarLevel[i],"styleSheet",mStyStr100);
    //        connect(st100,SIGNAL(propertiesAssigned()),this,SLOT(calMark2()));


    //        st0->addTransition(&btnMyStarLevel[i],SIGNAL(clicked()),st50);
    //        st50->addTransition(&btnMyStarLevel[i],SIGNAL(clicked()),st100);
    //        st100->addTransition(&btnMyStarLevel[i],SIGNAL(clicked()),st0);

    //        stMachine->addState(st0);
    //        stMachine->addState(st50);
    //        stMachine->addState(st100);
    //        stMachine->setInitialState(st0);
    //        stMachine->start();

    //    }
}

void DetailWidget::initCommProgressBar()
{
    for(int i=0;i<5;i++){
        pbStarLevel[i].setFixedSize(256,16);
        pbStarLevel[i].setValue(50);
        pbStarLevel[i].setTextVisible(false);
        pbStarLevel[i].setStyleSheet("QProgressBar {background-color:#ededed;border:none;}" "QProgressBar::chunk {background-color:#9e9e9e;}");
    }

}

void DetailWidget::refreshPage(int page)
{
    if( page == 5 ){
        toPage(mProductId);
    }
}

void DetailWidget::layoutWidgets()
{
    //part1

//    hly2->addWidget(lbComment);
    hly2->addWidget(lbCommentStar);
    hly2->addWidget(lbCommentStar+1);
    hly2->addWidget(lbCommentStar+2);
    hly2->addWidget(lbCommentStar+3);
    hly2->addWidget(lbCommentStar+4);

    QGridLayout *glya = new QGridLayout;
    glya->addWidget(lbComment,0,0);
    glya->addWidget(lbProductVersion,1,0);
    glya->addWidget(lbProductSize,2,0);
    glya->addLayout(hly2,0,1);
    glya->addWidget(lbVersionNo,1,1);
    glya->addWidget(lbSizeValue,2,1);
    glya->addWidget(lbCommentMark,0,2);
    glya->addItem(new QSpacerItem(lbCommentMark->width(),1),1,2);
    glya->addItem(new QSpacerItem(lbCommentMark->width(),1),1,2);

//    hly2->addWidget(lbCommentMark);

//    hly3->addWidget(lbProductVersion);
//    hly3->addWidget(lbVersionNo);
//    hly3->addSpacerItem(new QSpacerItem(50,1));

//    hly4->addWidget(lbProductSize);
//    hly4->addWidget(lbSizeValue);
//    hly4->addSpacerItem(new QSpacerItem(75,1));

    vly1->addWidget(lbProductName);
//    vly1->addLayout(hly2);
//    vly1->addLayout(hly3);
//    vly1->addLayout(hly4);
    vly1->addLayout(glya);

    hly1->addWidget(lbProductImg);
    hly1->addLayout(vly1);
    hly1->setAlignment(lbProductImg,Qt::AlignLeft);
    hly1->setAlignment(vly1,Qt::AlignRight);

    hly5->addLayout(hly1);
    hly5->addWidget(btnState);
    hly5->setAlignment(hly1,Qt::AlignLeft);
    hly5->setAlignment(btnState,Qt::AlignRight);

    //part2
    hly11->addWidget(lbAppInfo,Qt::AlignLeft);
    hly11->addWidget(btnMoreInfo,Qt::AlignRight);
    vly2->addLayout(hly11);
    vly2->addWidget(lbAppInfoDetail);

    //part3
    vly3->addWidget(lbUserComment);
    lbUserComment->setText(tr("User Comment Mark"));
    lbUserComment->setStyleSheet("QLabel {font:20px;color:#313131;}");
    vly3->addSpacerItem(new QSpacerItem(1,32));
    //------------------------------------------------------------------------
    vly4->addWidget(lbUserCommMark);
    hly10->addWidget(&lbUserCommStar[0]);
    hly10->addWidget(&lbUserCommStar[1]);
    hly10->addWidget(&lbUserCommStar[2]);
    hly10->addWidget(&lbUserCommStar[3]);
    hly10->addWidget(&lbUserCommStar[4]);

    vly4->addLayout(hly10);
    vly4->addWidget(lbTotalCommenters);
    lbTotalCommenters->setText("152525 people");

    //progress bar counts ----------------------------------------------
    for(int i=0;i<25;i++){
        lbStarLevel[i].setFixedSize(8,8);
    }

    for(int i=0;i<5;i++){
        lbStarLevel[i].setPixmap(QPixmap(":/image/star_0.png"));
        hly12->addWidget(&lbStarLevel[i]);
    }
    hly12->setContentsMargins(0,0,0,0);


    lbStarLevel[5].setText("");
    hly13->addWidget(&lbStarLevel[5]);
    for(int i=6;i<10;i++){
        lbStarLevel[i].setPixmap(QPixmap(":/image/star_0.png"));
        hly13->addWidget(&lbStarLevel[i]);
    }
    hly13->setContentsMargins(0,0,0,0);


    lbStarLevel[10].setText("");
    lbStarLevel[11].setText("");
    hly14->addWidget(&lbStarLevel[10]);
    hly14->addWidget(&lbStarLevel[11]);
    for(int i=12;i<15;i++){
        lbStarLevel[i].setPixmap(QPixmap(":/image/star_0.png"));
        hly14->addWidget(&lbStarLevel[i]);
    }

    hly14->setContentsMargins(0,0,0,0);


    lbStarLevel[15].setText("");
    lbStarLevel[16].setText("");
    lbStarLevel[17].setText("");
    hly15->addWidget(&lbStarLevel[15]);
    hly15->addWidget(&lbStarLevel[16]);
    hly15->addWidget(&lbStarLevel[17]);
    for(int i=18;i<20;i++){
        lbStarLevel[i].setPixmap(QPixmap(":/image/star_0.png"));
        hly15->addWidget(&lbStarLevel[i]);
    }
    hly15->setContentsMargins(0,0,0,0);


    for(int i=20;i<24;i++){
        lbStarLevel[i].setText("");
        hly16->addWidget(&lbStarLevel[i]);
    }
    lbStarLevel[24].setPixmap(QPixmap(":/image/star_0.png"));
    hly16->addWidget(&lbStarLevel[24]);
    hly16->setContentsMargins(0,0,0,0);


    initCommProgressBar();

    lbLevel1Users->setText("2289");
    lbLevel2Users->setText("2289");
    lbLevel3Users->setText("2289");
    lbLevel4Users->setText("2289");
    lbLevel5Users->setText("2289");
    lbLevel1Users->setStyleSheet("QLabel {font:14px;}");
    lbLevel2Users->setStyleSheet("QLabel {font:14px;}");
    lbLevel3Users->setStyleSheet("QLabel {font:14px;}");
    lbLevel4Users->setStyleSheet("QLabel {font:14px;}");
    lbLevel5Users->setStyleSheet("QLabel {font:14px;}");


    gly->setContentsMargins(0,0,0,0);

    gly->addLayout(hly12,0,0);
    gly->addWidget(&pbStarLevel[0],0,1);
    gly->addWidget(lbLevel1Users,0,2);

    gly->addLayout(hly13,1,0);
    gly->addWidget(&pbStarLevel[1],1,1);
    gly->addWidget(lbLevel2Users,1,2);

    gly->addLayout(hly14,2,0);
    gly->addWidget(&pbStarLevel[2],2,1);
    gly->addWidget(lbLevel3Users,2,2);

    gly->addLayout(hly15,3,0);
    gly->addWidget(&pbStarLevel[3],3,1);
    gly->addWidget(lbLevel4Users,3,2);

    gly->addLayout(hly16,4,0);
    gly->addWidget(&pbStarLevel[4],4,1);
    gly->addWidget(lbLevel5Users,4,2);

    //----------------------------------------------------


    hly7->addWidget(lbMyMark);
    lbMyMark->setText(tr("My Comment:"));
    lbMyMark->setStyleSheet("QLabel {font:14px;}");
    hly7->addWidget(&btnMyStarLevel[0]);
    hly7->addWidget(&btnMyStarLevel[1]);
    hly7->addWidget(&btnMyStarLevel[2]);
    hly7->addWidget(&btnMyStarLevel[3]);
    hly7->addWidget(&btnMyStarLevel[4]);
    hly7->addWidget(lbmyComment);
    lbmyComment->setText(tr("Very Bad"));
    lbmyComment->setStyleSheet("QLabel {font:14px;}");



    hly9->addLayout(vly4);
    hly9->addSpacerItem(new QSpacerItem(48,1));
    hly9->addLayout(gly);
    hly9->addLayout(hly7);

    vly3->addLayout(hly9);
    vly3->addSpacerItem(new QSpacerItem(1,32));
    //input comment area ----------------------------------------------
    vly5->addWidget(teCommContent);
    hly8->addWidget(btnSubmit);
    hly8->addWidget(btnCancel);
    vly5->addLayout(hly8);
    vly5->setAlignment(hly8,Qt::AlignRight);
    vly3->addLayout(vly5);
    vly3->addSpacerItem(new QSpacerItem(1,60));
    //
    vly->addLayout(hly5);
    vly->addSpacerItem(new QSpacerItem(1,20));
    vly->addWidget(imgBoxer);
    vly->addSpacerItem(new QSpacerItem(1,24));
    vly->addLayout(vly2);
    vly->addSpacerItem(new QSpacerItem(1,60));
    vly->addLayout(vly3);
    //    vly->addSpacerItem(new QSpacerItem(1,60));

    vly->addWidget(userCommBoxer);
//    vly->insertSpacerItem(-1,spItem);


    //    vly->setContentsMargins(16,0,16,0);
    vly->setContentsMargins(32,32,32,5);
    detailArea->setLayout(vly);

    vly6->addWidget(scrollArea);
    setLayout(vly6);

    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setWidget(detailArea);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->verticalScrollBar()->setValue(0);

}

//from mainwindow to this page,show the data
void DetailWidget::toPage(int productId)
{
    commentModel->clear();
    cancelComment();
    modelIndex = 0;
    mCounter = 0;
    int releaseAry[1] = {0};
    mReleaseId[0]=0;
    mReleaseId[1]=0;
    // reget comment argument: releaseId and the 25th commentId
    //    int releaseIds[2] = {1,1};
    mProductId = productId;
    screenImgData->setSproductId(productId);
    screenImgData->getScreenImg();

    for(auto item = shareData->classStrMap.begin();item != shareData->classStrMap.end();item++)
    {
        if(item.key() == productId ){
            shareData->updateStrMap.clear();
            releaseAry[0] = item.value().releaseId;
            mReleaseId[0] = item.value().releaseId;
            //            mCurrReleaseId = item.value().releaseId;
            mAppName = item.value().exeFile;
            //get updateStrMap
            jsonFunc->getUpdateRelease(releaseAry,1);
            getImage(item.value().proImage);
            productName = item.value().proName;
            lbProductName->setText(item.value().proName);
            lbCommentMark->setText(QString::number(item.value().proStar,'f',1)+tr("fen"));
            initStar(lbCommentStar,item.value().proStar);
            initStar(lbUserCommStar,item.value().proStar);
            lbUserCommMark->setText(QString::number(item.value().proStar,'f',1));
            mProstatus = item.value().proStatus;
            qDebug()<< "mProstatus===================="<<mProstatus;
            //1:下载 2:更新 3:打开 4:重新下载 5:重新更新 6:正在卸载 7:正在下载 8:正在更新 9:卸载
            qDebug()<<"item.value().proStatus==================="<<item.value().proStatus;
            switch(item.value().proStatus){
            case 1:
                btnState->setText(tr("Download"));
                btnState->setDisabled(false);
                //get updateStrMap
//                jsonFunc->getUpdateRelease(releaseAry,1);
                break;
            case 2:
                btnState->setText(tr("Update"));
                btnState->setDisabled(false);
                resolveNameToId(item.value().packageName);
                break;
            case 3:
                btnState->setText(tr("Open"));
                btnState->setDisabled(false);
                resolveNameToId(item.value().packageName);
                break;
            case 4:
                btnState->setText(tr("ReDownload"));
                btnState->setDisabled(false);
                //get updateStrMap
//                jsonFunc->getUpdateRelease(releaseAry,1);
                break;
            case 5:
                btnState->setText(tr("ReUpdate"));
                btnState->setDisabled(false);

                resolveNameToId(item.value().packageName);
                break;
            case 6:
                btnState->setText(tr("UnInstalling..."));
                resolveNameToId(item.value().packageName);
                btnState->setDisabled(true);
                break;
            case 7:
                btnState->setText(tr("DownLoading..."));
                btnState->setDisabled(true);
                //get updateStrMap
//                jsonFunc->getUpdateRelease(releaseAry,1);
                break;
            case 8:
                btnState->setText(tr("UpDating..."));
                resolveNameToId(item.value().packageName);
                btnState->setDisabled(true);
                break;
            default:
                break;
            }
            mAppDetailInfo = item.value().proDescription;
            setAppDetailInfo(mAppDetailInfo);

            //get User Comments
            jsonFunc->getComment(mReleaseId);

            //get User head image
            //jsonFunc->getUser();
            break;
        }
    }
}

// handle the elide app detail info
void DetailWidget::setAppDetailInfo(const QString& infoText)
{
    QFontMetrics infoFont(lbAppInfoDetail->font());
    qint32 iFontWidth = infoFont.width(infoText);
    QString detailText = infoText;
    if(iFontWidth/900 > 2){
        detailText = infoFont.elidedText(infoText,Qt::ElideRight,900);
        qint32 len = detailText.length();
        if(len > 900)
            for(int i=1;i<=len/900;i++){
                int n = i*900;
                detailText.insert(n,'\n');
            }
    }
    lbAppInfoDetail->setText(detailText);
}

// get updateStrMap data and show the packagesize
bool DetailWidget::updateJsonOk()
{
    auto item1 = shareData->updateStrMap.begin();

    if(item1.value().productId == mProductId)
    {
        //        lbSizeValue->setText(octetsToString(static_cast<qint64>(item1.value().packageSize)));
        if(mProstatus==1 || mProstatus==4 || mProstatus==7 )
            lbSizeValue->setText(transPackSize(item1.value().packageSize));
        else
            lbSizeValue->setText(tr("Calculating..."));
        lbVersionNo->setText( "V" + item1.value().version );
    }
    return true;
}


void DetailWidget::getImage(QString headUrl)
{
    imageManager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(headUrl));
    connect(imageManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
    imageManager->get(request);
}

void DetailWidget::replyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(bytes);
        lbProductImg->setPixmap(pixmap);

    }
}

void DetailWidget::initStar(QLabel* lbStar,double num)
{
    for(int i = 0;i < 5;i++)
    {
        lbStar[i].setFixedSize(10,10);
        lbStar[i].setPixmap(QPixmap(":/image/star_0.png"));
    }

    for(int i = 0;i < num;i++)
    {
        lbStar[i].setPixmap(QPixmap(":/image/star_100.png"));
    }

    if(num - qFloor(num) > 0)
    {
        lbStar[(int)num].setPixmap(QPixmap(":/image/star_50.png"));
    }
}

//Byte to KB,MB,GB translation
QString DetailWidget::octetsToString( qint64 octets )
{
#define THE_FACTOR 1024
    static const qint64 kb = THE_FACTOR;
    static const qint64 mb = THE_FACTOR * kb;
    static const qint64 gb = THE_FACTOR * mb;

    QString s;
    qreal value = octets;

    // Whether we care about decimals: only for GB and only
    // if it's less than 10 GB.
    bool round = true;

    if (octets >= gb) {
        s = QCoreApplication::translate("Size", "%L1 GB");
        value /= gb;
        round = false;
    } else if (octets >= mb) {
        s = QCoreApplication::translate("Size", "%L1 MB");
        value /= mb;
    } else if (octets >= kb) {
        s = QCoreApplication::translate("Size", "%L1 KB");
        value /= kb;
    } else  {
        s = QCoreApplication::translate("Size", "%L1 B");
    }

    if (value > 9.95)
        round = true;

    if (round)
        return s.arg(qRound(value));

    return s.arg(value, 0, 'f', 2);
}


QString DetailWidget::transPackSize(const double &size)
{
    double packSize = size;
    static QStringList measures;
    if (measures.isEmpty())
        measures << QCoreApplication::translate("QInstaller", "B")
                 << QCoreApplication::translate("QInstaller", "K")
                 << QCoreApplication::translate("QInstaller", "M")
                 << QCoreApplication::translate("QInstaller", "G")
                 << QCoreApplication::translate("QInstaller", "T");
    QStringListIterator it(measures);
    QString measure(it.next());
    while (packSize >= 1024.0 && it.hasNext()) {
        measure = it.next();
        packSize /= 1024.0;
    }
    return QString::fromLatin1("%1%2").arg(packSize, 0, 'f', 1).arg(measure);
}
//1:下载 2:更新 3:打开 4:重新下载 5:重新更新 6:正在卸载 7:正在下载 8:正在更新 9:卸载
void DetailWidget::updatePackageStatus(QString productName, bool bo,int flag)
{
    qDebug()<<"updatePackageStatus============================"<<endl;
    qDebug()<<"flag=========================="<<flag<<endl;
    qDebug()<< "productName=========================="<<productName<<endl;
    mProstatus = flag;
    for(auto item = shareData->classStrMap.begin(); item != shareData->classStrMap.end() ; item++ )
    {

        if(item.value().proName == productName){
            qDebug()<<"item.value().proName=========="<<item.value().proName;
            btnState->setDisabled(false);
            switch(flag){
            case 1://下载
                if(bo){
                    btnState->setText(tr("Open"));
                    mProstatus = 3;
                }else{
                    btnState->setText(tr("ReDownload"));
                    mProstatus = 4;
                }
                break;

            case 2://更新
                if(bo){
                    btnState->setText(tr("Open"));
                    mProstatus = 3;
                }else{
                    btnState->setText(tr("ReUpdate"));
                    mProstatus = 5;
                }
                break;
            case 3://打开
                //            if(bo){
                //                btnState->setText(tr("Open"));
                //            }else{
                //                btnState->setText(tr("ReDownload"));
                //            }
                break;
            case 4://重新下载
                if(bo){
                    btnState->setText(tr("Open"));
                    mProstatus = 3;
                }else{
                    btnState->setText(tr("ReDownload"));

                    mProstatus = 4;

                }
                break;
            case 5://重新更新
                if(bo){
                    btnState->setText(tr("Open"));
                    mProstatus = 3;
                }else{
                    btnState->setText(tr("ReUpdate"));
                    mProstatus = 5;

                }
                break;
            case 6://正在卸载
                if(bo){
                    btnState->setText(tr("Download"));
                    mProstatus = 1;

                }else{
                    btnState->setText(tr("Open"));
                    mProstatus = 3;
                }
                break;
            case 7://正在下载
                if(bo){
                    btnState->setText(tr("Open"));
                    mProstatus = 3;
                }else{
                    btnState->setText(tr("ReDownload"));
                    mProstatus = 4;

                }
                break;
            case 8://正在更新
                btnState->setText(tr("Open"));
                mProstatus = 3;

                //            if(bo){
                //                btnState->setText(tr("Open"));
                //            }else{
                //                btnState->setText(tr("ReUpdate"));
                //            }
                break;
            case 9://卸载
                if(bo){
                    btnState->setText(tr("Download"));
                    mProstatus = 1;

                }else{
                    btnState->setText(tr("Open"));
                    mProstatus = 3;
                }
                break;
            default:
                break;

            }
        }
    }
}


//1:下载 2:更新 3:打开 4:重新下载 5:重新更新 6:正在卸载 7:正在下载 8:正在更新 9:卸载
void DetailWidget::onBtnStateClicked()
{
    qDebug()<<__FUNCTION__<<"========================="<<endl;
    qDebug()<<"mProstatus========================="<<mProstatus<<endl;
    switch(mProstatus){
    case 1:
        emit sigDownload(productName,mReleaseId[0]);
        btnState->setText(tr("DownLoading..."));
        btnState->setDisabled(true);
        break;
    case 2:
        emit sigUpdate(productName,mReleaseId[0]);
        btnState->setText(tr("UpDating..."));
        btnState->setDisabled(true);
        break;
    case 3:
        openApp();
        break;
    case 4:
        btnState->setText(tr("DownLoading..."));
        emit sigDownload(productName,mReleaseId[0]);//redownload
        btnState->setDisabled(true);
        break;
    case 5:
        btnState->setText(tr("UpDating..."));
        emit sigUpdate(productName,mReleaseId[0]);//reupdate
        btnState->setDisabled(true);
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        btnState->setText(tr("DownLoading..."));
        emit sigDownload(productName,4);;//reupdate
        btnState->setDisabled(true);
        break;
    default:
        break;
    }
}

void DetailWidget::openApp()
{
    qDebug()<<__FUNCTION__<<"========================="<<endl;

    QProcess *proc = new QProcess(this);
    proc->start(mAppName);
}

void DetailWidget::passComment()
{
    //    auto item = shareData->commentMap.find(1);
    //    qDebug()<<"date ========= " << item.value().commentText << endl;
    //    COMMENTMAP tmpCmap;
    //    tmpCmap.begin().value().commentText = "test commenttext";
    //    QString date = shareData->commentMap.begin().value().commentText;
    //    qDebug()<<"date ========= " << date << endl;


}

void DetailWidget::submitComment()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QSettings address("/usr/share/emindappstore/ipaddress",QSettings::IniFormat);
    QString ipString = "http://" + address.value("Ip/address").toString() + ":8888/";

    //    QString ipString = "http://127.0.0.1:8888/";
    QString baseUrl = ipString +"comment";
    //    QString baseUrl = ipString +"user";
    QUrl url(baseUrl);
    QNetworkRequest request;
    //    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setUrl(url);

    //post comment format:
    //product_ID=$product_ID&release_ID=$release_ID&user_ID=$4&comment_text=$2&comment_grade=$3&comment_date=$4
    QByteArray dataArr;
    qint32 releaseId = mReleaseId[0];
    qint32 userId = 2;
    QString commentText;
    commentText = teCommContent->toPlainText();
    double commentGrade = calMark();
    QString commentDate = QDate::currentDate().toString("yyyy-MM-dd");
    dataArr.append(QString("release_ID=%1&user_ID=%2&comment_text=%3&comment_grade=%4&comment_date=%5").arg(releaseId).arg(userId).arg(commentText).arg(commentGrade).arg(commentDate));

    qDebug()<<"commentGrade======================"<<commentGrade;
    //post user add format:
    //user_name=$1&avatar_url=$2&mail=$3
    QString userName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).section("/",-1,-1);
    QString avatarUrl = "shotWell";
    QString mail = "1234@qq.com";
    //    dataArr.append(QString("user_name=%1&avatar_url=%2&mail=%3").arg(userName).arg(avatarUrl).arg(mail));
    qDebug()<<"dataArr========"<<dataArr<<endl;

    if(!commentText.isEmpty() && commentText.length() <= 200){
        manager->post(request,dataArr);
        teCommContent->clear();
        btnSubmit->clearFocus();
        QMessageBox::warning(this,tr("Tips"),tr("Comment successfully! Please Refresh!"));
    }else if(commentText.length() > 200){
        QMessageBox::warning(this,tr("Tips"),tr("Content's length is Larger than 200 characters !"));
    }else if(commentText.isEmpty()){
        QMessageBox::warning(this,tr("Tips"),tr("Content is Null!"));
    }

    resetMarkBtn();
}

void DetailWidget::resetMarkBtn()
{
    for(int i=0;i<5;i++){
        productMark = 0;
        btnMyStarLevel[i].setStyleSheet(mStyStr0);
        lbmyComment->setText(tr("Very Bad"));
    }
}

void DetailWidget::checkBtnState()
{

}

void DetailWidget::cancelComment()
{
    teCommContent->clear();
    resetMarkBtn();
}

//connect to jsonFunc's commentIsOk signal
void DetailWidget::commentJsonOk()
{
    qDebug()<<"CommentJsonOK====================="<<endl;
    mCommentSum = 0;
    qint32 pgNumber = 0;

    for(auto item = shareData->commentMap.begin();item != shareData->commentMap.end() ;item++ ){
        mCommentSum ++;
        jsonFunc->getUser(item.value().userId);

        if(mCommentSum == 25)
            commentModel->mCommentId = item.value().commentId;
    }

    commentModel->setCount(mCommentSum);
    pgNumber = qCeil(static_cast<qreal>(mCommentSum)/5.0);
    //    qDebug()<< "qCeil(static_cast<qreal>(mCommentSum)/5.0)============="<<qCeil(static_cast<qreal>(mCommentSum)/5.0)<<endl;
    if(mCommentSum > 25)
        pgNumber++;
    commentModel->setPageNumber(pgNumber);
}

//connect to jsonFunc's userIsOk signal
void DetailWidget::userJsonOk()
{
    mCounter++;
    //    qDebug()<<"mCounter================"<<mCounter<<endl;
    if(mCounter >= mCommentSum){
        loadData();
        mCounter = 0;
    }

}

void DetailWidget::loadData()
{
    for(auto item = shareData->commentMap.begin();item != shareData->commentMap.end() ;item++ ){
        commData.append(item.value().commentText);
        commData.append(QString::number(item.value().commentGrade));
        commData.append(item.value().CommentDate);
        commData.append(shareData->userMap.find(item.value().userId).value().userName);
        commData.append(shareData->userMap.find(item.value().userId).value().userAvatarURL);
        commentModel->insert(modelIndex,commData);
        commData.clear();
        modelIndex++;
    }
    shareData->commentMap.clear();
    shareData->userMap.clear();
}


void DetailWidget::screenImageJsonOk()
{
    auto item = shareData->screenImageMap.begin();
    jsonFunc->getScreenImage(item.value().imageId);
}

void DetailWidget::reGetComment(qint32 commId)
{
    mReleaseId[1] = commId;
    jsonFunc->getComment(mReleaseId);
}

void DetailWidget::createUserCommBox()
{
    userCommBoxer = new QQuickWidget(qmlEnginer,detailArea);
    ctx = userCommBoxer->rootContext();
    ctx->setContextProperty("commModel",commentModel);
    userCommBoxer->setSource(QUrl("qrc:/Comment.qml"));
    userCommBoxer->setMinimumHeight(910);
    userCommBoxer->setMaximumWidth(1200);
    vly->addWidget(userCommBoxer);
    vly->insertSpacerItem(-1,spItem);
}

//get product's current version comment
void DetailWidget::getCurrVerComm()
{
    commentModel->clear();
    //    mReleaseId[0]=mCurrReleaseId;
    mReleaseId[1]=0;
    jsonFunc->getComment(mReleaseId);
    refreshPage(5);
}

//get product's history version comment
void DetailWidget::getHisVerComm()
{
    commentModel->clear();
    //    mReleaseId[0]=mCurrReleaseId;
    qDebug()<<"mReleaseId[0]==================="<<mReleaseId[0]<<endl;
    mReleaseId[1]=0;
    jsonFunc->getCommentHistory(mReleaseId);
    refreshPage(5);
}

void DetailWidget::showMore(bool checked)
{
    if(checked){
        QFontMetrics infoFont(lbAppInfoDetail->font());
        qint32 iFontWidth = infoFont.width(mAppDetailInfo);
        qint32 infoLabelSize = iFontWidth/900;
        if(infoLabelSize > 2){
            qint32 len = mAppDetailInfo.length();
            if(len > 900)
                for(int i=1;i<=len/900;i++){
                    int n = i*900;
                    mAppDetailInfo.insert(n,'\n');
                }
            lbAppInfoDetail->setFixedHeight(infoLabelSize*20);
            lbAppInfoDetail->setText(mAppDetailInfo);

        }
    }else{
        lbAppInfoDetail->setFixedHeight(42);
        setAppDetailInfo(mAppDetailInfo);
    }
}

double DetailWidget::calMark()
{
    if(productMark<=1)
        lbmyComment->setText(tr("Very Bad"));
    else if (productMark>1 && productMark<=2)
        lbmyComment->setText(tr("Bad"));
    else if (productMark>2 && productMark<=3)
        lbmyComment->setText(tr("Normal"));
    else if (productMark>3 && productMark<=4)
        lbmyComment->setText(tr("Good"));
    else if (productMark>4 && productMark<=5)
        lbmyComment->setText(tr("Very Good"));

    qDebug()<<"productMark=========="<<productMark<<endl;
    return productMark;
}

void DetailWidget::calMark0()
{
    mBtnNumber = 0;
    if(btnMyStarLevel[0].styleSheet() == mStyStr0){
        productMark = 0.5;
        btnMyStarLevel[0].setStyleSheet(mStyStr50);
    }else if(btnMyStarLevel[0].styleSheet() == mStyStr50){
        productMark = 1;
        btnMyStarLevel[0].setStyleSheet(mStyStr100);
    }else if(btnMyStarLevel[0].styleSheet() == mStyStr100){
        productMark = 0;
        for(int i=0;i<5;i++)
            btnMyStarLevel[i].setStyleSheet(mStyStr0);
    }
    calMark();
}

void DetailWidget::calMark1()
{
    if(btnMyStarLevel[1].styleSheet() == mStyStr0){
        productMark = 1.5;
        btnMyStarLevel[1].setStyleSheet(mStyStr50);
        btnMyStarLevel[0].setStyleSheet(mStyStr100);
    }else if(btnMyStarLevel[1].styleSheet() == mStyStr50){
        productMark = 2;
        btnMyStarLevel[0].setStyleSheet(mStyStr100);
        btnMyStarLevel[1].setStyleSheet(mStyStr100);
    }else if(btnMyStarLevel[1].styleSheet() == mStyStr100){
        productMark = 1;
        for(int i=1;i<5;i++)
            btnMyStarLevel[i].setStyleSheet(mStyStr0);
    }
    calMark();

}

void DetailWidget::calMark2()
{
    if(btnMyStarLevel[2].styleSheet() == mStyStr0){
        productMark = 2.5;
        btnMyStarLevel[0].setStyleSheet(mStyStr100);
        btnMyStarLevel[1].setStyleSheet(mStyStr100);
        btnMyStarLevel[2].setStyleSheet(mStyStr50);
    }else if(btnMyStarLevel[2].styleSheet() == mStyStr50){
        productMark = 3;
        for(int i=0;i<3;i++)
            btnMyStarLevel[i].setStyleSheet(mStyStr100);
    }else if(btnMyStarLevel[2].styleSheet() == mStyStr100){
        productMark = 2;
        for(int i=2;i<5;i++)
            btnMyStarLevel[i].setStyleSheet(mStyStr0);
    }
    calMark();

}

void DetailWidget::calMark3()
{
    if(btnMyStarLevel[3].styleSheet() == mStyStr0){
        productMark = 3.5;
        btnMyStarLevel[3].setStyleSheet(mStyStr50);
        for(int i=0;i<3;i++)
            btnMyStarLevel[i].setStyleSheet(mStyStr100);
    }else if(btnMyStarLevel[3].styleSheet() == mStyStr50){
        productMark = 4;
        for(int i=0;i<4;i++)
            btnMyStarLevel[i].setStyleSheet(mStyStr100);
    }else if(btnMyStarLevel[3].styleSheet() == mStyStr100){
        productMark = 3;
        for(int i=3;i<5;i++)
            btnMyStarLevel[i].setStyleSheet(mStyStr0);
    }
    calMark();

}

void DetailWidget::calMark4()
{
    if(btnMyStarLevel[4].styleSheet() == mStyStr0){
        productMark = 4.5;
        btnMyStarLevel[4].setStyleSheet(mStyStr50);
        for(int i=0;i<4;i++)
            btnMyStarLevel[i].setStyleSheet(mStyStr100);
    }else if(btnMyStarLevel[4].styleSheet() == mStyStr50){
        productMark = 5;
        for(int i=0;i<5;i++)
            btnMyStarLevel[i].setStyleSheet(mStyStr100);
    }else if(btnMyStarLevel[4].styleSheet() == mStyStr100){
        productMark = 4;
        btnMyStarLevel[4].setStyleSheet(mStyStr0);
    }
    calMark();
}


void DetailWidget::resolveNameToId(QString packName)
{
    PackageKit::Transaction *transaction = PackageKit::Daemon::resolve(packName,
                                                                       PackageKit::Transaction::FilterArch);
    connect(transaction,
            SIGNAL(package(PackageKit::Transaction::Info,QString,QString)),
            SLOT(resetPackId(PackageKit::Transaction::Info,QString,QString)));

}
void DetailWidget::resetPackId(PackageKit::Transaction::Info, QString packageID, QString summar)
{
    qDebug() << "The new package is: " << packageID << summar;
    pkgId = packageID;
    setInstallSize(pkgId);
}


void DetailWidget::setInstallSize(QString pacId)
{
    detailTransaction = PackageKit::Daemon::getDetails(pacId);
    connect(detailTransaction,
            SIGNAL(details(const PackageKit::Details &)),
            this,
            SLOT(onGetDetails(const PackageKit::Details &)));
}

void DetailWidget::onGetDetails(const PackageKit::Details &value)
{
    double size = (double)value.size();
    qDebug()<<"onGetDetails size ============================"<<size<<endl;
    QString insSize = transPackSize(size);
    lbSizeValue->setText(insSize);
}
