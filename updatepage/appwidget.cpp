#include "appwidget.h"
#include <QDebug>
#include <QPalette>
#include <QEvent>
#include <QNetworkRequest>
#include <QNetworkReply>

AppWidget::AppWidget(QWidget *parent, QString headUrl, QString nameStr, QString sizeStr, QString verStr, QString logStr, QString pkgStr) : QWidget(parent),changeLog(logStr),pkgId(pkgStr),iconUrl(headUrl),appName(nameStr),appVersion(verStr),appSize(sizeStr)
{
    appLayout = new QHBoxLayout();
    appLayout->setMargin(0);
    rightLayout = new QVBoxLayout();
    rightLayout->setMargin(0);
    bottomLayout = new QHBoxLayout();
    bottomLayout->setMargin(0);

    headButton = new QPushButton();
    headButton->setObjectName("headButton");
    headButton->setFlat(true);
    headButton->setFixedSize(64,64);
    getImage(headUrl);

    nameButton = new QPushButton();
    nameButton->setObjectName("nameButton");
    nameButton->setFlat(true);
    nameButton->setMaximumWidth(120);
    setBtnMetric(nameStr,nameButton);

    QStringList logList = logStr.split("#");
    introstr = logList.at(0);
    QString newfuncstr = logList.at(1);
    QStringList newfuncList = newfuncstr.split("*");
    int count = newfuncList.count();
    for(int i = 0; i < count; i++)
    {
        if(i != (count -1))
        {
            newFuncStr += "- " + newfuncList.at(i) + "\n";
        }
        else
        {
            newFuncStr += "- " + newfuncList.at(i);
        }
    }
    introLabel = new QLabel();
    introLabel->setObjectName("introLabel");
    introLabel->setFrameStyle(QFrame::NoFrame);
    introLabel->setText(introstr);

    segLabel = new QLabel();
    segLabel->setObjectName("segLabel");
    segLabel->setFixedSize(1,15);

    funcButton = new QPushButton();
    funcButton->setObjectName("funcButton");
    funcButton->setFlat(true);
    funcButton->setText(tr("New Function"));
    funcButton->setFlat(true);
    funcButton->setMaximumWidth(90);

    sizeLabel = new QLabel();
    sizeLabel->setObjectName("sizeLabelApp");
    sizeLabel->resize(16,64);
    sizeLabel->setText(sizeStr);

    versionLabel = new QLabel();
    versionLabel->setObjectName("versionLabelApp");
    versionLabel->setText(verStr);

    updateButton = new QPushButton();
    updateButton->setObjectName("updateButton");
    updateButton->setFixedSize(80,32);
    updateButton->setText(tr("Update"));

    appLayout->addWidget(headButton);
    appLayout->setSpacing(16);
    appLayout->addLayout(rightLayout);
    rightLayout->addStretch(24);
    rightLayout->addWidget(nameButton);
    rightLayout->setSpacing(3);
    rightLayout->addLayout(bottomLayout);
    rightLayout->addStretch(24);
    bottomLayout->addWidget(introLabel);
    bottomLayout->addSpacing(5);
    bottomLayout->addWidget(segLabel);
    bottomLayout->addSpacing(5);
    bottomLayout->addWidget(funcButton);
    bottomLayout->addStretch();
    appLayout->addWidget(sizeLabel);
    appLayout->addSpacing(112);
    appLayout->addWidget(versionLabel);
    appLayout->addSpacing(112);
    appLayout->addWidget(updateButton);
    setLayout(appLayout);

    ComEmindsoftPkdbusRegistryInterface *myInterface= new ComEmindsoftPkdbusRegistryInterface(QString(),QString(),QDBusConnection::systemBus(),this);
    QObject::connect(myInterface,SIGNAL(isUpdateSuccess(bool)),this,SLOT(onUpdateFinished(bool)));
}

void AppWidget::getImage(QString headUrl)
{
    imageUpdate = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(headUrl));
    connect(imageUpdate, SIGNAL(finished(QNetworkReply *)), this, SLOT(getImageFinished(QNetworkReply *)));
    imageUpdate->get(request);
}

void AppWidget::getImageFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(bytes);
        headButton->setIcon(pixmap);
        headButton->setIconSize(QSize(64,64));
    }
}

void AppWidget::installUpdate(const QString &packageId)
{
    qDebug() << __FUNCTION__ << "packageId == " << packageId;
    com::emindsoft::pkdbus::registry pkDbus("com.emindsoft.pkdbus",
                                       "/pkdbus/registry",
                                       QDBusConnection::systemBus());

    pkDbus.installUpdate(packageId);

}

void AppWidget::onUpdateFinished(bool status)
{
    if (status)
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

void AppWidget::setBtnMetric(QString btnText,QPushButton *pushButton)
{
    QFontMetrics metric(btnText);
    QString nameStr = metric.elidedText(btnText,Qt::ElideRight,pushButton->width());
    pushButton->setText(nameStr);
    pushButton->setToolTip(btnText);
}

bool AppWidget::event(QEvent *event)
{
    if(event->type() == QEvent::Resize )
    {
        emit sigIntroResize();
        return true;
    }
    return QWidget::event(event);
}

QPushButton* AppWidget::getHeadButton()
{
    return headButton;
}

QPushButton* AppWidget::getNameButton()
{
    return nameButton;
}

QPushButton* AppWidget::getFuncButton()
{
    return funcButton;
}

QPushButton* AppWidget::getUpdateButton()
{
    return updateButton;
}

QString AppWidget::getChangeLog()
{
    return changeLog;
}

QLabel *AppWidget::getIntroLabel()
{
    return introLabel;
}

QString AppWidget::getIntroStr()
{
    return introstr;
}

QString AppWidget::getNewFunStr()
{
    return newFuncStr;
}

QString AppWidget::getPkgId()
{
    return pkgId;
}
QString AppWidget::getHeadUrl()
{
    return iconUrl;
}

QString AppWidget::getAppName()
{
    return appName;
}

QString AppWidget::getAppVer()
{
    return appVersion;
}

QString AppWidget::getAppSize()
{
    return appSize;
}

QString AppWidget::getExeFile()
{
    return exeFile;
}

int AppWidget::getProductId()
{
    return productId;
}

void AppWidget::setProductId(int productid)
{
    productId = productid;
}

void AppWidget::setExeFile(QString exefile)
{
    exeFile = exefile;
}

int AppWidget::getReleaseId()
{
    return releaseId;
}

void AppWidget::setReleaseId(int relid)
{
    releaseId = relid;
}

QString AppWidget::getPkgName()
{
    return pkgName;
}

void AppWidget::setPkgName(QString pkgname)
{
    pkgName = pkgname;
}

