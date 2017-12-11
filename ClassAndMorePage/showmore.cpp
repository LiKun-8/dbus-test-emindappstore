#include "showmore.h"
#include <QDebug>
#define MAXNUMBER 360
#define SHOWMOREROW 72
#define SHOWMORECOLUMN 5
#define SPACEWIDGET 5

ShowMore::ShowMore(QWidget *parent) : QWidget(parent)
{
    moreClassTop = new ClassTop();
    moreClassTop->setTopbtnHide();
    mainLayout = new QVBoxLayout();
    moreElement = new Element[MAXNUMBER];
    eleLayout = new QGridLayout();
    eleLayout->setSpacing(32);
    this->setMinimumSize(640,0);
    this->installEventFilter(this);
    elementNumber = 0;

    int maxEleNumber = 0;
    for(int i = 0;i<SHOWMOREROW;i++)
    {
        for(int j = 0;j<SHOWMORECOLUMN;j++)
        {
            eleLayout->addWidget(&moreElement[maxEleNumber],i,j,1,1,Qt::AlignLeft);
            moreElement[maxEleNumber].hide();
            connect(&moreElement[maxEleNumber],SIGNAL(installPackage(QString,int)),this,SLOT(sendInstallApp(QString,int)));
            connect(&moreElement[maxEleNumber],SIGNAL(updatePackage(QString,int)),this,SLOT(sendUpdateApp(QString,int)));
            connect(&moreElement[maxEleNumber],SIGNAL(detailspageSig(int)),this,SLOT(sendDetailSig(int)));
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
    mainLayout->setSpacing(32);
    mainLayout->setContentsMargins(16,24,8,0);
    mainLayout->addWidget(moreClassTop);
    mainLayout->addLayout(eleLayout);
    this->setLayout(mainLayout);

    spaceWidget = new QWidget*[SPACEWIDGET];
    for(int i =0 ;i<5;i++)
    {
        QWidget *p = new QWidget();
        spaceWidget[i] = p;
        spaceWidget[i]->setFixedSize(144,74);
    }
}

ShowMore::~ShowMore()
{
    delete[] moreElement;
}

//设置软件属性
void ShowMore::setElement(int category, const CLASSSTRUCTMAP &classStruct)
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
void ShowMore::setTopName(int category, const CATEGORYMAP &cateGoryMap)
{
    categoryFlag = category;
    if(cateGoryMap.isEmpty())
    {
        qDebug()<<"the cateMap is empty!"<<endl;
    }

    if(cateGoryMap.contains(category+1))
    {
        auto it = cateGoryMap.find(category+1);
        moreClassTop->setLabelData(it.value());
    }

}

void ShowMore::setElementNum(const ELEMENTNUMBERMAP &elementNum)
{
    auto it = elementNum.find(categoryFlag+1);
    elementNumber = it.value();
}

bool ShowMore::eventFilter(QObject *watched, QEvent *event)
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
                spaceWidget[i]->hide();
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
            spaceWidget[i]->show();
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
        return true;
    }
    return QWidget::eventFilter(watched,event);
}

void ShowMore::sendInstallApp(QString name, int id)
{
    emit installApp(name,id);
}

void ShowMore::sendUpdateApp(QString name, int id)
{
    emit updateApp(name,id);
}

void ShowMore::updatePackageStatus(QString name, bool bo,int flag)
{
    for(int i=0;i<elementNumber;i++)
    {
        if(name == moreElement[i].getBtnName())
        {
            moreElement[i].updateProStatus(bo,flag);
        }
    }
}


void ShowMore::sendDetailSig(int id)
{
    emit detailspageSig(id);
}

void ShowMore::updateMorePkgStatus(QString name)
{
    updatePackageStatus(name,true,DOWNLOADING);
}
