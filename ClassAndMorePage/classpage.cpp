#include "classpage.h"


ClassPage::ClassPage(QWidget *parent, JSONFUNC *json, ShareData *data) : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);
    scrollClass = new QScrollArea();
    shareData = data;
    jsonFunc = json;
    jsonFunc->getCategoryNum();
    moreClassWidget = new MorePage();
    scrollClass->setMinimumSize(980,480);
    pageClassWidget = new QWidget();
    pageClassWidget->setMaximumWidth(1200);

    connect(jsonFunc,SIGNAL(productIsOk()),moreClassWidget,SLOT(createMorewindow()),Qt::QueuedConnection);
    connect(jsonFunc,SIGNAL(categoryIsOk(int)),this,SLOT(createClassWindow(int)),Qt::QueuedConnection);
    connect(jsonFunc,SIGNAL(productIsOk()),this,SLOT(setClassElement()),Qt::QueuedConnection);
    //    connect(jsonFunc,SIGNAL(updateIsOk()),this,SLOT(testUpdateMap()));
    mainLayout->addWidget(scrollClass);
    mainLayout->setContentsMargins(0,0,5,0);
    this->setLayout(mainLayout);
    this->classWidget = NULL;
    update();
}

ClassPage::~ClassPage()
{
    delete[] classWidget;
    delete pageClassWidget;
}

void ClassPage::resetStatus()
{
    for(int i = 0;i<cateNum;i++)
    {
        classWidget[i].resetStatus(shareData->classStrMap);
    }
}

void ClassPage::detailspage(int id)
{
    emit detailspageSig(id);
}

void ClassPage::updatePkgStatus(QString name)
{
    updatePackageStatus(name,true,DOWNLOADING);
}

//void ClassPage::updateMorePkgStatus(QString name, int id)
//{
//    for(auto item = shareData->classStrMap.begin();item != shareData->classStrMap.end();++item)
//    {
//        if(item.value().releaseId == id)
//            item.value().proStatus = DOWNLOADING;
//        moreClassWidget->showMore->updatePkgStatus(name);
//    }
//}


//创建分类页
void ClassPage::createClassWindow(int catenum)
{
    cateNum = catenum;
    classWidget = new ClassWidget[catenum];
    vbClasslayout = new QVBoxLayout();
    scrollClass->setFrameShape(QFrame::NoFrame); //去除窗口边框

    for(int i=0;i<catenum;i++)
    {
        connect(&classWidget[i],SIGNAL(moreShow(int)),this,SLOT(setMoreShow(int)));
        connect(&classWidget[i],SIGNAL(installApp(QString,int)),this,SLOT(sendInstallPackage(QString,int)));
        connect(&classWidget[i],SIGNAL(updateApp(QString,int)),this,SLOT(sendUpdatePackage(QString,int)));
        connect(&classWidget[i],SIGNAL(detailspage(int)),this,SLOT(detailspage(int)));

        classWidget[i].setCategory(i);
        classWidget[i].setTopName(shareData->categoryMap);
        vbClasslayout->addWidget(&classWidget[i]);
    }

    pageClassSpacer =new QSpacerItem(24,24,QSizePolicy::Minimum,QSizePolicy::Expanding);
    pageClassWidget->setObjectName("pageClassWidget");
    scrollClass->setWidget(pageClassWidget);
    vbClasslayout->addSpacerItem(pageClassSpacer);
    vbClasslayout->setMargin(0);
    pageClassWidget->setLayout(vbClasslayout);
    vbClasslayout->addSpacing(24);

    //滚动条不可见，只能通过鼠标滑动
    scrollClass->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollClass->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollClass->setWidgetResizable(true);
    scrollClass->setAlignment(Qt::AlignCenter);
    jsonFunc->setAppname();
}


//设置分类软件的属性
void ClassPage::setClassElement()
{
//    qDebug()<<__FUNCTION__<<endl;
    if(shareData->classStrMap.isEmpty())
    {
        qDebug()<<"classStrMap is Empty!"<<endl;
    }

    for(int i = 0;i<cateNum;i++)
    {
        classWidget[i].initElement(shareData->classElementNumMap);
        classWidget[i].setElement(shareData->classStrMap);
    }
    jsonFunc->getRecommend();
}

//测试更多页面跳转
void ClassPage::setMoreShow(int i)
{
    moreClassWidget->showMore->setTopName(i,shareData->categoryMap);
    moreClassWidget->showMore->setElementNum(shareData->classElementNumMap);
    moreClassWidget->showMore->setElement(i,shareData->classStrMap);
    emit setMore();
}

void ClassPage::testUpdateMap()
{
    if(!shareData->updateStrMap.isEmpty())
    {
        for(auto it = shareData->updateStrMap.begin();it!=shareData->updateStrMap.end();++it)
        {
            qDebug()<<"release id = "<<it.key()<<"  version = "<<it.value().version<<"  packsize = "<<it.value().packageSize;
        }
    }
}

void ClassPage::sendInstallPackage(QString name, int id)
{
    emit installpackage(name,id);
    for(auto item = shareData->classStrMap.begin();item != shareData->classStrMap.end();++item)
    {
        if(item.value().releaseId == id)
            item.value().proStatus = DOWNLOADING;
    }
}

void ClassPage::sendUpdatePackage(QString name, int id)
{
    emit updatePackage(name,id);
    for(auto item = shareData->classStrMap.begin();item != shareData->classStrMap.end();++item)
    {
        if(item.value().releaseId == id)
            item.value().proStatus = UPDATING;
    }
}

//1:下载 2:更新 9:卸载 4:重新下载 5:重新更新 6:正在卸载
void ClassPage::updatePackageStatus(QString name, bool bo,int flag)
{
//    qDebug()<<__FUNCTION__<<endl;
    for(auto item = shareData->classStrMap.begin();item != shareData->classStrMap.end();++item)
    {
        if(name == item.value().proName)
        {
            if(flag == DOWNLOAD || flag == UPDATE)
            {
                if(bo)
                {
                    item.value().proStatus = OPEN;
                }
                else
                {
                    if(flag == DOWNLOAD)
                        item.value().proStatus = REDOWNLOAD;
                    else if(flag == UPDATE)
                        item.value().proStatus = REUPDATE;
                }
            }
            else if(flag == UNINSTALL)
            {
                if(bo)
                {
                    item.value().proStatus = DOWNLOAD;
                }
                else
                {
                    item.value().proStatus = OPEN;
                }
            }
            else if(flag == UNINSTALLING)
            {
                item.value().proStatus = UNINSTALLING;
            }
            else if(flag == UPDATING)
            {
                item.value().proStatus = UPDATING;
            }
            else if(flag == REDOWNLOAD)
            {
                item.value().proStatus = DOWNLOADING;
            }
            else if(flag == REUPDATE)
            {
                item.value().proStatus = UPDATING;
            }
            else if(flag == DOWNLOADING)
            {
                item.value().proStatus = DOWNLOADING;
            }
        }

        for(int i = 0;i<cateNum;i++)
        {
            if(classWidget[i].getCategory() == item.value().category)
            {
                for(int j = 0; j < classWidget[i].getElementNum();j++)
                {
                    if(classWidget[i].getTt(j)->getBtnName()== name )
                    {
                        classWidget[i].getTt(j)->updateProStatus(bo,flag);
                    }
                }
            }
        }

    }
}


