#ifndef APPWIDGET_H
#define APPWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPointer>
#include <QNetworkAccessManager>
#include <packagekitqt5/PackageKit/Transaction>
#include <packagekitqt5/PackageKit/Daemon>
#include "../dbus-pkdbus/pkdbusInterface.h"

class AppWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AppWidget(QWidget *parent ,QString headUrl,QString nameStr,QString sizeStr,QString verStr,QString logStr, QString pkgStr);
    bool event(QEvent *event);
    void getImage(QString headUrl);
    QPushButton *getHeadButton();
    QPushButton *getNameButton();
    QPushButton *getFuncButton();
    QPushButton *getUpdateButton();
    QString getChangeLog();
    QLabel *getIntroLabel();
    QString getIntroStr();
    QString getNewFunStr();
    QString getPkgId();
    QString getHeadUrl();
    QString getAppName();
    QString getAppVer();
    QString getAppSize();
    QString getExeFile();
    int getProductId();
    void setProductId(int productid);
    void setExeFile(QString exefile);
    int getReleaseId();
    void setReleaseId(int relid);
    QString getPkgName();
    void setPkgName(QString pkgname);
    void installUpdate(const QString &packageId);
    void setBtnMetric(QString btnText,QPushButton *pushButton);

signals:
    void sigIntroResize();
    void appUpdateFinished();
    void appUpdateFailure();
    void updateOk();
    void updateFailure();

public slots:
    void getImageFinished(QNetworkReply *reply);
    void onUpdateFinished(bool status);

private:
    QHBoxLayout *appLayout;
    QVBoxLayout *rightLayout;
    QHBoxLayout *bottomLayout;
    QPushButton *headButton;
    QPushButton *nameButton;
    QLabel *introLabel;
    QLabel *segLabel;
    QString introstr;
    QString newFuncStr;
    QPushButton *funcButton;
    QLabel *sizeLabel;
    QLabel *versionLabel;
    QPushButton *updateButton;
    QPixmap pix;
    QString changeLog;
    QString pkgId;
    QNetworkAccessManager *imageUpdate;
    QString iconUrl;
    QString appName;
    QString appVersion;
    QString appSize;
    QString exeFile;
    int releaseId;
    QString pkgName;
    int productId;
    QPointer<PackageKit::Transaction> m_updatesTrans;


};

#endif // APPWIDGET_H
