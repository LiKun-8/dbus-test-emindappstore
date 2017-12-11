#include "softwarecenterwindow.h"
#include "titlebar.h"
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include "homewidget.h"
#include <QGridLayout>
#include "searchwidget.h"
#include "detailwidget.h"
#include <QScrollArea>
#include "ClassAndMorePage/classpage.h"
#include <QScrollBar>
#include <QSizePolicy>
#include "ClassAndMorePage/recommendwidget.h"
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QSettings>
#include <QMessageBox>
#include <QLockFile>
#include <QLockFile>
#include <QThread>
#include <QTimer>

//QLockFile SoftwareCenterWindow::lockFile("/tmp/lockFile");

SoftwareCenterWindow::SoftwareCenterWindow(QWidget *parent)
    : QWidget(parent)
{
    resize(980,640);
    vly = new QVBoxLayout;
    titleBar = new TitleBar(this);
    stackWidget = new QStackedWidget(this);

    /*cannot hide the title and icon ?????????*/
    flags = 0;
    flags |= Qt::CustomizeWindowHint;
    flags |= Qt::WindowMinimizeButtonHint;
    flags |= Qt::WindowMaximizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);


    setWindowIcon(QIcon(":/image/winIcon.png"));
    setWindowTitle(tr("EmindAppStore"));


    vly->setContentsMargins(0,0,0,0);
    vly->setSpacing(0);
    vly->addWidget(titleBar);
    vly->addWidget(stackWidget);
    this->setLayout(vly);

    setObjectName("mainWindow");
    stackWidget->setObjectName("stackWidget");

    pageIndex = 0;
    pageList.append(0);
    mOldIndex = 100;

    shareData = new ShareData();
    jsonFunc = new JSONFUNC(shareData);

    pageHome = Q_NULLPTR;
    initTitleBarConns();
    createGui();
    mConnFlag = false;
    onBtnHome();
//    this->setDisabled(true);

    qRegisterMetaType<QMap<QString, QString>>("QMap<QString, QString>");

}

void SoftwareCenterWindow::initTitleBarConns()
{
    connect(titleBar->btnHome,SIGNAL(clicked()),this,SLOT(onBtnHome()));
    connect(titleBar->btnClass,SIGNAL(clicked()),this,SLOT(onBtnClass()));
    connect(titleBar->btnUpdate,SIGNAL(clicked()),this,SLOT(onBtnUpdate()));
    connect(titleBar->btnManager,SIGNAL(clicked()),this,SLOT(onBtnManager()));
    connect(titleBar->leSearch,SIGNAL(returnPressed()),this,SLOT(toPageSearch()));
    connect(titleBar->btnNext,SIGNAL(clicked()),this,SLOT(nextPage()));
    connect(titleBar->btnBack,SIGNAL(clicked()),this,SLOT(backPage()));
    connect(titleBar->btnRefresh,SIGNAL(clicked()),this,SLOT(refreshPage()));
    connect(stackWidget,SIGNAL(currentChanged(int)),this,SLOT(onPageChanged(int)));
}

void SoftwareCenterWindow::delConnections()
{

}

void SoftwareCenterWindow::setWinStatus()
{
    this->setEnabled(true);
}

