#ifndef PKDETAILTHREAD_H
#define PKDETAILTHREAD_H

#include <QObject>
#include <packagekitqt5/PackageKit/Transaction>
#include <packagekitqt5/PackageKit/Daemon>
#include <QCoreApplication>
#include <QDebug>
#include <Details>
#include <QMap>

class PkDetailThread : public QObject
{
    Q_OBJECT
public:
    explicit PkDetailThread(QObject *parent, QStringList pkgIdList);
    void setInstallSize(QString pacIds);
    QString transPackSize(const double &size);
    QMap<QString,QString> sizeMap;
private:
    QStringList pkIdList;
    int pkgIdCount;
    QMap<QString, QString> insdSizeMap;

signals:
    void getPkgSizeOk(QMap<QString, QString>);

public slots:
    void onGetDetail(const PackageKit::Details &value);
    void toGetSize();
};

#endif // PKDETAILTHREAD_H
