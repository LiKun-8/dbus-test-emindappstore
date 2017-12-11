#include "morepage.h"

MorePage::MorePage(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);
    vbClasslayoutMore = new QVBoxLayout();
    scrollMore = new QScrollArea();
    pageMoreWidget = new QWidget();
    pageMoreWidget->setMaximumWidth(1200);
    //scrollMore->setMaximumWidth(1200);
    showMore = new ShowMore();
    connect(showMore,SIGNAL(installApp(QString,int)),this,SLOT(sendInstallPackage(QString,int)));
    connect(showMore,SIGNAL(updateApp(QString,int)),this,SLOT(sendUpdatePackage(QString,int)));
    connect(showMore,SIGNAL(detailspageSig(int)),this,SLOT(sendDetailSig(int)));
    //去除pageMoreWidget布局重复添加的问题
    mainLayout->addWidget(scrollMore);
    mainLayout->setContentsMargins(0,0,5,0);
    this->setLayout(mainLayout);
//    this->layout()->addWidget(scrollMore);
}

MorePage::~MorePage()
{
    delete vbClasslayoutMore;
    delete showMore;
    delete pageMoreWidget;
    delete scrollMore;
    vbClasslayoutMore = NULL;
}

void MorePage::createMorewindow()
{
    scrollMore->setFrameShape(QFrame::NoFrame); //去除窗口边框
    vbClasslayoutMore->addWidget(showMore);

    pageMoreSpacer =new QSpacerItem(24,24,QSizePolicy::Minimum,QSizePolicy::Expanding);
    pageMoreWidget->setObjectName("pageMoreWidget");
    scrollMore->setWidget(pageMoreWidget);
    vbClasslayoutMore->addSpacerItem(pageMoreSpacer);
    vbClasslayoutMore->setMargin(0);
    pageMoreWidget->setLayout(vbClasslayoutMore);
    vbClasslayoutMore->addSpacing(24);
    //滚动条不可见，只能通过鼠标滑动
    scrollMore->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollMore->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    scrollMore->setWidgetResizable(true);
    scrollMore->setAlignment(Qt::AlignCenter);
}

void MorePage::sendDetailSig(int id)
{
    emit detailspageSig(id);
}

void MorePage::sendInstallPackage(QString name, int id)
{
    emit installpackage(name,id);
}

void MorePage::sendUpdatePackage(QString name, int id)
{
    emit updatePackage(name,id);
}