void SoftwareCenterWindow::createConnections()
{
    mConnFlag = true;
    connect(pageClass,SIGNAL(setMore()),this,SLOT(onBtnMore()));
    connect(this,SIGNAL(toSearch(QString)),pageSearch,SLOT(doSearch(QString)));
    connect(pageManager,SIGNAL(setInsdSizeOk()),this,SLOT(setBtnRefreshState()));
    connect(pageManager,SIGNAL(installStatusChanged()),this,SLOT(setWinStatus()));

    //refresh connections
//    connect(this,SIGNAL(refreshPg(int)),pageHome,SLOT(refreshPage(int)));
    connect(this,SIGNAL(refreshPg(int)),pageSearch,SLOT(refreshPage(int)));
    connect(this,SIGNAL(refreshPg(int)),pageDetail,SLOT(refreshPage(int)));

    connect(pageHome,SIGNAL(toDetailPage(int)),this,SLOT(toPageDetail(int)));
    connect(pageSearch,SIGNAL(toDetailSig(int)),this,SLOT(toPageDetail(int)));


    //pagehome connections
    connect(pageHome->recommWidget,SIGNAL(installApp(QString,int)),pageManager,SLOT(getInstallRlease(QString, int)),Qt::QueuedConnection);
    connect(pageHome->recommWidget,SIGNAL(updateApp(QString,int)),pageUpdate,SLOT(onUpBtnClicked(QString)),Qt::QueuedConnection);



    connect(pageManager,SIGNAL(sigInstallSuccess(QString,bool,int)),pageHome->recommWidget,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(sigReDownload(QString,bool,int)),pageHome->recommWidget,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(removePackageSuccess(QString,bool,int)),pageHome->recommWidget,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageManager,SIGNAL(sigUninstalling(QString,bool,int)),pageHome->recommWidget,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageUpdate,SIGNAL(updateStatusChanged()),pageHome->recommWidget,SLOT(resetStatus()));
    connect(pageManager,SIGNAL(installStatusChanged()),pageHome->recommWidget,SLOT(resetStatus()));

    connect(pageUpdate,SIGNAL(theUpdateApp(QString, QString, QString, QString, int, QString, QString, int)),pageManager,SLOT(onAppUpdate(QString, QString, QString, QString, int, QString, QString, int)));
    connect(pageUpdate,SIGNAL(appUpdateOk(QString, QString, QString, QString, QString, int)),pageManager,SLOT(onAppUpdateOk(QString)));
    connect(pageUpdate,SIGNAL(appUpdateFailure(QString)),pageManager,SLOT(onAppUpdateFailure(QString)));
    connect(pageUpdate,SIGNAL(appUpdateOk(QString, QString, QString ,QString, QString, int)),pageManager,SLOT(upInsToInsd(QString, QString, QString, QString, QString, int)));
    connect(pageUpdate,SIGNAL(updateStatusChanged()),pageClass,SLOT(resetStatus()));
    connect(pageUpdate,SIGNAL(updatePackageSuccess(QString,bool,int)),pageClass,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageUpdate,SIGNAL(updatePackageSuccess(QString,bool,int)),pageClass->moreClassWidget->showMore,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageUpdate,SIGNAL(updatePackageSuccess(QString,bool,int)),pageHome->recommWidget,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageUpdate,SIGNAL(updatePackage(QString,bool,int)),pageClass,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageUpdate,SIGNAL(updatePackage(QString,bool,int)),pageHome->recommWidget,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageUpdate,SIGNAL(updatePackage(QString,bool,int)),pageClass->moreClassWidget->showMore,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageUpdate,SIGNAL(detailspageSig(int)),this,SLOT(toPageDetail(int)));

    //class page software status connection with update page
    connect(pageManager,SIGNAL(upBtnClicked(QString)),pageUpdate,SLOT(onUpBtnClicked(QString)));
    connect(pageManager,SIGNAL(sigReDownload(QString,bool,int)),pageClass,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(sigInstallSuccess(QString,bool,int)),pageClass,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(sigInstallSuccess(QString,bool,int)),pageClass->moreClassWidget->showMore,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageManager,SIGNAL(removePackageSuccess(QString,bool,int)),pageClass,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageManager,SIGNAL(removePackageSuccess(QString,bool,int)),pageClass->moreClassWidget->showMore,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageManager,SIGNAL(installStatusChanged()),pageClass,SLOT(resetStatus()));
    connect(pageManager,SIGNAL(sigUninstalling(QString,bool,int)),pageClass,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageManager,SIGNAL(upBtnClicked(QString)),pageUpdate,SLOT(onUpBtnClicked(QString)));
    connect(pageManager,SIGNAL(detailspageSig(int)),this,SLOT(toPageDetail(int)));
    connect(pageManager,SIGNAL(toGetDetail(QMap<QString,INSTALLEDSTRUCT>)),this,SLOT(startPkDetailThread(QMap<QString,INSTALLEDSTRUCT>)));


    connect(pageClass,SIGNAL(installpackage(QString,int)),pageManager,SLOT(getInstallRlease(QString, int)),Qt::QueuedConnection);
    connect(pageClass,SIGNAL(updatePackage(QString,int)),pageUpdate,SLOT(onUpBtnClicked(QString)),Qt::QueuedConnection);
    connect(pageClass->moreClassWidget,SIGNAL(installpackage(QString,int)),pageManager,SLOT(getInstallRlease(QString,int)),Qt::QueuedConnection);
    connect(pageClass,SIGNAL(detailspageSig(int)),this,SLOT(toPageDetail(int)));
    connect(pageClass->moreClassWidget,SIGNAL(detailspageSig(int)),this,SLOT(toPageDetail(int)));
    connect(pageClass->moreClassWidget,SIGNAL(updatePackage(QString,int)),pageUpdate,SLOT(onUpBtnClicked(QString)),Qt::QueuedConnection);

    connect(pageDetail,SIGNAL(sigDownload(QString,int)),pageClass,SLOT(updatePkgStatus(QString)),Qt::QueuedConnection);
    connect(pageSearch,SIGNAL(installApp(QString,int)),pageClass,SLOT(updatePkgStatus(QString)),Qt::QueuedConnection);
    connect(pageHome->recommWidget,SIGNAL(installApp(QString,int)),pageClass,SLOT(updatePkgStatus(QString)),Qt::QueuedConnection);
    connect(pageClass->moreClassWidget,SIGNAL(installpackage(QString,int)),pageClass,SLOT(updatePkgStatus(QString)),Qt::QueuedConnection);
//    connect(pageManager,SIGNAL(sigReDownloadIng(QString,bool,int)),pageClass,SLOT(updatePackageStatus(QString,bool,int)));

    connect(pageDetail,SIGNAL(sigDownload(QString,int)),pageClass->moreClassWidget->showMore,SLOT(updateMorePkgStatus(QString)),Qt::QueuedConnection);
    connect(pageSearch,SIGNAL(installApp(QString,int)),pageClass->moreClassWidget->showMore,SLOT(updateMorePkgStatus(QString)),Qt::QueuedConnection);
    connect(pageHome->recommWidget,SIGNAL(installApp(QString,int)),pageClass->moreClassWidget->showMore,SLOT(updateMorePkgStatus(QString)),Qt::QueuedConnection);
    connect(pageClass,SIGNAL(installpackage(QString,int)),pageClass->moreClassWidget->showMore,SLOT(updateMorePkgStatus(QString)),Qt::QueuedConnection);
//    connect(pageManager,SIGNAL(sigReDownloadIng(QString,bool,int)),pageClass->moreClassWidget->showMore,SLOT(updateMorePkgStatus(QString)));

    connect(pageDetail,SIGNAL(sigDownload(QString,int)),pageHome->recommWidget,SLOT(updatePkgStatus(QString,int)),Qt::QueuedConnection);
    connect(pageSearch,SIGNAL(installApp(QString,int)),pageHome->recommWidget,SLOT(updatePkgStatus(QString,int)),Qt::QueuedConnection);
    connect(pageClass,SIGNAL(installpackage(QString,int)),pageHome->recommWidget,SLOT(updatePkgStatus(QString,int)),Qt::QueuedConnection);
    connect(pageClass->moreClassWidget,SIGNAL(installpackage(QString,int)),pageHome->recommWidget,SLOT(updatePkgStatus(QString,int)),Qt::QueuedConnection);
//    connect(pageManager,SIGNAL(sigReDownloadIng(QString,bool,int)),pageHome->recommWidget,SLOT(updatePackageStatus(QString,bool,int)));

    //Detail page software status connection with update page

    connect(pageManager,SIGNAL(sigReUpdate(QString,bool,int)),pageDetail,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(sigReDownload(QString,bool,int)),pageDetail,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageUpdate,SIGNAL(sigUpdateSuccess(QString,bool,int)),pageDetail,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(sigInstallSuccess(QString,bool,int)),pageDetail,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(removePackageSuccess(QString,bool,int)),pageDetail,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageDetail,SIGNAL(updatePackage(QString,int)),pageUpdate,SLOT(onUpBtnClicked(QString)),Qt::QueuedConnection);
    connect(pageDetail,SIGNAL(sigDownload(QString,int)),pageManager,SLOT(getInstallRlease(QString, int)),Qt::QueuedConnection);
    connect(pageDetail,SIGNAL(sigUpdate(QString,int)),pageUpdate,SLOT(onUpBtnClicked(QString)),Qt::QueuedConnection);



    //Search page software status connection with update page

    connect(pageManager,SIGNAL(sigReUpdate(QString,bool,int)),pageSearch,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(sigReDownload(QString,bool,int)),pageSearch,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageUpdate,SIGNAL(sigUpdateSuccess(QString,bool,int)),pageSearch,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(sigInstallSuccess(QString,bool,int)),pageSearch,SLOT(updatePackageStatus(QString,bool,int)),Qt::QueuedConnection);
    connect(pageManager,SIGNAL(removePackageSuccess(QString,bool,int)),pageSearch,SLOT(updatePackageStatus(QString,bool,int)));
    connect(pageSearch,SIGNAL(installApp(QString,int)),pageManager,SLOT(getInstallRlease(QString, int)),Qt::QueuedConnection);
    //    connect(pageSearch,SIGNAL(sigDownload(QString,int)),pageManager,SLOT(getInstallRlease(QString, int)),Qt::QueuedConnection);
    connect(pageSearch,SIGNAL(updateApp(QString,int)),pageUpdate,SLOT(onUpBtnClicked(QString)),Qt::QueuedConnection);

    // thread to get pkgSize
    connect(this,SIGNAL(getPkgSizeOk(QMap<QString,QString>)),pageManager,SLOT(setInsdSize(QMap<QString, QString>)));

//    onBtnHome();
    stackWidget->setCurrentIndex(mLastPage);
    qDebug()<<"mLastPage=========================================================="<<mLastPage;
}

