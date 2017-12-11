#include <QHeaderView>
#include <QDebug>
#include <QSettings>
#include <QLoggingCategory>
#include <QDir>
#include "updatepage.h"

UpdatePage::UpdatePage(QWidget *parent, JSONFUNC *json,ShareData *sharedata) : QWidget(parent)
{
    pageUpdateWidget = NULL;
    updTaskBar = NULL;
    upVLayout = NULL;
    upScroArea = NULL;
    upd = NULL;
    jsonFunc = NULL;

    upd = new PkUpdates(this,json,sharedata);
    jsonFunc = json;
    connect(json,SIGNAL(productIsOk()),this,SLOT(getPackageUpdate()));
    connect(upd,SIGNAL(updateStatusChanged()),this,SLOT(onStatusChanged()));
    connect(this,SIGNAL(appUpdateOk(QString, QString, QString, QString, QString, int)),this,SLOT(sendUpdateSuccess(QString)));
    createUpdateWindow();
}

UpdatePage::~UpdatePage()
{

}

void UpdatePage::createUpdateWindow()
{
    upScroArea = new QScrollArea(this);
    upScroArea->resize(960,640);
    upScroArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    upScroArea->setFrameShape(QFrame::NoFrame);
    upScroArea->setAlignment(Qt::AlignHCenter);
    pageUpdateWidget = new QWidget();
    pageUpdateWidget->setMaximumWidth(1200);

    updateTable = new QTableWidget(this);
    updateTable->setObjectName("updateTable");
    updateTable->setShowGrid(false);
    updateTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    updateTable->setFrameShape(QFrame::NoFrame);
    updateTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    updateTable->setFocusPolicy(Qt::NoFocus);

    updTaskBar = new TaskBar(this,tr("0 Application Can Be Updated"),tr("All Update"));
    connect(upd,SIGNAL(sigUpdateData(UPDATESTRUCTMAP)),this,SLOT(onGetupFinished(UPDATESTRUCTMAP)));
    connect(updTaskBar->getOnekeyBtn(),SIGNAL(clicked()),this,SLOT(oneKeyBtnclicked()));

    upVLayout = new QVBoxLayout();
    upVLayout->setMargin(16);
//    upVLayout->addSpacing(14);
    upVLayout->addWidget(updTaskBar);
    upVLayout->addSpacing(18);
    upVLayout->addWidget(updateTable);
    this->setLayout(upVLayout);

    pageUpdateWidget->setLayout(upVLayout);
    pageUpdateWidget->setObjectName("pageUpdateWidget");
    upScroArea->setWidget(pageUpdateWidget);
    upScroArea->setWidgetResizable(true);

}

