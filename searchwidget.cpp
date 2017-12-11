#include "searchwidget.h"
#include <QDebug>
#include <QLayout>
#include <QSpacerItem>
#include <QScrollArea>
#include <QScrollBar>
#include "tipswidget.h"

#define MAXNUMBER 360
#define SearchWidgetROW 72
#define SearchWidgetCOLUMN 5
#define SPACEWIDGET 5
static const int textMax = 16;

SearchWidget::SearchWidget(QWidget *parent,ShareData* shareData,JSONFUNC *jsonFunc) : QWidget(parent)
{

    sdata = shareData;
    jdata = jsonFunc;

    searchArea = new QWidget(this);
    searchArea->setMaximumWidth(1200);
    searchArea->setObjectName("searchArea");
    scrollArea  = new QScrollArea(this);
//
    tipsWidget = new TipsWidget(this);
//

    moreClassTop = new ClassTop();
    moreClassTop->setTopbtnHide();
    mainLayout = new QVBoxLayout();
    moreElement = new Element[MAXNUMBER];
    eleLayout = new QGridLayout();
    eleLayout->setSpacing(24);
    this->setMinimumSize(500,0);
    this->installEventFilter(this);
    eleLayout->setContentsMargins(16,0,16,0);
    elementNumber = 0;

    int maxEleNumber = 0;
    for(int i = 0;i<SearchWidgetROW;i++)
    {
        for(int j = 0;j<SearchWidgetCOLUMN;j++)
        {
            eleLayout->addWidget(&moreElement[maxEleNumber],i,j,1,1,Qt::AlignLeft);
            moreElement[maxEleNumber].hide();
            connect(&moreElement[maxEleNumber],SIGNAL(installPackage(QString,int)),this,SLOT(sendInstallApp(QString,int)));
            connect(&moreElement[maxEleNumber],SIGNAL(updatePackage(QString,int)),this,SLOT(sendUpdateApp(QString,int)));
            connect(&moreElement[maxEleNumber],SIGNAL(detailspageSig(int)),this,SLOT(toDetailPage(int)));
            if(maxEleNumber < (MAXNUMBER-1))
            {
                maxEleNumber++;
            }
            else
            {
                break;
            }
        }
    }

    categoryFlag = -1;
    mainLayout->addWidget(moreClassTop);
//
    mainLayout->addWidget(tipsWidget);
    mainLayout->setAlignment(tipsWidget,Qt::AlignCenter);
//
    mainLayout->addLayout(eleLayout);

    spItem = new QSpacerItem(100,100,QSizePolicy::Expanding,QSizePolicy::Expanding);
    mainLayout->insertSpacerItem(-1,spItem);
    searchArea->setLayout(mainLayout);

    spaceWidget = new QWidget*[SPACEWIDGET];
    for(int i =0 ;i<5;i++)
    {
        QWidget *p = new QWidget();
        spaceWidget[i] = p;
        spaceWidget[i]->setFixedSize(144,74);
    }

    vly1 = new QVBoxLayout;
    vly1->addWidget(scrollArea);
    setLayout(vly1);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setWidget(searchArea);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->verticalScrollBar()->setValue(0);

}

SearchWidget::~SearchWidget()
{
    delete[] moreElement;
}


void SearchWidget::setElement(QString keyText1, const CLASSSTRUCTMAP &classStruct)
{
    int showNum = 0;
    for(auto item = classStruct.begin();item != classStruct.end();++item)
    {
        if(item.value().proName.contains(keyText1))
        {
            moreElement[showNum].setBtnName(item.value().proName);
            moreElement[showNum].show();
            moreElement[showNum].setProductId(item.key());
            moreElement[showNum].setBtnImage(item.value().proImage);
            moreElement[showNum].setProStatus(item.value().proStatus);
            moreElement[showNum].initStar(item.value().proStar);
            moreElement[showNum].setReleaseId(item.value().releaseId);
            moreElement[showNum].setExeFile(item.value().exeFile);
            showNum++;
        }
    }

    for(int hideNum = showNum;hideNum<MAXNUMBER;hideNum++)
    {
        moreElement[hideNum].hide();
    }

    if(showNum == 0){
        tipsWidget->setTips(mKeyText);
        tipsWidget->show();

    }else{
        tipsWidget->hide();
    }
}


