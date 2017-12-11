#include "element.h"
#include "qreplytimeout.h"
#include <QFont>
#include <QDebug>
#include "sharedata.h"

Element::Element()
{
    this->setFixedSize(144,74);

    vbLayout = new QVBoxLayout();
    hbLayout = new QHBoxLayout();

    btnImage = new QPushButton(this);
    btnImage->setFixedSize(64,64);
    btnName = new QPushButton(this);
    btnName->setFixedSize(72,22);
    btnName->setStyleSheet("text-align: left;");//设置按钮文字显示位置-左对齐
    btnImage->setFlat(true);
    btnName->setFlat(true);
    vbLayout->addWidget(btnName);
    vbLayout->setMargin(0);

    hbLayout->addWidget(btnImage);
    btnStatus = new QPushButton(this);
    btnStatus->setFixedSize(72,24);
    //        btnStatus->setFlat(true);

    hbStartLayout = new QHBoxLayout();
    hbStartLayout->setContentsMargins(0,0,0,0);
    hbStartLayout->setSpacing(0);

    labelStar = new QLabel[5];
    for(int i=0;i<5;i++)
    {
        hbStartLayout->addWidget(&labelStar[i]);
    }
    hbStartLayout->addSpacing(24);

    vbLayout->addLayout(hbStartLayout);
    vbLayout->addSpacing(5);
    vbLayout->addWidget(btnStatus);
    connect(btnStatus,SIGNAL(clicked(bool)),this,SLOT(btnStatusSlot()));
    connect(btnImage,SIGNAL(clicked(bool)),this,SLOT(btnImageSlot()));
    connect(btnName,SIGNAL(clicked(bool)),this,SLOT(btnNameSlot()));

    //去除矩形虚线框
    btnName->setFocusPolicy(Qt::NoFocus);
    btnName->setObjectName("btnName");
    btnImage->setFocusPolicy(Qt::NoFocus);
    btnImage->setObjectName("btnImage");
    btnStatus->setFocusPolicy(Qt::NoFocus);
    btnStatus->setCursor(Qt::PointingHandCursor);
    btnStatus->setObjectName("btnStatus");
    btnName->setCursor(Qt::PointingHandCursor);
    btnImage->setCursor(Qt::PointingHandCursor);
    hbLayout->addLayout(vbLayout);
    hbLayout->setMargin(0);
    this->setLayout(hbLayout);

    m_ImageManager = new QNetworkAccessManager();
    m_ImagePix = new QPixmap();
    m_Flag = 0;
    productId = 0;
    proStatus = 0;
    this->installEventFilter(this);
    connect(m_ImageManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)),Qt::QueuedConnection);
}

Element::~Element()
{
    delete[] labelStar;
    delete hbStartLayout;
    delete vbLayout;
    delete hbLayout;
}

void Element::setBtnImage(QString imagePath)
{
    m_ImagePath = imagePath;
    m_Flag = 1;
}

void Element::setBtnName(QString name)
{
    //使文本生成省略号(...)
    QFontMetrics metrice(btnName->font());
    QString nameText = metrice.elidedText(name,Qt::ElideRight,btnName->width());
    btnName->setText(nameText);
    btnName->setToolTip(name);
}

