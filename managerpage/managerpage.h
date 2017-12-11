#ifndef MANAGERPAGE_H
#define MANAGERPAGE_H

#include <QWidget>
#include <QEvent>
#include <QtDBus>
#include <QDBusConnection>
#include "updatepage/taskbar.h"
#include "managerpage/managerwidget.h"
#include "../updatepage/pkupdates.h"
#include "updatepage/updatepage.h"

class ManagerPage : public QWidget
{
    Q_OBJECT
public:
    explicit ManagerPage(QWidget *parent , JSONFUNC *json, ShareData *sharedata);
    ~ManagerPage();
    void createManagerWindow();
    bool event(QEvent *event);
    void saveConfigFile(QString headUrl, QString appName, QString appVersion, QString appSize, QString flag, QString exefile, int releaseId, QString packageName, int productid);
    void readConfigFile();

private:
    QWidget *wid;
    QVBoxLayout *vboxLayout;
    QVBoxLayout *vboxLayout1;
    QVBoxLayout *vboxLayout2;
    TaskBar *manTaskBar;
    TaskBar *manCompBar;
    QTableWidget *installTable;
    QTableWidget *compTable;
    QLabel *splitLabel;
    ManagerWidget *compManager;
    QScrollArea *manScroArea;
    PkUpdates *updm;
    UpdatePage *updpg;
    ShareData *shareData;    
    JSONFUNC *jsonFunc;

signals:
    void upBtnClicked(QString);
    void insBtnClicked(QString,int);
    void sigInstallSuccess(QString,bool,int);
    void appInstallOk(QString,QString,QString,QString,QString,int);
    void removePackageSuccess(QString,bool,int);
    void installStatusChanged();
    void sigReUpdate(QString,bool,int);
    void sigReDownload(QString,bool,int);
    void sigUninstalling(QString,bool,int);
    void detailspageSig(int);
    void toGetDetail(QMap<QString,INSTALLEDSTRUCT>);  //for start pkdetail thread to get package installed size
    void setInsdSizeOk();   //for enable refresh button

public slots:
    void onGetInsFinished(QMap<QString, INSTALLEDSTRUCT> insdMap);
    void onAppUpdate(QString iconUrl, QString appName, QString appVersion, QString appSize, int relid, QString exefile, QString pkgname, int productid);
    void onAppUpdateOk(QString appName);
    void onAppUpdateFailure(QString appName);
    void onAppInstallSuccess(QString appName);
    void onAppInstallFailure(QString appName);
    void compBtnClicked();
    void upInsToInsd(QString appName, QString iconUrl, QString appVer, QString exefile, QString pkgName, int productId);
    void deleteRmvRow(QString pacId);
    void onRemoveFailure(QString pacId);
    void getPackageInstalled();
    void readInsdConfig();
    void insManBtnClicked();
    void allStartBtnclicked();
    void getInstallRlease(QString appName, int releaseId);
    void onAppInstall();
//    void installPackage(QString pacName);
    void insReleaseError();
    void onStatusChanged();
    void sendSigReUpdate(QString appName);
    void sendSigReDownload(QString appName);
    void toDetailSig(int productId);
    void setInsdSize(QMap<QString, QString> insdSizeMap);

};

#endif // MANAGERPAGE_H
