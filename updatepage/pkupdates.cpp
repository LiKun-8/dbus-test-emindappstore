#include <QDebug>
#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include "pkupdates.h"

PkUpdates::PkUpdates(QObject *parent, JSONFUNC *jsonfunc, ShareData *sharedata ) :
    QObject(parent),
    m_updatesTrans(Q_NULLPTR),
    m_cacheTrans(Q_NULLPTR),
    m_packagesTrans(Q_NULLPTR)
{
    mainShareData = sharedata;
    mainJsonFunc = jsonfunc;
    shareData = new ShareData();
    jsonFunc = new JSONFUNC(shareData);
    //    sigFlag = 1;
}

void PkUpdates::checkUpdates(bool force)
{
//    qDebug() << "Checking updates, forced";
    m_cacheTrans = PackageKit::Daemon::refreshCache(force);
    connect(m_cacheTrans.data(), &PackageKit::Transaction::finished, this, &PkUpdates::onFinished);
}

void PkUpdates::getUpdateData()
{
    getUpdates();
}

void PkUpdates::getMagData()
{
    getPackages();
}

void PkUpdates::getUpdates()
{
    m_updatesTrans = PackageKit::Daemon::getUpdates();
    m_updateList.clear();
    m_upNameList.clear();

    connect(m_updatesTrans.data(), &PackageKit::Transaction::finished, this, &PkUpdates::onFinished);
    //    connect(m_updatesTrans.data(), &PackageKit::Transaction::errorCode, this, &PkUpdates::onErrorCode);
    connect(m_updatesTrans.data(), &PackageKit::Transaction::package, this, &PkUpdates::onPackage);
}

void PkUpdates::getPackages()
{
//    qDebug() << __FUNCTION__;
    m_packagesTrans = PackageKit::Daemon::getPackages(PackageKit::Transaction::FilterInstalled);
    m_packagesList.clear();

    connect(m_packagesTrans.data(), &PackageKit::Transaction::finished, this, &PkUpdates::onFinished);
    //    connect(m_updatesTrans.data(), &PackageKit::Transaction::errorCode, this, &PkUpdates::onErrorCode);
    connect(m_packagesTrans.data(), &PackageKit::Transaction::package, this, &PkUpdates::onGetPackages);
}

void PkUpdates::onPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
    QString packageName = PackageKit::Transaction::packageName(packageID);
//    qDebug() << __FUNCTION__ << ": Got update package:" << packageID;

    switch (info)
    {
    case PackageKit::Transaction::InfoBlocked:
        return;
    default:
        break;
    }
    m_updateList.insert(packageID, summary);
    m_upNameList.append(packageName);
}

void PkUpdates::onGetPackages(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary)
{
    switch (info) {
    case PackageKit::Transaction::InfoInstalled:
    {
        m_packagesList.insert(packageID, summary);
    }
        break;
    default:
        break;
    }
}

void PkUpdates::onUpdateFinished(PackageKit::Transaction::Exit status, uint runtime)
{
    Q_UNUSED(runtime);
    if (status == PackageKit::Transaction::ExitSuccess)
    {
        emit updateOk();
        return;
    }
    else
    {
        emit updateFailure();
        qDebug() << "Update package didn't finish successfully";
    }

}

void PkUpdates::onFinished(PackageKit::Transaction::Exit status, uint runtime)
{
    Q_UNUSED(runtime);
    //    qDebug() << __FUNCTION__ ;
    PackageKit::Transaction * trans = qobject_cast<PackageKit::Transaction *>(sender());
    if (!trans)
        return;

    trans->deleteLater();

    if (trans->role() == PackageKit::Transaction::RoleRefreshCache)
    {
        if (status == PackageKit::Transaction::ExitSuccess)
        {
//            qDebug() << "Cache transaction finished successfully";
            return;
        }
        else
        {
            qDebug() << "Cache transaction didn't finish successfully";
        }
    }
    else if (trans->role() == PackageKit::Transaction::RoleGetUpdates)
    {
        if (status == PackageKit::Transaction::ExitSuccess)
        {
            getUpRelease();
        }
        else
        {
            qDebug() << "Check updates transaction didn't finish successfully";
        }
    }
    else if(trans->role() == PackageKit::Transaction::RoleGetPackages)
    {
        if (status == PackageKit::Transaction::ExitSuccess)
        {
            getInstalled();
//            qDebug() << "Total number of Packages Installed: " << insCount() << endl;
        }
        else
        {
            qDebug() << "Get Packages Installed transaction didn't finish successfully";
        }
    }
}