//设置软件属性
void SearchWidget::setElement(int category, const CLASSSTRUCTMAP &classStruct)
{
    if(classStruct.isEmpty())
    {
        qDebug()<<"the sortstr is empty!"<<endl;
    }

    int showNum = 0;
    for(auto item = classStruct.begin();item != classStruct.end();++item)
    {
        if(item.value().category == (category+1))
        {
            moreElement[showNum].setBtnName(item.value().proName);
            moreElement[showNum].show();
            moreElement[showNum].setProductId(item.key());
            moreElement[showNum].setBtnImage(item.value().proImage);
            moreElement[showNum].setProStatus(item.value().proStatus);
            moreElement[showNum].initStar(item.value().proStar);
            moreElement[showNum].setReleaseId(item.value().releaseId);
            moreElement[showNum].setExeFile(item.value().exeFile);
            showNum++;
        }
    }

    for(int hideNum = showNum;hideNum<MAXNUMBER;hideNum++)
    {
        moreElement[hideNum].hide();
    }
}

//设置软件类名字
void SearchWidget::setTopName(QString keyText)
{
    moreClassTop->setLabelData(tr("Application About %1").arg(keyText));
}

void SearchWidget::setElementNum(const ELEMENTNUMBERMAP &elementNum)
{
    auto it = elementNum.find(categoryFlag+1);
    elementNumber = it.value();
}

bool SearchWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this)
    {
        if(event->type() == QEvent::Resize)
        {
            if(categoryFlag == -1)
            {
                return true;
            }

            int column = (this->size().width()+48)/192;
            int row;

            if(column > 6)
            {
                column = 6;
            }

            if(elementNumber%column == 0)
            {
                row  = (elementNumber/column);
            }
            else
            {
                row  = (elementNumber/column)+1;
            }


            if(column < 0 || row < 0)
            {
                qDebug()<<"column or row is error!"<<endl;
            }

            if(!eleLayout->isEmpty())
            {
                //现有的控件不必清空,只是对现有的控件进行排序,空Widget每次都要清空
                for(int i = 0;i < 5;i++)
                {
                    eleLayout->removeWidget(spaceWidget[i]);
                }

            }

            //对软件控件进行重新排序
            int num=0;
            for(int i=0;i<row;i++)
            {
                for(int j=0;j<column;j++)
                {
                    eleLayout->addWidget(&moreElement[num],i,j,1,1,Qt::AlignLeft);

                    if(num<(elementNumber-1))
                    {
                        num++;
                    }
                    else
                    {
                        break;
                    }
                }
            }

            //为不够一行的软件类添加空控件，使布局好看
            for(int i = 0;i<(column - elementNumber);++i)
            {
                eleLayout->addWidget(spaceWidget[i],0,elementNumber+i,1,1,Qt::AlignLeft);
            }

            //隐藏多余的控件
            if(elementNumber>column)
            {
                for(int i = elementNumber;i<MAXNUMBER;i++)
                {
                    moreElement[i].hide();
                }
                for(int i = 0;i<elementNumber;i++)
                {
                    moreElement[i].show();
                }
            }
        }
    }
    return QWidget::eventFilter(watched,event);
}

void SearchWidget::sendInstallApp(QString name, int id)
{
    emit installApp(name,id);
}

void SearchWidget::sendUpdateApp(QString name, int id)
{
    emit updateApp(name,id);
}

void SearchWidget::updatePackageStatus(QString name, bool bo,int flag)
{
    for(int i=0;i<elementNumber;i++)
    {

        if(name == moreElement[i].getBtnName())
        {
            if(flag == DOWNLOAD || flag == UPDATE)
            {
                moreElement[i].updateProStatus(bo,flag);
            }
            else if(flag == UNINSTALL)
            {
                moreElement[i].updateProStatus(bo,UNINSTALL);
            }
            else if(flag == UNINSTALLING || flag == UPDATING)
            {
                moreElement[i].updateProStatus(bo,flag);
            }
            else if(flag == REDOWNLOAD || flag == REUPDATE)
            {
                moreElement[i].updateProStatus(bo,flag);
            }
        }

    }
}

void SearchWidget::doSearch(QString keyText)
{
    if(!keyText.isEmpty()){
        mKeyText = keyText;
        if(mKeyText.length()>textMax){
            mKeyText.truncate(textMax);
            mKeyText.append("...");
        }
        mKeyText = "\"" + mKeyText + "\"";
        setTopName(mKeyText);
        setElement(keyText,sdata->classStrMap);
    }
}

void SearchWidget::refreshPage(int page)
{
    if(page == 4)
        doSearch(mKeyText);
}

void SearchWidget::toDetailPage(int id)
{
    emit toDetailSig(id);
}
