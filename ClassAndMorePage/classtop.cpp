#include "classtop.h"
#include <QDebug>

ClassTop::ClassTop()
{
    category = 0;
    btnTop = new QPushButton();
    btnTop->setFlat(true);
    labelTop = new QLabel();
    spacerTop = new QSpacerItem(0,48,QSizePolicy::Expanding,QSizePolicy::Minimum);
    labelTop->setObjectName("labelTop");
    labelTop->setContentsMargins(8,0,0,0);
    labelTop->setMaximumHeight(21);

    setBtnData();
    lineTop = new QLabel();
    lineTop->setObjectName("lineTop");
    lineTop->setStyleSheet("border-bottom: 1px;border-style: solid;border-bottom-color: rgb(220, 220, 220);");
    lineTop->setMaximumHeight(16);

    //美化布局
    hbTopLayout = new QHBoxLayout();
    hbTopLayout->addWidget(labelTop);
    hbTopLayout->addSpacerItem(spacerTop);
    hbTopLayout->addWidget(btnTop);

    vbTopLayout = new QVBoxLayout();
    vbTopLayout->setSpacing(0);
    vbTopLayout->setMargin(0);
    vbTopLayout->addLayout(hbTopLayout);
    vbTopLayout->addWidget(lineTop);
    vbTopLayout->setContentsMargins(0,0,0,0);

    this->setLayout(vbTopLayout);
    this->setMaximumHeight(48);
    btnTop->setFocusPolicy(Qt::NoFocus);
    btnTop->setMinimumWidth(48);
    btnTop->setCursor(Qt::PointingHandCursor);
    connect(btnTop,SIGNAL(clicked(bool)),this,SLOT(sendSlotBtn()));
}

ClassTop::~ClassTop()
{

}

void ClassTop::setBtnData()
{
    QHBoxLayout *layout = new QHBoxLayout();
    QLabel *labelTxt = new QLabel();
    labelTxt->setObjectName("labelTxt");

    labelTxt->setText(tr("more"));
    QLabel *labelImage = new QLabel();
    QPixmap image;
    image.load(":/image/more.png");
    labelImage->setPixmap(image);
    layout->addWidget(labelTxt);
    layout->addWidget(labelImage);
    layout->setMargin(0);
    layout->setSpacing(0);
    btnTop->setLayout(layout);
}

void ClassTop::setLabelData(QString data)
{
    labelTop->setText(data);
}

void ClassTop::setcategory(int cate)
{
    category = cate;
}

void ClassTop::setTopbtnHide()
{
    btnTop->hide();
}

void ClassTop::sendSlotBtn()
{
    emit showAll(category);
//    qDebug()<<"top sort  show all!"<<endl;
}
