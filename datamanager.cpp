#include "datamanager.h"
#include <QSettings>
#include <QDebug>

DataManager::DataManager(QObject *parent) : QObject(parent)
{
    QSettings address("/usr/share/emindappstore/ipaddress",QSettings::IniFormat);
    QString ipaddress = "http://" + address.value("Ip/address").toString() + ":8888/";
    setServerIp(ipaddress);
}

void DataManager::setSproductId(const int &productId)
{
    mProductId = productId;
    emit sproductIdChanged();
}

int DataManager::sproductId()
{
    return mProductId;
}

void DataManager::setServerIp(const QString &sIp)
{
    mServerIp = sIp;
    emit serverIpChanged();
}

QString DataManager::serverIp()
{
    return mServerIp;
}

void DataManager::setBannerProId(const int &bannerPid)
{
    mBannerPid = bannerPid;
    bannerProIdChanged();
}

int DataManager::bannerProId()
{
    return mBannerPid;
}

void DataManager::getScreenImg()
{
    qDebug()<<"getScreenImg=============================";
    emit sigGetScreenImg();
}

void DataManager::toDtPage()
{
    emit sigToDetailPg(mBannerPid);
}
