#include "managerwidget.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <Details>

ManagerWidget::ManagerWidget(QWidget *parent, QString urlstr, QString namestr, QString verstr, QString sizestr) : QWidget(parent),appName(namestr),headUrl(urlstr),appVersion(verstr)

{
    shareData = new ShareData();
    jsonFunc = new JSONFUNC(shareData);

    hbLayout = new QHBoxLayout();
    hbLayout->setMargin(0);
    vbLayout = new QVBoxLayout();
    vbLayout->setMargin(0);

    headButton = new QPushButton();
    headButton->setObjectName("headButton");
    headButton->setFlat(true);
    headButton->setFixedSize(64,64);
    if(urlstr != "")
    {
        QString homePath = QDir::homePath();
        QString iconPath = homePath + "/.config/.emindappstore/icon/";
        QStringList urlList = headUrl.split("/");
        int count = urlList.count();
        QString imageName = urlList.at(count - 1);
        QString imagePath = iconPath + imageName;
        QFile file;
        bool fileFlag = file.exists(imagePath);
        if(fileFlag)
        {
            QPixmap pixmap(imagePath);
            headButton->setIcon(pixmap);
            headButton->setIconSize(QSize(64,64));
            //            headButton->setFixedSize(24,24);
        }
        else
        {
            getImage(urlstr);
        }

    }

    nameButton = new QPushButton();
    nameButton->setObjectName("nameButton");
    nameButton->setFlat(true);
    nameButton->setMaximumWidth(120);
    setBtnMetric(namestr, nameButton);

    verLabel = new QLabel();
    verLabel->setObjectName("versionLabelMan");
    verLabel->setText(verstr);

    sizeLabel = new QLabel();
    sizeLabel->setObjectName("sizeLabelMan");
    QString size;
    if(sizestr != "")
    {
        size = tr("Size : ") + sizestr;
    }
    else
    {
        size = tr("Size : ") + tr("Calculating...");
    }
    sizeLabel->setText(size);

    uninsButton = new QPushButton();
    uninsButton->setObjectName("uninsButton");
    uninsButton->setText(tr("Uninstall"));
    uninsButton->setFixedSize(80,32);

    managerButton = new QPushButton();
    managerButton->setObjectName("managerButton");
    managerButton->setText(tr("Open"));
    managerButton->setFixedSize(80,32);

    hbLayout->addWidget(headButton);
    hbLayout->addSpacing(5);
    hbLayout->addLayout(vbLayout);
    hbLayout->addStretch();
    hbLayout->addWidget(uninsButton);
    hbLayout->addWidget(managerButton);

    vbLayout->addSpacing(9);
    vbLayout->addWidget(nameButton);
    vbLayout->addSpacing(4);
    vbLayout->addWidget(verLabel);
    vbLayout->addSpacing(0);
    vbLayout->addWidget(sizeLabel);
    vbLayout->addStretch();

    setLayout(hbLayout);

    connect(headButton,SIGNAL(clicked()),this,SLOT(toDetailSig()));
    connect(nameButton,SIGNAL(clicked()),this,SLOT(toDetailSig()));

    ComEmindsoftPkdbusRegistryInterface *myInterface= new ComEmindsoftPkdbusRegistryInterface(QString(),QString(),QDBusConnection::systemBus(),this);
    QObject::connect(myInterface,SIGNAL(isInstallSuccess(bool)),this,SLOT(packageInstallFinished(bool)));
    QObject::connect(myInterface,SIGNAL(isPacRmvSuccess(bool)),this,SLOT(removeFinished(bool)));
}

QPushButton * ManagerWidget::getButton(int num)
{
    switch (num)
    {
    case UNINSBUTTON:
        return uninsButton;
    case MANAGERBUTTON:
        return managerButton;
    case NAMEBUTTON:
        return nameButton;
    case HEADBUTTON:
        return headButton;
    default:
        return NULL;
    }
}

void ManagerWidget::setVerLabel(QString ver)
{
    verLabel->setText(ver);
}

void ManagerWidget::setSizeLabel(QString size)
{
    sizeLabel->setText(size);
}

void ManagerWidget::setManagerButton(QString manastr)
{
    managerButton->setText(manastr);
}

void ManagerWidget::getImage(QString headUrl)
{
    imageManager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(headUrl));
    connect(imageManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
    imageManager->get(request);
}

void ManagerWidget::resolveNameToId(QString packName)
{
    PackageKit::Transaction *transaction = PackageKit::Daemon::resolve(packName,
                                                                       //                                                                       PackageKit::Transaction::FilterNone);
                                                                       PackageKit::Transaction::FilterArch);
    connect(transaction,
            SIGNAL(package(PackageKit::Transaction::Info,QString,QString)),
            SLOT(resetPackId(PackageKit::Transaction::Info,QString,QString)));

}

void ManagerWidget::removePackage(QString pacId)
{
    com::emindsoft::pkdbus::registry pkDbus("com.emindsoft.pkdbus",
                                       "/pkdbus/registry",
                                       QDBusConnection::systemBus());

    pkDbus.removePackage(pacId);
}