void PkUpdates::getUpRelease()
{
    int releaseAry[m_upNameList.count()];
    int num = 0;
//    qDebug()<< "m_upNameList.count() :" << m_upNameList.count();
//    qDebug()<< "mainJsonFunc->jsonData->classStrMap.count() :" << mainShareData->classStrMap.count();
    QMap<int,CLASSSTRUCT>::iterator item;
    int status;
    for(int i = 0; i < m_upNameList.count(); i++)
    {
        for(item = mainShareData->classStrMap.begin(); item != mainShareData->classStrMap.end(); item++)
        {
            if(m_upNameList.at(i) == item.value().packageName)
            {
                releaseAry[num] = item.value().releaseId;
                num++;
                status = item.value().proStatus;
                if((status != REUPDATE)&&(status != REDOWNLOAD))
                {
                    item.value().proStatus = UPDATE;
                }
            }
        }
    }

    if(num != 0)
    {
        emit updateStatusChanged();
        jsonFunc->getUpdateRelease(releaseAry,num);
        connect(jsonFunc,SIGNAL(updateIsOk()),this,SLOT(sendUpdateData()));
    }
    else
    {
        QString homePath = QDir::homePath();
        QString fileName = homePath + "/.config/.emindappstore/upins.ini";
        QSettings settings(fileName, QSettings::IniFormat);
        QStringList keys = settings.childGroups();
        QString section;
        QString flag;
        int saveCount = keys.count();
        for(int i = 0; i < saveCount; i++)
        {
            section = keys.at(i);
            settings.beginGroup(section);
            flag = settings.value("Flag").toString();
            settings.endGroup();
            if(flag == "UPDATEFLAG")
            {
                settings.remove(section);
            }
        }

    }
}

void PkUpdates::getInstalled()
{
//    qDebug() << __FUNCTION__;
    INSTALLEDSTRUCT instruct;
    QMap<QString,INSTALLEDSTRUCT> installedMap;
    QMap<int,CLASSSTRUCT>::iterator item1;
    QVariantMap::iterator iter;

    for(iter = m_packagesList.begin(); iter != m_packagesList.end(); iter++)
    {
        for(item1 = mainShareData->classStrMap.begin(); item1 != mainShareData->classStrMap.end(); item1++)
        {
            QString packName = PackageKit::Daemon::packageName(iter.key());

            if(packName == item1.value().packageName)
            {
                instruct.appName = item1.value().proName;
                instruct.proImage = item1.value().proImage;
                instruct.exeFile = item1.value().exeFile;
                instruct.productId = item1.key();
                instruct.releaseId = item1.value().releaseId;

                installedMap.insert(iter.key(),instruct);
                int status = item1.value().proStatus;
                if(status == DOWNLOAD)
                {
                    item1.value().proStatus = OPEN;
                }

            }
            else
            {
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
                    if(pkgName == item1.value().packageName)
                    {
                        if(flag == "INSTALLFLAG")
                        {
                            item1.value().proStatus = REDOWNLOAD;
                        }
                        else if(flag == "UPDATEFLAG")
                        {
                            item1.value().proStatus = REUPDATE;
                        }
                        //                    break;
                    }
                }
            }
        }
    }

    QMap<QString,INSTALLEDSTRUCT>::iterator itor;
    QString homePath = QDir::homePath();
    QString fileName = homePath + "/.config/.emindappstore/upins.ini";
    QSettings insdSettings( fileName, QSettings::IniFormat );
    QStringList saveKeys = insdSettings.childGroups();
    QString saveSection;
    QString insdPkgName;
    QString savePkgName;
    QString saveFlag;
    int saveCount = saveKeys.count();

    for(int j = 0; j < saveCount; j++)
    {
        saveSection = saveKeys.at(j);
        insdSettings.beginGroup(saveSection);
        savePkgName = insdSettings.value("PkgName").toString();
        saveFlag = insdSettings.value("Flag").toString();
        insdSettings.endGroup();
        int m = 0;
        for(itor = installedMap.begin(); itor != installedMap.end(); itor++)
        {
            insdPkgName = PackageKit::Daemon::packageName(itor.key());

            if(insdPkgName == savePkgName)
            {
                m++;
                break;
            }
        }
        if(m != 0)
        {
            if(saveFlag == "INSTALLFLAG")
            {
                insdSettings.remove(saveSection);
            }
        }

    }

    emit installStatusChanged();
    emit getInsFinished(installedMap);
}