SoftwareCenterWindow::~SoftwareCenterWindow()
{
    //    delete shareData;
    //    delete jsonFunc;
}

void SoftwareCenterWindow::onBtnHome()
{
    stackWidget->setCurrentWidget(pageHome);
    titleBar->changeBtnHome();
    mLastPage = stackWidget->currentIndex();

}

void SoftwareCenterWindow::onBtnClass()
{
    stackWidget->setCurrentWidget(pageClass);
    mLastPage = stackWidget->currentIndex();

}

void SoftwareCenterWindow::onBtnMore()
{
    stackWidget->setCurrentWidget(pageClass->moreClassWidget);
    mLastPage = stackWidget->currentIndex();


}

void SoftwareCenterWindow::onBtnUpdate()
{
    stackWidget->setCurrentWidget(pageUpdate);
    mLastPage = stackWidget->currentIndex();
}

void SoftwareCenterWindow::onBtnManager()
{
    stackWidget->setCurrentWidget(pageManager);
    mLastPage = stackWidget->currentIndex();

}

void SoftwareCenterWindow::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    setFocus();
}

void SoftwareCenterWindow::toPageSearch()
{
    if(!titleBar->leSearch->text().isEmpty()){
        stackWidget->setCurrentWidget(pageSearch);
        emit toSearch(titleBar->leSearch->text());
        titleBar->setBtnNormal();
    }
}

