#include "classwidget.h"

#define SPACEWIDGET 5

ClassWidget::ClassWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumSize(640,0);
    classtop = new ClassTop();

    gridLayout = new QGridLayout();
    //    gridLayout->setContentsMargins(16,0,16,0);

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(classtop);
    mainLayout->addLayout(gridLayout);
    mainLayout->setSpacing(32);
    mainLayout->setContentsMargins(16,24,8,0);

    this->setLayout(mainLayout);
    this->installEventFilter(this);

    connect(classtop,SIGNAL(showAll(int)),this,SLOT(sendMoreShow(int)));

    spaceWidget = new QWidget*[SPACEWIDGET];
    for(int i =0 ;i<SPACEWIDGET;i++)
    {
        QWidget *p = new QWidget();
        spaceWidget[i] = p;
        spaceWidget[i]->setFixedSize(144,74);
    }
}

ClassWidget::~ClassWidget()
{
    delete[] tt;
    tt = NULL;
    delete gridLayout;
    gridLayout = NULL;
    delete[] spaceWidget;
    spaceWidget = NULL;
}
//设置分类标志
void ClassWidget::setCategory(int cate)
{
    //    tt->setcategory(cate);
    classtop->setcategory(cate);
    category = cate;
}

//事件过滤器
bool ClassWidget::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::Resize)
    {
        int numElement =0;
        int column = (this->size().width()+48)/192;
        int row;
        //            qDebug()<<"column 1  ==  "<<column<<endl;

        if(demoElement.isEmpty())
        {
            return true;
        }

        if(demoElement.size()%column == 0)
        {
            row  = (demoElement.size()/column);
        }
        else
        {
            row  = (demoElement.size()/column)+1;
        }

        if(row > 3)
        {
            row = 3;
        }

        if(column > 6)
        {
            column = 6;
        }

        if(column < 0 || row < 0)
        {
            qDebug()<<"column or row is error!"<<endl;
        }

        if(!gridLayout->isEmpty())
        {
            //现有的控件不必清空,只是对现有的控件进行排序
            //                for(int i=0;i<demoElement.size();i++)
            //                {
            //                    while((child = gridLayout->takeAt(0))!=0)
            //                        delete child;
            //                    gridLayout->removeWidget(demoElement.at(i));

            //                }

            //空Widget每次都要清空
            for(int i = 0;i < SPACEWIDGET;i++)
            {
                gridLayout->removeWidget(spaceWidget[i]);
            }

        }

        //对软件控件进行重新排序
        for(int i=0;i<row;i++)
        {
            for(int j=0;j<column;j++)
            {

                gridLayout->addWidget(demoElement.at(numElement),i,j,1,1,Qt::AlignLeft);

                if(numElement<demoElement.size()-1)
                {
                    numElement++;
                }
                else
                {
                    break;
                }
            }
        }

        //为不够一行的软件类添加空控件，使布局好看
        for(int i = 0;i<(column - demoElement.size());i++)
        {
            //                qDebug()<<(column - demoElement.size())<<endl;
            gridLayout->addWidget(spaceWidget[i],0,demoElement.size()+i,1,1,Qt::AlignLeft);
        }

        //隐藏多余的控件
        if(demoElement.size()>=(row*column) && column > 3)
        {

            for(int i = (row*column);i<demoElement.size();i++)
            {
                demoElement.at(i)->hide();
            }
            for(int i = 0;i<(row*column);i++)
            {
                demoElement.at(i)->show();
            }
        }

        if(demoElement.size() > 15 && demoElement.size() <= 18)
        {
            if(column == 5)
            {
                for(int i = 15;i<demoElement.size();i++)
                    demoElement.at(i)->hide();
            }
            else if(column == 6)
            {
                for(int i = 15;i<demoElement.size();i++)
                    demoElement.at(i)->show();
            }
        }
        return true;
    }
    return QWidget::eventFilter(target,event);
}

void ClassWidget::sendMoreShow(int i)
{
    emit moreShow(i);
    //    qDebug()<<" More  Show  !!!!!!!"<<endl;
}

void ClassWidget::sendInstallApp(QString name, int id)
{
    emit installApp(name,id);
}

void ClassWidget::sendUpdateApp(QString name, int id)
{
    emit updateApp(name,id);
}

void ClassWidget::detailspageSig(int id)
{
    emit  detailspage(id);
}

//设置分类项名字
void ClassWidget::setTopName(const CATEGORYMAP &cateMap)
{
    if(cateMap.isEmpty())
    {
        qDebug()<<"the cateMap is empty!"<<endl;
    }

    if(cateMap.contains(category+1))
    {
        const auto it = cateMap.find(category+1);
        classtop->setLabelData(it.value());
        //        qDebug()<<"the it.value is : "<<it.value()<<endl;
    }
}

//设置软件项属性
void ClassWidget::setElement(const CLASSSTRUCTMAP &classStructMap)
{
    if(classStructMap.isEmpty())
    {
        qDebug()<<"the sortstr is empty!"<<endl;
    }

    auto item = classStructMap.begin();
    for(int i = 0;item != classStructMap.end() && i<18 ; ++item)
    {
        if(item.value().category == (category+1))
        {
            tt[i].setBtnName(item.value().proName);
            tt[i].setProductId(item.key());
            tt[i].initStar(item.value().proStar);
            tt[i].setBtnImage(item.value().proImage);
            tt[i].setProStatus(item.value().proStatus);
            tt[i].setReleaseId(item.value().releaseId);
            tt[i].setExeFile(item.value().exeFile);
            i++;
        }
    }
}

//初始化软件项
void ClassWidget::initElement(const ELEMENTNUMBERMAP &classElementNumMap)
{
    //    qDebug()<<__FUNCTION__<<endl;
    auto it = classElementNumMap.find(category+1);
    tt = new Element[it.value()];
    elementNum = it.value();
    for(int i=0 ; i<it.value() && i<18 ; i++)
    {
        demoElement.append(&tt[i]);
        connect(&tt[i],SIGNAL(installPackage(QString,int)),this,SLOT(sendInstallApp(QString,int)));
        connect(&tt[i],SIGNAL(updatePackage(QString,int)),this,SLOT(sendUpdateApp(QString,int)));
        connect(&tt[i],SIGNAL(detailspageSig(int)),this,SLOT(detailspageSig(int)));
    }
    this->resize(QSize(this->width()+1,this->height()+1));
}

int ClassWidget::getCategory()
{
    return category+1;
}

int ClassWidget::getElementNum()
{
    return elementNum;
}

Element *ClassWidget::getTt(int num)
{
    return &tt[num];
}

void ClassWidget::resetStatus(const CLASSSTRUCTMAP &classStructMap)
{
    auto item = classStructMap.begin();
    for(int i = 0;item != classStructMap.end() && i<18 ; ++item)
    {
        if(item.value().category == (category+1))
        {
            tt[i].setProStatus(item.value().proStatus);
            i++;
        }
    }
}