void UpdatePage::onGetupFinished(UPDATESTRUCTMAP updateMap)
{
    qDebug() << endl << __FUNCTION__ ;
    int rowCount = updateMap.count();
    updTaskBar->setTaskLabel(rowCount);
    updateTable->setColumnCount(1);
    updateTable->setRowCount(rowCount);
    //    updateTable->resizeColumnToContents(0);
    updateTable->verticalHeader()->setDefaultSectionSize(96);
    updateTable->horizontalHeader()->setStretchLastSection(true);
    updateTable->setFrameStyle(QFrame::NoFrame);
    updateTable->setSelectionMode(QAbstractItemView::NoSelection);
    updateTable->setFocusPolicy(Qt::NoFocus);

    updateTable->setMinimumHeight(96*rowCount);

    QHeaderView* vheaderView = updateTable->verticalHeader();
    vheaderView->setHidden(true);
    QHeaderView *hheaderView = updateTable->horizontalHeader();
    hheaderView->setHidden(true);

    QMap<int,UPDATESTRUCT>::iterator itor;
    int count = 0;
    for(itor = updateMap.begin(); itor != updateMap.end(); itor++)
    {
        QString headUrl = itor.value().proImage;
        QString appName = itor.value().proName;
        QString packName = itor.value().packageName;
        QString changeLog = itor.value().changeLog;
        double psize = itor.value().packageSize;
        QString packSize = transPackSize(psize);
        QString packVersion = "V" + itor.value().version;
        QString exefile = itor.value().exeFile;
        int productId = itor.value().productId;
        int relid = itor.key();

        QVariantMap updateList = upd->packages();
        QString pkgId;
        QVariantMap::iterator iter;
        for(iter = updateList.begin(); iter != updateList.end(); iter++)     //get pkgId
        {
            QString packageName = PackageKit::Transaction::packageName(iter.key());
            if(packName == packageName)
            {
                pkgId = iter.key();
                break;
            }
        }

        AppWidget* appWidget = new AppWidget(this,headUrl,appName,packSize,packVersion,changeLog,pkgId);
        appWidget->setExeFile(exefile);
        appWidget->setReleaseId(relid);
        appWidget->setPkgName(packName);
        appWidget->setProductId(productId);
        updateTable->setCellWidget(count,0,appWidget);

        QString homePath = QDir::homePath();
        QString fileName = homePath + "/.config/.emindappstore/upins.ini";
        QSettings settings( fileName, QSettings::IniFormat );
        QStringList keys = settings.childGroups();
        QString section;
        QString pkgName;
        QString flag;
        int saveCount = keys.count();
        for(int i = 0; i < saveCount; i++)
        {
            section = keys.at(i);
            settings.beginGroup(section);
            pkgName = settings.value("PkgName").toString();
            flag = settings.value("Flag").toString();
            settings.endGroup();
            if(packName == pkgName)
            {
                if(flag == "UPDATEFLAG")
                {
                    appWidget->getUpdateButton()->setText(tr("ReUpdate"));
                }
                break;
            }
        }
        count++;

        connect(appWidget->getHeadButton(),SIGNAL(clicked()),this,SLOT(pageUpdateBtnClicked()));
        connect(appWidget->getNameButton(),SIGNAL(clicked()),this,SLOT(pageUpdateBtnClicked()));
        connect(appWidget->getFuncButton(),SIGNAL(clicked()),this,SLOT(pageUpdateBtnClicked()));
        connect(appWidget->getUpdateButton(),SIGNAL(clicked()),this,SLOT(pageUpdateBtnClicked()));
        connect(appWidget,SIGNAL(sigIntroResize()),this,SLOT(strLenChanged()));
    }

    QString homePath = QDir::homePath();
    QString fileName = homePath + "/.config/.emindappstore/upins.ini";
    QSettings settings1( fileName, QSettings::IniFormat );
    QStringList settingsKeys = settings1.childGroups();
    int upCount;
    QString sectionIni;
    QString pkgNameIni;
    int iniCount = settingsKeys.count();

    for(int i = 0; i < iniCount; i++)
    {
        sectionIni = settingsKeys.at(i);
        settings1.beginGroup(sectionIni);
        pkgNameIni = settings1.value("PkgName").toString();
        settings1.endGroup();

        upCount = 0;
        for(itor = updateMap.begin(); itor != updateMap.end(); itor++)
        {
            QString packNameUp = itor.value().packageName;
            if(packNameUp == pkgNameIni)
            {
                upCount++;
                break;
            }
        }
        if(upCount == 0)
        {
            settings1.beginGroup(sectionIni);
            QString flag1 = settings1.value("Flag").toString();
            settings1.endGroup();
            if(flag1 == "UPDATEFLAG")
            {
                settings1.remove(sectionIni);
            }
        }
    }

    updateTable->setRowHeight(0,95);
    updateTable->setRowHeight(0,96);
}