void SoftwareCenterWindow::toPageDetail(int productId)
{
    qDebug()<<"++++++++++++++ productId="<<productId<<endl;
    stackWidget->setCurrentWidget(pageDetail);
    pageDetail->toPage(productId);
    titleBar->setBtnNormal();
}

void SoftwareCenterWindow::nextPage()
{
    qDebug()<<__FUNCTION__<<"pagelist.count()="<<pageList.count()<<endl;

    if(pageIndex < pageList.count()-1){
        pageIndex++;
        qDebug()<<"pageIndex="<<pageIndex<<endl;
        disconnect(stackWidget,SIGNAL(currentChanged(int)),this,SLOT(onPageChanged(int)));
        stackWidget->setCurrentIndex(pageList.at(pageIndex));
        //        pageList.removeLast();
        connect(stackWidget,SIGNAL(currentChanged(int)),this,SLOT(onPageChanged(int)));
    }

    if(pageIndex == pageList.count() - 1 ){
        titleBar->btnNext->setStyleSheet(
                    "QPushButton {border-image:url(:/image/forward_gray.png);}"
                    );
        titleBar->btnNext->setDisabled(true);
    }

    if(pageIndex > 0){
        titleBar->btnBack->setDisabled(false);
        titleBar->btnBack->setStyleSheet(
                    "QPushButton {border-image:url(:/image/return_black.png);}"
                    );
    }

    showSearchImg();

}