void PkUpdates::sendUpdateData()
{
    qDebug() << __FUNCTION__;
    emit sigUpdateData(shareData->updateStrMap);
}

void PkUpdates::installUpdate(const QString &packageId)
{
    //    qDebug() << __FUNCTION__ << "packageId == " << packageId;

    PackageKit::Transaction::TransactionFlag flag = PackageKit::Transaction::TransactionFlagOnlyTrusted;
    m_installTrans = PackageKit::Daemon::updatePackage(packageId, flag);

//    connect(m_installTrans.data(), &PackageKit::Transaction::package, this, &PkUpdates::onPackageUpdating);
    connect(m_installTrans.data(), &PackageKit::Transaction::finished, this, &PkUpdates::onUpdateFinished);

}

void PkUpdates::installPackage(QString packageName)
{
    qDebug() << __FUNCTION__ << packageName;
    PackageKit::Transaction *resolveTransaction = PackageKit::Daemon::resolve(packageName,
                                                                              //                                                   PackageKit::Transaction::FilterNone);
                                                                              PackageKit::Transaction::FilterArch);
    connect(resolveTransaction,
            SIGNAL(package(PackageKit::Transaction::Info,QString,QString)),
            this,
            SLOT(packageInstall(PackageKit::Transaction::Info,QString,QString)));
    connect(resolveTransaction,
            SIGNAL(finished(PackageKit::Transaction::Exit, uint)),
            this,
            SLOT(resolveFinished(PackageKit::Transaction::Exit, uint)));

}

void PkUpdates::packageInstall(PackageKit::Transaction::Info, QString packageID, QString summary)
{
    Q_UNUSED(summary);
    //    qDebug() << "packageInstall() packageID " << packageID;
    //    qDebug() << "packageInstall() summary " << summary;
    PackageKit::Transaction *installTransaction = PackageKit::Daemon::installPackage(packageID);
    connect(installTransaction,
            SIGNAL(finished(PackageKit::Transaction::Exit, uint)),
            this,
            SLOT(packageInstallFinished(PackageKit::Transaction::Exit, uint)));
}

void PkUpdates::resolveFinished(PackageKit::Transaction::Exit status, uint runtime)
{
    Q_UNUSED(runtime);
    //    qDebug() << "testResolveFinished() status: " << status << endl;
    //    qDebug() << "testResolveFinished() on of seconds: " << runtime << endl;
    if (status == PackageKit::Transaction::Exit::ExitSuccess)
    {
        qDebug() << "Package Resolve Success!";
    }
    else
    {
        emit installFailure();
        qDebug() << "Package Resolve Failure!";
    }
}

void PkUpdates::packageInstallFinished(PackageKit::Transaction::Exit status, uint runtime)
{
    Q_UNUSED(runtime);
    qDebug() << "packageInstallFinished() status: " << status << endl;
    if (status == PackageKit::Transaction::Exit::ExitSuccess)
    {
        emit installSuccess();
//        qDebug() << "Package Install Success!";
    }
    else
    {
        emit installFailure();
//        qDebug() << "Package Install Failure!";
    }
}

void PkUpdates::onPackageUpdating(PackageKit::Transaction::Info info, const QString &packageID)
{
    Q_UNUSED(info);
    const uint percent = m_installTrans->percentage();
//    qDebug() << "Package updating:" << packageID << "percent == " << percent;
}

QString PkUpdates::transPackSize(const double &size)
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


int PkUpdates::count() const
{
    return m_updateList.count();
}

int PkUpdates::insCount() const
{
    return m_packagesList.count();
}

QVariantMap PkUpdates::packages() const
{
    return m_updateList;
}

QStringList PkUpdates::getPacName() const
{
    return m_upNameList;
}

