#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <qglobal.h>

class DataManager : public QObject
{
    Q_OBJECT
public:
    explicit DataManager(QObject *parent = 0);

    Q_PROPERTY(int sproductId READ sproductId WRITE setSproductId NOTIFY sproductIdChanged)
    void setSproductId(const int& productId);
    int sproductId();

    Q_PROPERTY(QString serverIp READ serverIp WRITE setServerIp NOTIFY serverIpChanged)
    void setServerIp(const QString& sIp);
    QString serverIp();

    Q_PROPERTY(int bannerProId READ bannerProId WRITE setBannerProId NOTIFY bannerProIdChanged)
    void setBannerProId(const int& bannerPid);
    int bannerProId();

    void getScreenImg();
    Q_INVOKABLE void toDtPage();


signals:
    void sigGetScreenImg();
    void sproductIdChanged();
    void serverIpChanged();
    void bannerProIdChanged();
    void sigToDetailPg(int);

public slots:



private:
    int mProductId;
    QString mServerIp;
    int mBannerPid;

};

#endif // DATAMANAGER_H