void SoftwareCenterWindow::backPage()
{
    qDebug()<<__FUNCTION__<<"pagelist.count()="<<pageList.count()<<endl;
    qDebug()<<"pageIndex="<<pageIndex<<endl;
    if(pageIndex > 0){
        pageIndex--;
        qDebug()<<"pageIndex="<<pageIndex<<endl;
        disconnect(stackWidget,SIGNAL(currentChanged(int)),this,SLOT(onPageChanged(int)));
        stackWidget->setCurrentIndex(pageList.at(pageIndex));
        if(!titleBar->btnNext->isEnabled()){
            titleBar->btnNext->setDisabled(false);
            titleBar->btnNext->setStyleSheet(
                        "QPushButton {border-image:url(:/image/forward_black.png);}"
                        );

        }

        connect(stackWidget,SIGNAL(currentChanged(int)),this,SLOT(onPageChanged(int)));

    }
    if(pageIndex == 0){
        titleBar->btnBack->setStyleSheet(
                    "QPushButton {border-image:url(:/image/return_gray.png);}"
                    );
        titleBar->btnBack->setDisabled(true);
    }

    showSearchImg();
}

void SoftwareCenterWindow::refreshPage()
{
    qDebug() << __FUNCTION__;
    if(stackWidget->currentIndex() == 4 )
        emit refreshPg(4);
    else if(stackWidget->currentIndex() == 5 )
        emit refreshPg(5);
    else if(stackWidget->currentIndex() == 6 ){

    }
    else
        checkServer();
    refreshFlag++;

}


void SoftwareCenterWindow::onPageChanged(int index)
{
    qDebug()<<__FUNCTION__<<"page changed index = "<< index <<endl;
    //    scrollArea->verticalScrollBar()->setValue(0);
    if(mOldIndex != index){
        pageList.append(index);
        pageIndex = pageList.count()-1;
    }else{
        pageIndex = pageList.count()-1;
    }

    mOldIndex = index;

    qDebug()<<"pageIndex="<<pageIndex<<endl;
    qDebug()<<"pagelist.count()="<<pageList.count()<<endl;
    if(pageList.count() <= 1){
        titleBar->btnNext->setStyleSheet(
                    "QPushButton {border-image:url(:/image/forward_gray.png);}");
        titleBar->btnBack->setDisabled(true);
        titleBar->btnNext->setDisabled(true);

    }else{
        titleBar->btnBack->setDisabled(false);
        titleBar->btnBack->setStyleSheet(
                    "QPushButton {border-image:url(:/image/return_black.png);}"
                    );

    }

    showSearchImg();

}

void SoftwareCenterWindow::showSearchImg()
{
    if(stackWidget->currentWidget() != pageSearch ){
        titleBar->leSearch->setText("");
        titleBar->setEnabled(false);
        titleBar->setEnabled(true);
        titleBar->lbSearchImg->show();
        titleBar->lbSearch->show();
    }
}

void SoftwareCenterWindow::checkServer()
{
    //    setBtnRefreshState();
    titleBar->btnRefresh->setDisabled(true);
    titleBar->btnRefresh->setStyleSheet("QPushButton {border-image:url(:/image/refresh_gray.png);}");
    qDebug()<<"checking server--------------------------------------";
    QSettings address("/usr/share/emindappstore/ipaddress",QSettings::IniFormat);
    QString ipaddress = "http://" + address.value("Ip/address").toString() + ":8888/";
    QNetworkAccessManager *netMng = new QNetworkAccessManager;
    QNetworkRequest netRequest;
    connect(netMng,SIGNAL(finished(QNetworkReply*)),this,SLOT(requestFinished(QNetworkReply*)));

    netRequest.setUrl(QUrl(ipaddress));
    netRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    netMng->get(netRequest);
}

