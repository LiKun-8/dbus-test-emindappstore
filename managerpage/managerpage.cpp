#include <QHeaderView>
#include <QSettings>
#include <QDir>
#include "managerpage.h"

ManagerPage::ManagerPage(QWidget *parent, JSONFUNC *json ,ShareData *sharedata) : QWidget(parent)
{
    wid = NULL;
    vboxLayout = NULL;
    vboxLayout1 = NULL;
    vboxLayout2 = NULL;
    manTaskBar = NULL;
    manCompBar = NULL;
    installTable = NULL;
    compTable = NULL;
    splitLabel = NULL;
    compManager = NULL;
    manScroArea = NULL;
    updm = NULL;
    updpg = NULL;
    shareData = NULL;
    jsonFunc = NULL;

    updm = new PkUpdates(this,json,sharedata);
    shareData = new ShareData();
    jsonFunc = new JSONFUNC(shareData);
    connect(json,SIGNAL(jsonError()),this,SLOT(readInsdConfig()));
    connect(json,SIGNAL(productIsOk()),this,SLOT(getPackageInstalled()));
    connect(updm,SIGNAL(getInsFinished(QMap<QString,INSTALLEDSTRUCT>)),this,SLOT(onGetInsFinished(QMap<QString,INSTALLEDSTRUCT>)));
    connect(jsonFunc,SIGNAL(updateIsOk()),this,SLOT(onAppInstall()));
    connect(this,SIGNAL(appInstallOk(QString, QString, QString,QString,QString,int)),this,SLOT(upInsToInsd(QString, QString, QString,QString,QString,int)));
    connect(updm,SIGNAL(installStatusChanged()),this,SLOT(onStatusChanged()));
    connect(this,SIGNAL(insBtnClicked(QString,int)),this,SLOT(getInstallRlease(QString, int)),Qt::QueuedConnection);
    connect(this,SIGNAL(insBtnClicked(QString,int)),this,SLOT(sendSigReDownload(QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(upBtnClicked(QString)),this,SLOT(sendSigReUpdate(QString)),Qt::QueuedConnection);

    createManagerWindow();
}

ManagerPage::~ManagerPage()
{

}

void ManagerPage::createManagerWindow()
{
    manScroArea = new QScrollArea(this);
    manScroArea->resize(this->width(),this->height());
//    manScroArea->setMaximumWidth(1200);
    manScroArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    manScroArea->setFrameShape(QFrame::NoFrame);
    manScroArea->setAlignment(Qt::AlignHCenter);

    QString strtask = tr("Tasks In Progress");
    QString stronekey =  tr("All Start");
    wid = new QWidget();
    manTaskBar = new TaskBar(this,strtask,stronekey);
    connect(manTaskBar->getOnekeyBtn(),SIGNAL(clicked()),this,SLOT(allStartBtnclicked()));

    //    manTaskBar->size().setHeight(64);

    installTable = new QTableWidget();
    installTable->setColumnCount(1);
    installTable->setRowCount(0);
    installTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    installTable->setFrameShape(QFrame::NoFrame);
    installTable->setSelectionMode(QAbstractItemView::NoSelection);
    installTable->setFocusPolicy(Qt::NoFocus);
    installTable->resizeColumnToContents (0);
    installTable->setShowGrid(false);

    installTable->verticalHeader()->setDefaultSectionSize(96);
    installTable->horizontalHeader()->setStretchLastSection(true);
    QHeaderView* vheaderView = installTable->verticalHeader();
    vheaderView->setHidden(true);
    QHeaderView *hheaderView = installTable->horizontalHeader();
    hheaderView->setHidden(true);

    //    int rowCount = installTable->rowCount();
    //    installTable->setMinimumHeight(96*rowCount);

    splitLabel = new QLabel();
    splitLabel->size().setHeight(16);
    splitLabel->setStyleSheet("background: #f9f9f9;");

    QString strtask1 = tr("Completed Tasks ");
    QString stronekey1 =  "";
    manCompBar = new TaskBar(this,strtask1,stronekey1);
    manCompBar->hideOnekeyButton();
    manCompBar->size().setHeight(64);

    compTable = new QTableWidget();
    compTable->setColumnCount(1);
    //    compTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    compTable->setFrameShape(QFrame::NoFrame);
    compTable->setSelectionMode(QAbstractItemView::NoSelection);
    compTable->setFocusPolicy(Qt::NoFocus);
    compTable->resizeColumnToContents (0);
    compTable->setShowGrid(false);
    compTable->verticalHeader()->setDefaultSectionSize(96);
    compTable->horizontalHeader()->setStretchLastSection(true);
    QHeaderView* vheaderView1 = compTable->verticalHeader();
    vheaderView1->setHidden(true);
    QHeaderView *hheaderView1 = compTable->horizontalHeader();
    hheaderView1->setHidden(true);

    vboxLayout = new QVBoxLayout();
    vboxLayout->setMargin(0);
    vboxLayout1 = new QVBoxLayout();
    vboxLayout1->setContentsMargins(16,0,16,0);
    vboxLayout2 = new QVBoxLayout();
    vboxLayout2->setContentsMargins(16,0,16,0);

    vboxLayout->addLayout(vboxLayout1);
    vboxLayout1->addSpacing(18);
    vboxLayout1->addWidget(manTaskBar);
    vboxLayout1->addSpacing(18);
    vboxLayout1->addWidget(installTable);
    vboxLayout->addWidget(splitLabel);
    vboxLayout->addLayout(vboxLayout2);
    //    vboxLayout->addSpacing(0);
    vboxLayout2->addSpacing(24);
    vboxLayout2->addWidget(manCompBar);
    vboxLayout2->addSpacing(24);
    vboxLayout2->addWidget(compTable);
    //    vboxLayout->addStretch();

    wid->setMaximumWidth(1200);
    wid->setLayout(vboxLayout);
    wid->setObjectName("pageManagerWidget");
    manScroArea->setWidget(wid);
    manScroArea->setWidgetResizable(true);

}

void ManagerPage::onAppUpdate(QString iconUrl, QString appName, QString appVersion, QString appSize, int relid, QString exefile, QString pkgname, int productid)
{
    qDebug() << __FUNCTION__;
    qDebug() << "appSize = " << appSize;

    for(int i = 0; i < installTable->rowCount(); i++)
    {
        ManagerWidget *managerWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        QString insName = managerWidget->getPkgName();
        if(pkgname == insName)
        {
            managerWidget->setManagerButton(tr("Updating..."));
            managerWidget->getButton(MANAGERBUTTON)->setEnabled(false);
            return;
        }
    }

    ManagerWidget *manTaskManager = new ManagerWidget(this,iconUrl,appName,appVersion,appSize);
    manTaskManager->getButton(MANAGERBUTTON)->setStyleSheet("font-weight: 500; color:#027aff; border: 1px solid #027aff  ; border-radius: 2px; ");

    manTaskManager->setFlag("UPDATEFLAG");
    manTaskManager->setReleaseId(relid);
    manTaskManager->setPkgName(pkgname);
    manTaskManager->setExeFile(exefile);
    manTaskManager->setProductId(productid);
    manTaskManager->setManagerButton(tr("Updating..."));
    manTaskManager->getButton(MANAGERBUTTON)->setEnabled(false);
    manTaskManager->getButton(UNINSBUTTON)->hide();
    installTable->insertRow(0);
    installTable->setCellWidget(0,0,manTaskManager);
    int rowCount = installTable->rowCount();
    installTable->setMinimumHeight(96*rowCount);
    connect(manTaskManager->getButton(MANAGERBUTTON),SIGNAL(clicked()),this,SLOT(insManBtnClicked()));
    connect(manTaskManager,SIGNAL(detailspageSig(int)),this,SLOT(toDetailSig(int)));

    QString flag = "UPDATEFLAG";
    saveConfigFile(iconUrl, appName, appVersion, appSize, flag, exefile, relid, pkgname, productid);

}

void ManagerPage::onAppUpdateOk(QString appName)
{
    int rowCount = installTable->rowCount();
    for(int i = 0; i < rowCount; i++)
    {
        ManagerWidget *insWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        QString upAppName = insWidget->getAppName();
        if(appName == upAppName)
        {
            installTable->removeRow(i);
            installTable->setMinimumHeight(96*(rowCount - 1));

            int releaseId = insWidget->getReleaseId();
            QString section = QString::number(releaseId,10);
            QString homePath = QDir::homePath();
            QString fileName = homePath + "/.config/.emindappstore/upins.ini";
            QSettings settings(fileName, QSettings::IniFormat);
            settings.remove(section);

            return;
        }
    }

}

void ManagerPage::onAppUpdateFailure(QString appName)
{
    int rowCount = installTable->rowCount();
    for(int i = 0; i < rowCount; i++)
    {
        ManagerWidget *insWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        QString upAppName = insWidget->getAppName();
        if(appName == upAppName)
        {
            insWidget->getButton(MANAGERBUTTON)->setText(tr("ReUpdate"));
            insWidget->getButton(MANAGERBUTTON)->setEnabled(true);

        }
    }
}

void ManagerPage::onAppInstallSuccess(QString appName)
{
    int rowCount = installTable->rowCount();
    for(int i = 0; i < rowCount; i++)
    {
        ManagerWidget *insWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        QString insAppName = insWidget->getAppName();
        if(appName == insAppName)
        {
            QString headUrl = insWidget->getHeadUrl();
            QString appVersion = insWidget->getVersion();
            QString exefile = insWidget->getExeFile();
            QString packageName = insWidget->getPkgName();
            int releaseId = insWidget->getReleaseId();
            int productId = insWidget->getProductId();
            QString section = QString::number(releaseId,10);
            emit appInstallOk(appName,headUrl,appVersion,exefile,packageName,productId);
            emit sigInstallSuccess(appName,true,DOWNLOAD);
            installTable->removeRow(i);
            installTable->setMinimumHeight(96*(rowCount - 1));

            QString homePath = QDir::homePath();
            QString fileName = homePath + "/.config/.emindappstore/upins.ini";
            QSettings settings(fileName, QSettings::IniFormat);
            settings.remove(section);

            return;
        }
    }
}

void ManagerPage::onAppInstallFailure(QString appName)
{
    int rowCount = installTable->rowCount();
    for(int i = 0; i < rowCount; i++)
    {
        ManagerWidget *insWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        QString insAppName = insWidget->getAppName();

        if(appName == insAppName)
        {
            emit sigInstallSuccess(appName,false,DOWNLOAD);
            insWidget->getButton(MANAGERBUTTON)->setText(tr("ReDownLoad"));
            insWidget->getButton(MANAGERBUTTON)->setEnabled(true);
        }
    }
}

void ManagerPage::upInsToInsd(QString appName, QString iconUrl, QString appVer, QString exefile, QString pkgName, int productId)
{
//    qDebug() << __FUNCTION__;
    int rowCountInsd = compTable->rowCount();
    for(int i = 0; i < rowCountInsd; i++)
    {
        ManagerWidget *insdWidget = (ManagerWidget *)compTable->cellWidget(i, 0);
        QString insdAppName = insdWidget->getAppName();
        if(appName == insdAppName)
        {
            qDebug() << "remove i == " <<  i;
            compTable->removeRow(i);
            compTable->setMinimumHeight(96*(rowCountInsd - 1));
            break;
        }
    }
    ManagerWidget *manCompManager = new ManagerWidget(this,iconUrl,appName,appVer,"");
    manCompManager->setExeFile(exefile);
    manCompManager->setProductId(productId);
    compTable->insertRow(0);
    compTable->setCellWidget(0,0,manCompManager);
    int rowCount = compTable->rowCount();
    compTable->setMinimumHeight(96*rowCount);
    manCompManager->resolveNameToId(pkgName);
    connect(manCompManager,SIGNAL(resetPackageIdSuccess(QString)),manCompManager,SLOT(setInstallSize(QString)));
    connect(manCompManager->getButton(HEADBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));
    connect(manCompManager->getButton(NAMEBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));
    connect(manCompManager->getButton(UNINSBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));
    connect(manCompManager->getButton(MANAGERBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));
    connect(manCompManager,SIGNAL(detailspageSig(int)),this,SLOT(toDetailSig(int)));

}

void ManagerPage::deleteRmvRow(QString pacId)
{
    for(int i = 0; i < compTable->rowCount(); i++)
    {
        ManagerWidget* managerWidget = (ManagerWidget*)compTable->cellWidget(i,0);
        if(pacId == managerWidget->getPkgId())
        {
            QString pacName = managerWidget->getAppName();
            emit removePackageSuccess(pacName,true,UNINSTALL);
            compTable->removeRow(i);
            compTable->setMinimumHeight(96*(compTable->rowCount()));
            break;
        }
    }
}

void ManagerPage::onRemoveFailure(QString pacId)
{
    for(int i = 0; i < compTable->rowCount(); i++)
    {
        ManagerWidget* managerWidget = (ManagerWidget*)compTable->cellWidget(i,0);
        if(pacId == managerWidget->getPkgId())
        {
            QString appName = managerWidget->getAppName();
            emit removePackageSuccess(appName,false,UNINSTALL);
            managerWidget->getButton(UNINSBUTTON)->setText(tr("UnInstall"));
        }
    }
}

void ManagerPage::getPackageInstalled()
{
    updm->getMagData();
}

void ManagerPage::readInsdConfig()
{
    QString homePath = QDir::homePath();
    QString fileName = homePath + "/.config/.emindappstore/insd.ini";
    QSettings settings( fileName, QSettings::IniFormat );
    QStringList keys = settings.childGroups();
    int saveCount = keys.count();
    QString section;
    QString headUrl;
    QString appName;
    QString appVersion;
    QString appSizeStr;
    QString exefile;
    int releaseId;
    QString packageId;

    compTable->setRowCount(saveCount);
    compTable->setMinimumHeight(96*saveCount);

    for(int i = 0; i < saveCount; i++)
    {
        section = keys.at(i);
        settings.beginGroup(section);
        headUrl = settings.value("Url").toString();
        appName = settings.value("Name").toString();
        appVersion = settings.value("Version").toString();
        appSizeStr = settings.value("Size").toString();
        exefile = settings.value("Exe").toString();
        releaseId = settings.value("Relid").toInt();
        packageId = settings.value("PkgId").toString();
        settings.endGroup();

        QStringList urlList = headUrl.split("/");
        int count = urlList.count();

        QString imageName = urlList.at(count - 1);
        QString homePath = QDir::homePath();
        QString iconPath = homePath + "/.config/.emindappstore/icon/";
        QString fileName = iconPath + imageName;
        QPixmap pixmap(fileName);

        ManagerWidget *manTaskManager = new ManagerWidget(this,"",appName,appVersion,"");
        manTaskManager->setSizeLabel(appSizeStr);
        manTaskManager->setExeFile(exefile);
        manTaskManager->setPkgId(packageId);
        manTaskManager->setReleaseId(releaseId);
        manTaskManager->getButton(MANAGERBUTTON)->setText(tr("Open"));
        manTaskManager->getButton(HEADBUTTON)->setIcon(pixmap);
        manTaskManager->getButton(HEADBUTTON)->setIconSize(QSize(64,64));

        compTable->setCellWidget(i,0,manTaskManager);

        connect(manTaskManager->getButton(UNINSBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));
        connect(manTaskManager->getButton(MANAGERBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));


    }
}

void ManagerPage::insManBtnClicked()
{
    qDebug() << __FUNCTION__;
    int rowCount = installTable->rowCount();
    for(int i = 0; i < rowCount; i++)
    {
        ManagerWidget *insWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        if(sender() == insWidget->getButton(MANAGERBUTTON))
        {
            QString flag = insWidget->getFlag();
            QString appName = insWidget->getAppName();
            if(flag == "UPDATEFLAG")
            {
                insWidget->setManagerButton(tr("Updating..."));
                insWidget->getButton(MANAGERBUTTON)->setEnabled(false);
                emit upBtnClicked(appName);
            }
            else if(flag == "INSTALLFLAG")
            {
                int releaseId = insWidget->getReleaseId();
                qDebug() << "releaseId == " << releaseId;
                insWidget->setManagerButton(tr("Installing..."));
                insWidget->getButton(MANAGERBUTTON)->setEnabled(false);
                emit insBtnClicked(appName,releaseId);
            }
        }
    }
}

void ManagerPage::allStartBtnclicked()
{
    int insdCount = installTable->rowCount();
    for(int i = 0; i < insdCount; i++)
    {
        ManagerWidget* insUpWidget = (ManagerWidget*)installTable->cellWidget(i,0);
        QString flag = insUpWidget->getFlag();
        if(flag == "UPDATEFLAG")
        {
            insUpWidget->setManagerButton(tr("Updating..."));
            insUpWidget->getButton(MANAGERBUTTON)->setEnabled(false);
            emit upBtnClicked(insUpWidget->getButton(NAMEBUTTON)->text());
        }
        else if(flag == "INSTALLFLAG")
        {
            int releaseId = insUpWidget->getReleaseId();
            insUpWidget->setManagerButton(tr("Installing..."));
            insUpWidget->getButton(MANAGERBUTTON)->setEnabled(false);
            emit insBtnClicked(insUpWidget->getButton(NAMEBUTTON)->text(),releaseId);
        }
    }
}

void ManagerPage::getInstallRlease(QString appName, int releaseId)
{
    qDebug() << __FUNCTION__;
    for(int i = 0; i < installTable->rowCount(); i++)
    {
        ManagerWidget *managerWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        QString insName = managerWidget->getAppName();
        if(appName == insName)
        {
            QString packageName = managerWidget->getPkgName();
            managerWidget->setManagerButton(tr("Installing..."));
            managerWidget->installPackage(packageName);
            return;
        }
    }
    ManagerWidget *manTaskManager = new ManagerWidget(this,"", appName, "", "");
    manTaskManager->setReleaseId(releaseId);
    manTaskManager->getButton(UNINSBUTTON)->hide();
    manTaskManager->setFlag("INSTALLFLAG");

    installTable->insertRow(0);
    installTable->setCellWidget(0,0,manTaskManager);
    int rowCount = installTable->rowCount();
    installTable->setMinimumHeight(96*rowCount);

    connect(manTaskManager->jsonFunc,SIGNAL(updateIsOk()),this,SLOT(onAppInstall()));
    connect(manTaskManager->jsonFunc,SIGNAL(jsonError()),this,SLOT(insReleaseError()));

    int releaseAry[1];
    releaseAry[0] = releaseId;
    manTaskManager->jsonFunc->getUpdateRelease(releaseAry,1);
}

void ManagerPage::onAppInstall()
{
    qDebug() << __FUNCTION__;
    //    if(shareData->updateStrMap.isEmpty())
    //    {
    //        return;
    //    }

    for(int i = 0; i < installTable->rowCount(); i++)
    {
        ManagerWidget *managerWidget = (ManagerWidget *)installTable->cellWidget(i, 0);
        managerWidget->getButton(MANAGERBUTTON)->setStyleSheet("font-weight: 500; color:#027aff; border: 1px solid #027aff  ; border-radius: 2px; ");

        if(sender() == managerWidget->jsonFunc)
        {
            QString headUrl = managerWidget->shareData->updateStrMap.begin().value().proImage;
            QString appName = managerWidget->shareData->updateStrMap.begin().value().proName;
            QString packageName = managerWidget->shareData->updateStrMap.begin().value().packageName;
            QString appVersion = tr("Version : V") + managerWidget->shareData->updateStrMap.begin().value().version;
            double appSize = managerWidget->shareData->updateStrMap.begin().value().packageSize;
            QString pacSize = updm->transPackSize(appSize);
            QString appSizeStr = tr("Size : ") + pacSize;
            QString exefile = managerWidget->shareData->updateStrMap.begin().value().exeFile;
            int productId = managerWidget->shareData->updateStrMap.begin().value().productId;

            managerWidget->setManagerButton(tr("Installing..."));
            managerWidget->getButton(MANAGERBUTTON)->setEnabled(false);
            managerWidget->setHeadUrl(headUrl);
            managerWidget->setAppName(appName);
            managerWidget->setPkgName(packageName);
            managerWidget->setVersion(appVersion);
            managerWidget->setExeFile(exefile);
            managerWidget->setProductId(productId);

            managerWidget->getImage(headUrl);
            managerWidget->getButton(NAMEBUTTON)->setText(appName);
            managerWidget->setVerLabel(appVersion);
            managerWidget->setSizeLabel(appSizeStr);

            QString flag = "INSTALLFLAG";
            int releaseId = managerWidget->getReleaseId();
            saveConfigFile(headUrl, appName, appVersion, pacSize, flag, exefile, releaseId, packageName, productId);

            connect(managerWidget,SIGNAL(installSuccess(QString)),this,SLOT(onAppInstallSuccess(QString)));
            connect(managerWidget,SIGNAL(installFailure(QString)),this,SLOT(onAppInstallFailure(QString)));
            connect(managerWidget->getButton(MANAGERBUTTON),SIGNAL(clicked()),this,SLOT(insManBtnClicked()));
            connect(managerWidget,SIGNAL(detailspageSig(int)),this,SLOT(toDetailSig(int)));

            managerWidget->installPackage(packageName);
            return;
        }
    }
}


void ManagerPage::insReleaseError()
{
    qDebug() << __FUNCTION__;

    for(int i = 0; i < installTable->rowCount(); i++)
    {
        ManagerWidget *managerWidget = (ManagerWidget *)installTable->cellWidget(i, 0);

        if(sender() == managerWidget->jsonFunc)
        {
            QString appName = managerWidget->getAppName();
            installTable->removeRow(i);
            qDebug() << "appName ======" << appName;
            emit sigInstallSuccess(appName,false,DOWNLOAD);
            break;
        }
    }
}

void ManagerPage::onGetInsFinished(QMap<QString,INSTALLEDSTRUCT> insdMap)
{
    emit toGetDetail(insdMap);
    readConfigFile();

    int insRowCount = insdMap.count();

    compTable->setRowCount(insRowCount);
    compTable->setMinimumHeight(96*insRowCount);

    QMap<QString,INSTALLEDSTRUCT>::iterator item;
    int count = 0;
    for(item = insdMap.begin(); item != insdMap.end(); item++)
    {
        QString packageId = item.key();
        QString appName = item.value().appName;
        QString appVersion = tr("Version : V") + PackageKit::Transaction::packageVersion(packageId);
        QString headUrl = item.value().proImage;
        QString exefile = item.value().exeFile;
        int productId = item.value().productId;
        int releaseId = item.value().releaseId;

        ManagerWidget *manTaskManager = new ManagerWidget(this,headUrl,appName,appVersion,"");
        manTaskManager->setExeFile(exefile);
        manTaskManager->setPkgId(packageId);
        manTaskManager->setProductId(productId);
        manTaskManager->setReleaseId(releaseId);
        manTaskManager->getButton(MANAGERBUTTON)->setText(tr("Open"));
        compTable->setCellWidget(count,0,manTaskManager);
//        manTaskManager->setInstallSize(packageId);     //now not use,get size has moved to thread
        count++;

        connect(manTaskManager,SIGNAL(detailspageSig(int)),this,SLOT(toDetailSig(int)));
        connect(manTaskManager->getButton(HEADBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));
        connect(manTaskManager->getButton(NAMEBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));
        connect(manTaskManager->getButton(UNINSBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));
        connect(manTaskManager->getButton(MANAGERBUTTON),SIGNAL(clicked()),this,SLOT(compBtnClicked()));       

        QString relidStr = QString::number(releaseId);
        QString homePath = QDir::homePath();
        QString fileName = homePath + "/.config/.emindappstore/insd.ini";
        QSettings settings( fileName, QSettings::IniFormat );
        settings.beginGroup(relidStr);
        settings.setValue("Url", headUrl);
        settings.setValue("Name", appName);
        settings.setValue("Version", appVersion);
        settings.setValue("Exe", exefile);
        settings.setValue("Relid", releaseId);
        settings.setValue("PkgId",packageId);
        settings.endGroup();

    }
}

void ManagerPage::compBtnClicked()
{
    for(int i = 0; i < compTable->rowCount(); i++)
    {
        ManagerWidget* managerWidget = (ManagerWidget*)compTable->cellWidget(i,0);

        if(sender() == managerWidget->getButton(HEADBUTTON) )
        {
            qDebug() << "btn == headButton:" << "row == " << i;
            break;
        }
        else if(sender() == managerWidget->getButton(NAMEBUTTON))
        {
            qDebug() << "btn == nameButton:" << "row == " << i;
            break;
        }
        else if(sender() == managerWidget->getButton(UNINSBUTTON) )
        {
            qDebug() << "btn == uninsButton:" << "row == " << i;
            managerWidget->getButton(UNINSBUTTON)->setText(tr("UnInstalling..."));
            QString packageID = managerWidget->getPkgId();
            QString appName = managerWidget->getAppName();
            emit sigUninstalling(appName,true,UNINSTALLING);
            managerWidget->removePackage(packageID);
            connect(managerWidget,SIGNAL(pacRmvSuccess(QString)),this,SLOT(deleteRmvRow(QString)));
            connect(managerWidget,SIGNAL(pacRmvFailure(QString)),this,SLOT(onRemoveFailure(QString)));

            break;
        }
        else if(sender() == managerWidget->getButton(MANAGERBUTTON))
        {
            qDebug() << "btn == managerButton:" << "row == " << i;
            QString openAppName =  managerWidget->getExeFile();
            QProcess *proc = new QProcess;
            proc->start(openAppName);
            break;
        }

    }
}

void ManagerPage::saveConfigFile(QString headUrl, QString appName, QString appVersion, QString appSizeStr, QString flag, QString exefile, int releaseId, QString packageName, int productid)
{
    //    QStringList appSizeList = appSizeStr.split(":");
    QString appSize = appSizeStr;
    QString relidStr = QString::number(releaseId);
    QString homePath = QDir::homePath();
    QString fileName = homePath + "/.config/.emindappstore/upins.ini";
    QSettings settings( fileName, QSettings::IniFormat );
    settings.beginGroup(relidStr);
    settings.setValue("Url", headUrl);
    settings.setValue("Name", appName);
    settings.setValue("Version", appVersion);
    settings.setValue("Size", appSize);
    settings.setValue("Flag", flag);
    settings.setValue("Exe", exefile);
    settings.setValue("Relid", releaseId);
    settings.setValue("PkgName",packageName);
    settings.setValue("ProId", productid);
    settings.endGroup();
}

void ManagerPage::readConfigFile()
{
    QString homePath = QDir::homePath();
    QString fileName = homePath + "/.config/.emindappstore/upins.ini";
    QSettings settings( fileName, QSettings::IniFormat );
    QStringList keys = settings.childGroups();
    int saveCount = keys.count();
    QString section;
    QString headUrl;
    QString appName;
    QString appVersion;
    QString appSizeStr;
    QString flag;
    QString exefile;
    int releaseId;
    QString packageName;
    int productId;

    for(int i = 0; i < saveCount; i++)
    {
        section = keys.at(i);
        settings.beginGroup(section);
        headUrl = settings.value("Url").toString();
        appName = settings.value("Name").toString();
        appVersion = settings.value("Version").toString();
        appSizeStr = settings.value("Size").toString();
        flag = settings.value("Flag").toString();
        exefile = settings.value("Exe").toString();
        releaseId = settings.value("Relid").toInt();
        packageName = settings.value("PkgName").toString();
        productId = settings.value("ProId").toInt();
        settings.endGroup();

        ManagerWidget *savedTaskManager = new ManagerWidget(this,headUrl, appName, appVersion, appSizeStr);
        savedTaskManager->getButton(MANAGERBUTTON)->setStyleSheet("font-weight: 500; color:#027aff; border: 1px solid #027aff  ; border-radius: 2px; ");

        savedTaskManager->setFlag(flag);
        savedTaskManager->setPkgName(packageName);
        savedTaskManager->setExeFile(exefile);
        savedTaskManager->setReleaseId(releaseId);
        savedTaskManager->setProductId(productId);
        if(flag == "INSTALLFLAG")
        {
            savedTaskManager->setManagerButton(tr("ReDownload"));
            connect(savedTaskManager,SIGNAL(installSuccess(QString)),this,SLOT(onAppInstallSuccess(QString)));
            connect(savedTaskManager,SIGNAL(installFailure(QString)),this,SLOT(onAppInstallFailure(QString)));
            connect(savedTaskManager->getButton(MANAGERBUTTON),SIGNAL(clicked()),this,SLOT(insManBtnClicked()));
        }
        else
        {
            savedTaskManager->setManagerButton(tr("ReUpdate"));
            connect(savedTaskManager->getButton(MANAGERBUTTON),SIGNAL(clicked()),this,SLOT(insManBtnClicked()));
        }
        savedTaskManager->getButton(UNINSBUTTON)->hide();
        installTable->insertRow(0);
        installTable->setCellWidget(0,0,savedTaskManager);
        int rowCount = installTable->rowCount();
        installTable->setMinimumHeight(96*rowCount);

        connect(savedTaskManager,SIGNAL(detailspageSig(int)),this,SLOT(toDetailSig(int)));

    }
}

void ManagerPage::onStatusChanged()
{
    emit installStatusChanged();
}

void ManagerPage::sendSigReUpdate(QString appName)
{
    emit sigReUpdate(appName,true,UPDATING);
}

void ManagerPage::sendSigReDownload(QString appName)
{
    emit sigReDownload(appName,true,DOWNLOADING);
}

void ManagerPage::toDetailSig(int productId)
{
    emit detailspageSig(productId);
}

void ManagerPage::setInsdSize(QMap<QString, QString> insdSizeMap)
{
//    qDebug() << __FUNCTION__;
    int rowCountInsd = compTable->rowCount();
    QString insdPkgId;
    QString insdSizePkgId;
    QString size;
    for(int i = 0; i < rowCountInsd; i++)
    {
        ManagerWidget *insdWidget = (ManagerWidget *)compTable->cellWidget(i, 0);
        insdPkgId = insdWidget->getPkgId();
        int releaseId = insdWidget->getReleaseId();

        QMap<QString,QString>::iterator itor;
        for(itor = insdSizeMap.begin(); itor != insdSizeMap.end(); itor++)
        {
            insdSizePkgId = itor.key();
            if(insdPkgId == insdSizePkgId)
            {
                size = itor.value();
                insdWidget->setSizeLabel(size);
                QString relidStr = QString::number(releaseId);
                QString homePath = QDir::homePath();
                QString fileName = homePath + "/.config/.emindappstore/insd.ini";
                QSettings settings( fileName, QSettings::IniFormat );
                settings.beginGroup(relidStr);
                settings.setValue("Size", size);
                settings.endGroup();
            }
        }
    }
    emit setInsdSizeOk();
}

bool ManagerPage::event(QEvent *event)
{
    if(event->type() == QEvent::Resize)
    {
        manScroArea->resize(QSize(this->width()-5,this->height()));

        return true;
    }
    return QWidget::event(event);

}


