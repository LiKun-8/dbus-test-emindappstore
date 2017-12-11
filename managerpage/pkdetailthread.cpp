#include "pkdetailthread.h"


PkDetailThread::PkDetailThread(QObject *parent, QStringList pkgIdList):QObject(parent),pkIdList(pkgIdList)
{

}

void PkDetailThread::toGetSize()
{
    sizeMap.clear();
    pkgIdCount = pkIdList.count();
    QString pkgId;
    for(int i = 0; i < pkgIdCount; i++)
    {
        pkgId = pkIdList.at(i);
        setInstallSize(pkgId);
    }
}

void PkDetailThread::setInstallSize(QString pacId)
{
//    qDebug() << __FUNCTION__;
    PackageKit::Transaction *detailTransaction = PackageKit::Daemon::getDetails(pacId);
    connect(detailTransaction,
            SIGNAL(details(const PackageKit::Details &)),
            this,
            SLOT(onGetDetail(const PackageKit::Details &)),Qt::DirectConnection);
}

void PkDetailThread::onGetDetail(const PackageKit::Details &value)
{
    double size = (double)value.size();
    QString pacId = value.packageId();
    QString insSize = transPackSize(size);
    QString sizeStr = tr("Size : ") + insSize;
    sizeMap.insert(pacId, sizeStr);
    if(pkgIdCount == sizeMap.count())
    {
        emit getPkgSizeOk(sizeMap);
    }
}

QString PkDetailThread::transPackSize(const double &size)
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