void ManagerWidget::setInstallSize(QString pacId)
{
    detailTransaction = PackageKit::Daemon::getDetails(pacId);
    connect(detailTransaction,
            SIGNAL(details(const PackageKit::Details &)),
            this,
            SLOT(onGetDetails(const PackageKit::Details &)));
}

void ManagerWidget::onGetDetails(const PackageKit::Details &value)
{
    double size = (double)value.size();
    QString insSize = transPackSize(size);
    QString sizeStr = tr("Size : ") + insSize;
    sizeLabel->setText(sizeStr);
}

QString ManagerWidget::transPackSize(const double &size)
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

void ManagerWidget::removeFinished(bool status)
{
    qDebug() << "packageFinished() status: " << status << endl;
    if (status)
    {
        qDebug() << "Package remove" << pkgId <<  "success! ^_^";
        emit pacRmvSuccess(pkgId);
    }
    else
    {
        emit pacRmvFailure(pkgId);
        qDebug() << "Package remove Failure! ~_~";
    }
}

void ManagerWidget::resetPackId(PackageKit::Transaction::Info, QString packageID, QString summar)
{
    qDebug() << "The new package is: " << packageID << summar;
    pkgId = packageID;
    emit resetPackageIdSuccess(packageID);
}

void ManagerWidget::replyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(bytes);
        headButton->setIcon(pixmap);
        headButton->setIconSize(QSize(64,64));

        QStringList urlList = headUrl.split("/");
        int count = urlList.count();
        QString imageName = urlList.at(count - 1);

        QString homePath = QDir::homePath();
        QString iconPath = homePath + "/.config/.emindappstore/icon/";
        QDir dir(iconPath);
        if (!dir.exists())
        {
            QDir mkDir;
            bool mkdirFlag = mkDir.mkdir(iconPath);
            Q_UNUSED(mkdirFlag);
            //            if(mkdirFlag)
            //            {
            //                qDebug() << "Mkdir success!";
            //            }
            //            else
            //            {
            //                qDebug() << "Mkdir Failure!";
            //            }
        }

        QString fileName = iconPath + imageName;

        bool saveFlag = pixmap.save(fileName);
        Q_UNUSED(saveFlag);
        //        if(saveFlag)
        //        {
        //            qDebug() << "Save image success!";
        //        }
        //        else
        //        {
        //            qDebug() << "Save image Failure!";
        //        }

    }
}

void ManagerWidget::installPackage(QString packageName)
{
    QString str = QString(": %1").arg(packageName);
    qDebug() << __FUNCTION__ << str;

    com::emindsoft::pkdbus::registry pkDbus("com.emindsoft.pkdbus",
                                       "/pkdbus/registry",
                                       QDBusConnection::systemBus());

    pkDbus.installPackage(packageName);
}

void ManagerWidget::packageInstallFinished(bool status)
{
    qDebug() << "packageInstallFinished() status: " << status << endl;
    if (status)
    {
        emit installSuccess(appName);
        qDebug() << "Package Install Success!";
    }
    else
    {
        emit installFailure(appName);
        qDebug() << "Package Install Failure!";
    }
}

void ManagerWidget::toDetailSig()
{
    emit detailspageSig(productId);
}

void ManagerWidget::setBtnMetric(QString btnText,QPushButton *pushButton)
{
    QFontMetrics metric(btnText);
    QString nameStr = metric.elidedText(btnText,Qt::ElideRight,pushButton->width());
    pushButton->setText(nameStr);
    pushButton->setToolTip(btnText);
}

QLabel *ManagerWidget::getLabel()
{
    return sizeLabel;
}

QString ManagerWidget::getPkgId()
{
    return pkgId;
}

void ManagerWidget::setPkgId(QString packId)
{
    pkgId = packId;
}

QString ManagerWidget::getPkgName()
{
    return pkgName;
}

void ManagerWidget::setPkgName(QString packName)
{
    pkgName = packName;
}

void ManagerWidget::setAppName(QString appname)
{
    appName = appname;
}

QString ManagerWidget::getAppName()
{
    return appName;
}

void ManagerWidget::setHeadUrl(QString iconUrl)
{
    headUrl = iconUrl;
}

QString ManagerWidget::getHeadUrl()
{
    return headUrl;
}

void ManagerWidget::setVersion(QString version)
{
    appVersion = version;
}

QString ManagerWidget::getVersion()
{
    return appVersion;
}

void ManagerWidget::setFlag(QString flag)
{
    insUpFlag = flag;
}

QString ManagerWidget::getFlag()
{
    return insUpFlag;
}

void ManagerWidget::setReleaseId(int relid)
{
    releaseId = relid;
}

int ManagerWidget::getReleaseId()
{
    return releaseId;
}

void ManagerWidget::setExeFile(QString exefile)
{
    exeFile = exefile;
}

QString ManagerWidget::getExeFile()
{
    return exeFile;
}

int ManagerWidget::getProductId()
{
    return productId;
}

void ManagerWidget::setProductId(int productid)
{
    productId = productid;
}