void Element::setProStatus(int status)
{
    QString text;
    proStatus = status;
    if(status == DOWNLOAD)
    {
        text = tr("DownLoad");
    }
    else if(status == UPDATE)
    {
        text = tr("Open");
    }
    else if(status == OPEN)
    {
        text = tr("Open");
    }
    else if(status == REDOWNLOAD)
    {
        text = tr("ReDownLoad");
    }
    else if(status == REUPDATE)
    {
        text = tr("Open");
    }
    else if(status == UNINSTALLING)
    {
        text = tr("UnInstalling...");
    }
    else if(status == DOWNLOADING)
    {
        text = tr("DownLoading...");
    }
    else if(status == UPDATING)
    {
        text = tr("UpDating...");
    }
    else
    {
        qDebug("product status is error");
        text = tr("DownLoad");
    }
    btnStatus->setText(text);
}
void Element::updateProStatus(bool status, int flag)
{
    QString text;
    if(flag == DOWNLOAD || flag == UPDATE)
    {
        if(status)
        {
            text = tr("Open");
            proStatus = OPEN;
            btnStatus->setEnabled(true);
        }
        else
        {
            if(proStatus == DOWNLOAD || proStatus == REDOWNLOAD || proStatus == DOWNLOADING)
            {
                text = tr("ReDownLoad");
                proStatus = REDOWNLOAD;
                btnStatus->setEnabled(true);
            }
            else if(proStatus == UPDATE || proStatus == REUPDATE || proStatus == UPDATING)
            {
                text = tr("Open");
                proStatus = OPEN;
                btnStatus->setEnabled(true);
            }
        }
    }
    else if(flag == REDOWNLOAD)
    {
        text = tr("ReDownLoad");
        proStatus = REDOWNLOAD;
        btnStatus->setEnabled(true);
    }
    else if(flag == DOWNLOADING)
    {
        text = tr("DownLoading...");
        proStatus = DOWNLOADING;
    }
    else if(flag == UNINSTALL)
    {
        if(status)
        {
            text = tr("DownLoad");
            proStatus =DOWNLOAD;
            btnStatus->setEnabled(true);
        }
        else
        {
            text = tr("Open");
            proStatus = OPEN;
            btnStatus->setEnabled(true);
        }
    }
    else if(flag == UNINSTALLING)
    {
        text = tr("UnInstalling...");
        proStatus =UNINSTALLING;
        btnStatus->setEnabled(false);
    }
    else if(flag == UPDATING)
    {
        text = tr("UpDating...");
        proStatus =UPDATING;
        btnStatus->setEnabled(false);
    }
    btnStatus->setText(text);
}

void Element::setReleaseId(int id)
{
    releaseId = id;
}

//1-下载,2-更新,3-打开
void Element::btnStatusSlot()
{
//    qDebug()<<__FUNCTION__<<" proStatus = "<<proStatus<<endl;
    if(proStatus == DOWNLOAD || proStatus == REDOWNLOAD)
    {
        btnStatus->setText(tr("DownLoading..."));
        btnStatus->setEnabled(false);
        emit installPackage(btnName->toolTip(),releaseId);
    }
    else if(proStatus == UPDATE || proStatus == REUPDATE)
    {
        btnStatus->setText(tr("UpDating..."));
        btnStatus->setEnabled(false);
        emit updatePackage(btnName->toolTip(),releaseId);
    }
    else if(proStatus == OPEN)
    {
        process.start(exebleFile);
    }
    else
    {
        qDebug("product status is error!");
    }
}

void Element::btnNameSlot()
{
    emit detailspageSig(productId);
}

void Element::btnImageSlot()
{
    emit detailspageSig(productId);
}

void Element::setcategory(int cate)
{
    category = cate;
}

void Element::setProductId(int id)
{
    productId = id;
}

void Element::replyFinished(QNetworkReply *reply)
{
    QReplyTimeout *pTimeout = new QReplyTimeout(reply, 1000);
    // 超时进一步处理
    connect(pTimeout, &QReplyTimeout::timeout, [=]() {
        qDebug() << "Timeout";
    });

    if(reply->error() == QNetworkReply::NoError)
    {
        m_ImagePix->loadFromData(reply->readAll());
        btnImage->setIcon(*m_ImagePix);
        btnImage->setIconSize(btnImage->size());
    }
    else
    {
        qDebug()<<"reply is error!"<<endl;
    }

    reply->deleteLater();
}

void Element::initStar(double num)
{ 
    for(int i = 0;i < 5;i++)
    {
        labelStar[i].setFixedSize(8,8);
        labelStar[i].setPixmap(QPixmap(":/image/star_0.png"));
    }

    for(int i = 0;i < (int)num;i++)
    {
        labelStar[i].setPixmap(QPixmap(":/image/star_100.png"));
    }

    if(num - (int)num > 0)
    {
        labelStar[(int)num].setPixmap(QPixmap(":/image/star_50.png"));
    }
}

void Element::setExeFile(QString exe)
{
    exebleFile = exe;
}

QString Element::getBtnName()
{
    return btnName->toolTip();
}

bool Element::eventFilter(QObject *target, QEvent *event)
{
        if(event->type() == QEvent::Paint)
        {
            if(m_Flag == 1)
            {
                m_Flag = 0;
                m_ImageManager->get(QNetworkRequest(QUrl(m_ImagePath)));
            }
            return true;
        }

    return QWidget::eventFilter(target,event);
}