void SoftwareCenterWindow::requestFinished(QNetworkReply* reply)
{
    //    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(reply->error()){
        //        QString testStr = reply->errorString();
        mConnState = 1;
        QMessageBox::warning(this,tr("Tips"),tr("Server Connection Error!"));
        setBtnRefreshState();
        return;
    }else{
        mConnState = 0;
        qDebug()<<"connect success---------------------------";
    }

    // after reconnect to server
    if( mConnState == 0){
        qDebug()<<"after reconnect to server--------------------============"<<stackWidget->count();
        if(stackWidget->count()>0){
            stackWidget->removeWidget(pageHome); //0
            stackWidget->removeWidget(pageClass->moreClassWidget);//6
            stackWidget->removeWidget(pageClass); //1
            stackWidget->removeWidget(pageUpdate);//2
            stackWidget->removeWidget(pageManager);//3
            stackWidget->removeWidget(pageSearch);//4
            stackWidget->removeWidget(pageDetail);//5
        }

        if(pageHome){
            delete pageHome;
            pageHome = Q_NULLPTR;
        }
        if(pageClass)
        {
            delete pageClass;
            pageClass = NULL;
        }

        if(pageUpdate)
        {
            delete pageUpdate;
            pageUpdate = NULL;
        }
        if(pageManager)
        {
            delete pageManager;
            pageManager = NULL;
        }
        if(pageSearch)
        {
            delete pageSearch;
            pageSearch = NULL;
        }
        if(pageDetail)
        {
            delete pageDetail;
            pageDetail = NULL;
        }

        createGui();
    }

    qDebug()<<"after reconnect to server--------------------"<<stackWidget->count();

}

void SoftwareCenterWindow::startPkDetailThread(QMap<QString, INSTALLEDSTRUCT> insdMap)
{
    //    qDebug() << __FUNCTION__;
    QStringList pkgIdList;
    QMap<QString,INSTALLEDSTRUCT>::iterator item;
    QString pkgId;
    for(item = insdMap.begin(); item != insdMap.end(); item++)
    {
        pkgId = item.key();
        pkgIdList.append(pkgId);
    }

    pkthread = new QThread();
    pkDetailThread = new PkDetailThread(0,pkgIdList);
    pkDetailThread->moveToThread(pkthread);
    connect(pkthread,SIGNAL(finished()),pkDetailThread,SLOT(deleteLater()));
    connect(pkDetailThread,SIGNAL(getPkgSizeOk(QMap<QString, QString>)),this,SLOT(sendPkgSize(QMap<QString, QString>)),Qt::QueuedConnection);
    connect(pkthread,SIGNAL(started()),pkDetailThread,SLOT(toGetSize()));
    pkthread->start();

}


void SoftwareCenterWindow::sendPkgSize(QMap<QString, QString> insdSizeMap)
{
//    qDebug() << __FUNCTION__;
    emit getPkgSizeOk(insdSizeMap);
}

void SoftwareCenterWindow::setBtnRefreshState()
{
    if(!titleBar->btnRefresh->isEnabled()){
        titleBar->btnRefresh->setEnabled(true);
        titleBar->btnRefresh->setStyleSheet("QPushButton {border-image:url(:/image/refresh_black.png);}");
    }

}

void SoftwareCenterWindow::createGui()
{
    pageHome = new HomeWidget(this,shareData,jsonFunc);
    pageClass = new ClassPage(this,jsonFunc,shareData);
    pageUpdate = new UpdatePage(this,jsonFunc,shareData);
    pageManager = new ManagerPage(this,jsonFunc,shareData);
    pageSearch = new SearchWidget(this,shareData,jsonFunc);
    pageDetail = new DetailWidget(this,shareData,jsonFunc);


    stackWidget->addWidget(pageHome); //0
    stackWidget->addWidget(pageClass); //1
    stackWidget->addWidget(pageUpdate);//2
    stackWidget->addWidget(pageManager);//3
    stackWidget->addWidget(pageSearch);//4
    stackWidget->addWidget(pageDetail);//5
    stackWidget->addWidget(pageClass->moreClassWidget);//6
//    qDebug()<<"stackWidget.count================================="<<stackWidget->count();

    createConnections();
}

void SoftwareCenterWindow::closeEvent(QCloseEvent* event)
{
    qDebug()<<"closing..........................";
    //    lockFile.unlock();
    //    exit(0);
}
