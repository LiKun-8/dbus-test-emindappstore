#ifndef PKUPDATES_H
#define PKUPDATES_H

#include <QObject>
#include <QPointer>
#include <QStringList>
#include <packagekitqt5/PackageKit/Transaction>
#include <packagekitqt5/PackageKit/Daemon>
#include "../jsonfunc.h"
#include "../sharedata.h"


struct INSTALLEDSTRUCT{
    QString appName;
    QString proImage;
    QString exeFile;
    int productId;
    int releaseId;
};

class PkUpdates : public QObject
{
    Q_OBJECT
public:
    explicit PkUpdates(QObject *parent , JSONFUNC *jsonfunc, ShareData *sharedata);
    int count() const;
    int insCount() const;
    QStringList getPacName() const;
    QVariantMap packages() const;
    void installUpdate(const QString &packageId);
    void getUpdateData();
    void getMagData();
    void installPackage(QString packageName);
    QString transPackSize(const double &size);
    ShareData *mainShareData;
    ShareData *shareData;
    JSONFUNC *mainJsonFunc;
    JSONFUNC *jsonFunc;
signals:
    void updatesChanged();
    void getUpdFinished(QStringList);
    void getInsFinished(QMap<QString,INSTALLEDSTRUCT>);
    void upReleaseAry(int *,int);
    void sigUpdateData(UPDATESTRUCTMAP);
    void updateOk();
    void updateFailure();
    void installSuccess();
    void installFailure();
    void updateStatusChanged();
    void installStatusChanged();

public slots:
    Q_INVOKABLE void checkUpdates(bool force = true);
    void packageInstall(PackageKit::Transaction::Info, QString packageID, QString summary);
    void packageInstallFinished(PackageKit::Transaction::Exit status, uint runtime);
    void resolveFinished(PackageKit::Transaction::Exit status, uint runtime);


private slots:
    void onFinished(PackageKit::Transaction::Exit status, uint runtime);
    void onUpdateFinished(PackageKit::Transaction::Exit status, uint runtime);
    void getUpdates();
    void getPackages();
    void onPackage(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary);
    void onGetPackages(PackageKit::Transaction::Info info, const QString &packageID, const QString &summary);
    void getUpRelease();
    void getInstalled();
    void sendUpdateData();
    void onPackageUpdating(PackageKit::Transaction::Info info, const QString &packageID);

private:
    void onChanged();
    void onUpdatesChanged();
    void onStatusChanged();
    QPointer<PackageKit::Transaction> m_updatesTrans;
    QPointer<PackageKit::Transaction> m_cacheTrans;
    QPointer<PackageKit::Transaction> m_packagesTrans;
    QPointer<PackageKit::Transaction> m_installTrans;
//    QPointer<PackageKit::Transaction> m_detailTrans;
    QVariantMap m_updateList;
    QVariantMap m_packagesList;
    QStringList m_upNameList;
    int sigFlag;
//    int releseAry[MAXUPNUM];

};

#endif // PKUPDATES_H