void UpdatePage::pageUpdateBtnClicked()
{
    for(int i = 0; i < updateTable->rowCount(); i++)
    {
        AppWidget* appWidget = (AppWidget*)updateTable->cellWidget(i,0);

        if(sender() == appWidget->getHeadButton() )
        {
            //            qDebug() << "btn == headButton:" << "row == " << i;
            int productId = appWidget->getProductId();
            emit detailspageSig(productId);
            break;
        }
        else if(sender() == appWidget->getNameButton())
        {
            //            qDebug() << "btn == nameButton:" << "row == " << i;
            int productId = appWidget->getProductId();
            emit detailspageSig(productId);
            break;
        }
        else if(sender() == appWidget->getFuncButton())
        {
            int tableHeight = updateTable->height();

            updateTable->insertRow(i+1);
            QString funcStr = appWidget->getNewFunStr();
            qDebug() << "funcStr == " << funcStr;
            FuncWidget *nfuncWidget = new FuncWidget(this,funcStr);
            updateTable->setCellWidget(i+1,0,nfuncWidget);
            int textHeight  = nfuncWidget->getNfuncEdit()->document()->size().height();
            int rowHeight = textHeight+20+16+18+22;
            updateTable->setRowHeight(i+1,rowHeight);
            appWidget->getFuncButton()->setEnabled(false);
            updateTable->setMinimumHeight(tableHeight + rowHeight);

            connect(nfuncWidget->getHideButton(),SIGNAL(clicked()),this,SLOT(pageUpdateBtnClicked()));
            connect(nfuncWidget,SIGNAL(sigTextHeight(int)),this,SLOT(textAreaChanged(int)));

            break;
        }
        else if(sender() == appWidget->getUpdateButton())
        {
            qDebug() << "btn == updateButton:" << "row == " << i;

            QString iconUrl = appWidget->getHeadUrl();
            QString appName = appWidget->getAppName();
            QString appVersion = tr("Version : ") + appWidget->getAppVer();
            QString appSize = appWidget->getAppSize();
            int releasId = appWidget->getReleaseId();
            QString exeFile = appWidget->getExeFile();
            QString pkgName = appWidget->getPkgName();
            int productId = appWidget->getProductId();
            emit updatePackage(appName,true,8);
            emit theUpdateApp(iconUrl, appName, appVersion, appSize, releasId, exeFile, pkgName, productId);

            QString pkgId = appWidget->getPkgId();
            appWidget->getUpdateButton()->setText(tr("UpDating..."));
            appWidget->getUpdateButton()->setEnabled(false);
            connect(appWidget,SIGNAL(updateOk()),this,SLOT(onAppUpdateFinished()));
            connect(appWidget,SIGNAL(updateFailure()),this,SLOT(onAppUpdateFailure()));

            appWidget->installUpdate(pkgId);
            break;
        }
        else
        {
            FuncWidget* gfuncWidget = (FuncWidget*)updateTable->cellWidget(i,0);
            if(sender() == gfuncWidget->getHideButton())
            {
                //                qDebug() << "btn == hideButton:" << "row == " << i;
                int rmvTableHeight = updateTable->height();
                int rmvRowHeight = updateTable->rowHeight(i);
                int tableHeight = rmvTableHeight - rmvRowHeight;
                AppWidget *eblBtnWid = (AppWidget *)updateTable->cellWidget(i-1,0);
                eblBtnWid->getFuncButton()->setEnabled(true);
                updateTable->removeRow(i);
                updateTable->setMinimumHeight(tableHeight);
            }
        }
    }
}

void UpdatePage::oneKeyBtnclicked()
{
    int updCount = updateTable->rowCount();
    for(int i = 0; i < updCount; i++)
    {
        AppWidget* appWidget = (AppWidget*)updateTable->cellWidget(i,0);

        QString iconUrl = appWidget->getHeadUrl();
        QString appName = appWidget->getAppName();
        QString appVersion = tr("Version : ") + appWidget->getAppVer();
        QString appSize =  appWidget->getAppSize();

        int releasId = appWidget->getReleaseId();
        QString exeFile = appWidget->getExeFile();
        QString pkgName = appWidget->getPkgName();
        int productId = appWidget->getProductId();
        emit updatePackage(appName,true,UPDATING);
        emit theUpdateApp(iconUrl, appName, appVersion, appSize, releasId, exeFile, pkgName, productId);

        QString pkgId = appWidget->getPkgId();
        appWidget->getUpdateButton()->setText(tr("Updating..."));
        connect(appWidget,SIGNAL(updateOk()),this,SLOT(onAppUpdateFinished()));
        connect(appWidget,SIGNAL(updateFailure()),this,SLOT(onAppUpdateFailure()));
        appWidget->installUpdate(pkgId);

        if(!appWidget->getFuncButton()->isEnabled())
            i++;

    }
}

void UpdatePage::onAppUpdateFinished()
{
    for(int i = 0; i < updateTable->rowCount(); i++)
    {
        AppWidget* appWidget = (AppWidget*)updateTable->cellWidget(i,0);
        if(sender() == appWidget)
        {
            emit updatePackageSuccess(appWidget->getAppName(),true,UPDATE);
            emit appUpdateOk(appWidget->getAppName(), appWidget->getHeadUrl(), appWidget->getAppVer(), appWidget->getExeFile(), appWidget->getPkgName(), appWidget->getReleaseId());
            if(!appWidget->getFuncButton()->isEnabled())
                updateTable->removeRow(i+1);
            updateTable->removeRow(i);
            int count = updateTable->rowCount();
            updateTable->setMinimumHeight(96*count);
            updTaskBar->setTaskLabel(count);
        }
    }
}

