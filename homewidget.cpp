#include "homewidget.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QTableView>
#include <QQuickWidget>
#include <QQmlEngine>
#include <QScrollArea>
#include "ClassAndMorePage/recommendwidget.h"
#include "sharedata.h"
#include <QScrollArea>
#include <QScrollBar>
#include "datamanager.h"
#include <QQmlContext>

HomeWidget::HomeWidget(QWidget *parent,ShareData *data,JSONFUNC *jdata) : QWidget(parent)
{
    homeArea = new QWidget(this);
//    homeArea->resize(980,1200);
    homeArea->setMaximumWidth(1200);
    //width??
    homeArea->setMinimumSize(950,1050);
    homeArea->setObjectName("homeArea");

    scrollArea  = new QScrollArea(this);

    vly = new QVBoxLayout;
    vly1 = new QVBoxLayout;

    shareData = data;
    jsonFunc = jdata;
    recommWidget = new RecommendWidget(homeArea,data);
    connect(recommWidget,SIGNAL(toDetailSig(int)),this,SIGNAL(toDetailPage(int)));
    connect(jsonFunc,SIGNAL(recommendIsOk()),this,SLOT(setElement()));
    connect(jsonFunc,SIGNAL(bannerIsOk()),this,SLOT(bannerJsonOk()));


    bannerImgData = new DataManager;
    qmlEngine = new QQmlEngine;

//    imgBox = new QQuickWidget(qmlEngine,homeArea);
//    imgBox->setResizeMode(QQuickWidget::SizeRootObjectToView);
//    imgBox->setSource(QUrl("qrc:/Imgbox.qml"));
//    ctx1 = imgBox->rootContext();

    bannerView = new QQuickView();
    bannerWidget = QWidget::createWindowContainer(bannerView,homeArea);
    bannerView->setResizeMode(QQuickView::SizeRootObjectToView);
    bannerWidget->setMaximumSize(QSize(1200,236));
    bannerWidget->setMinimumSize(QSize(1000,236));

    ctx1 = bannerView->rootContext();

    ctx1->setContextProperty("BannerData",bannerImgData);
    //    imgBox = new QQuickWidget(homeArea);
    //    imgBox->setMaximumSize(QSize(1200,236));
    //    imgBox->setMinimumWidth(1000);


    connect(bannerImgData,SIGNAL(sigToDetailPg(int)),this,SIGNAL(toDetailPage(int)));

    bannerView->setSource(QUrl("qrc:/Imgbox.qml"));



//    hly2 = new QHBoxLayout;
//    hly2->addStretch(0);
//    hly2->addWidget(imgBox);
//    hly2->addStretch(0);
//    vly->addLayout(hly2);

//    vly->addWidget(imgBox);
//    vly->setAlignment(imgBox,Qt::AlignCenter);
    vly->addWidget(bannerWidget);
    vly->setAlignment(bannerWidget,Qt::AlignCenter);

    vly->addWidget(recommWidget);
    vly->setContentsMargins(0,0,0,0);
    homeArea->setLayout(vly);

    vly1->addWidget(scrollArea);
    setLayout(vly1);

    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setWidget(homeArea);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->verticalScrollBar()->setValue(0);



}

void HomeWidget::refreshPage(int page)
{
    if(page == 0)
        setElement();
}

void HomeWidget::setElement()
{
    recommWidget->setElement(shareData->classStrMap,shareData->recommendMap);
}

void HomeWidget::bannerJsonOk()
{
    auto item = shareData->bannerMap.begin();
//    jsonFunc->getBanner();
}

HomeWidget::~HomeWidget()
{
//    delete bannerImgData;
//    delete ctx1;
//    delete qmlEngine;
//    delete homeArea;
//    delete scrollArea;

}

void HomeWidget::mousePressEvent(QMouseEvent* event)
{
    setFocus();
}