void UpdatePage::onAppUpdateFailure()
{
    qDebug() << __FUNCTION__;
    for(int i = 0; i < updateTable->rowCount(); i++)
    {
        AppWidget* appWidget = (AppWidget*)updateTable->cellWidget(i,0);
        if(sender() == appWidget)
        {
            QString appName = appWidget->getAppName();
            emit updatePackageSuccess(appName,false,UPDATE);
            emit appUpdateFailure(appName);
            appWidget->getUpdateButton()->setText(tr("ReUpdate"));
        }
    }
}

void UpdatePage::getPackageUpdate()
{
    upd->getUpdateData();
}

void UpdatePage::onUpBtnClicked(QString appName)
{
    qDebug() << __FUNCTION__;
    int updCount = updateTable->rowCount();
    for(int i = 0; i < updCount; i++)
    {
        AppWidget* appWidget = (AppWidget*)updateTable->cellWidget(i,0);
        if(appName == appWidget->getAppName())
        {
            QString iconUrl = appWidget->getHeadUrl();
            QString appName = appWidget->getAppName();
            QString appVersion = tr("Version : ") + appWidget->getAppVer();
            QString appSize = appWidget->getAppSize();
            int releasId = appWidget->getReleaseId();
            QString exeFile = appWidget->getExeFile();
            QString pkgName = appWidget->getPkgName();
            int productId = appWidget->getProductId();
            emit updatePackage(appName,true,8);
            emit theUpdateApp(iconUrl, appName, appVersion, appSize, releasId, exeFile, pkgName, productId);

            appWidget->getUpdateButton()->setText("UpDating...");
            QString pkgId = appWidget->getPkgId();
            connect(appWidget,SIGNAL(updateOk()),this,SLOT(onAppUpdateFinished()));
            connect(appWidget,SIGNAL(updateFailure()),this,SLOT(onAppUpdateFailure()));

            appWidget->installUpdate(pkgId);
        }

        if(!appWidget->getFuncButton()->isEnabled())
            i++;
    }
}

void UpdatePage::strLenChanged()
{
    for(int i = 0; i < updateTable->rowCount(); i++)
    {
        AppWidget* chLabelWidth = (AppWidget*)updateTable->cellWidget(i,0);
        if(sender() == chLabelWidth)
        {
            chLabelWidth->getIntroLabel()->setText(chLabelWidth->getIntroStr());
            int labelWidth = chLabelWidth->getIntroLabel()->width();
            QFont font;
            QFontMetrics fm(font);
            int fontSize = fm.width(chLabelWidth->getIntroStr());
            QString fmstr = chLabelWidth->getIntroStr();
            labelWidth = updateTable->width() - 520;
            if(labelWidth > 550)
            {
                labelWidth = 550;
            }
            if(fontSize > labelWidth)
            {
                fmstr = fm.elidedText(chLabelWidth->getIntroStr(), Qt::ElideRight, labelWidth-130);
            }
            chLabelWidth->getIntroLabel()->setText(fmstr);
        }
    }
}

void UpdatePage::textAreaChanged(int hig)
{
    for(int i = 0; i < updateTable->rowCount(); i++)
    {
        FuncWidget* chEditHeight = (FuncWidget*)updateTable->cellWidget(i,0);
        if(sender() == chEditHeight)
        {
            int rowHeight = hig+20+16+18+22;
            updateTable->setRowHeight(i,rowHeight+5);
        }
    }
}

QString UpdatePage::transPackSize(const double &size)
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

void UpdatePage::onStatusChanged()
{
    emit updateStatusChanged();
}

void UpdatePage::sendUpdateSuccess(QString appName)
{
    emit sigUpdateSuccess(appName,true,UPDATE);
}

void UpdatePage::sendUpdateFailure(QString appName)
{
    emit sigUpdateSuccess(appName,false,UPDATE);
}

bool UpdatePage::event(QEvent *event)
{
    if(event->type() == QEvent::Resize)
    {
        upScroArea->resize(QSize(this->width() - 5,this->height()));

        return true;
    }
    return QWidget::event(event);
}

