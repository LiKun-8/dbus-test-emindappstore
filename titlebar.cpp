#include "titlebar.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QFrame>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QTimer>

TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
    resize(960,80);
    setObjectName("titleBar");

    btnBack = new QPushButton(this);
    btnNext = new QPushButton(this);
    btnRefresh = new QPushButton(this);
    connect(btnRefresh,SIGNAL(pressed()),this,SLOT(changeStyle()));
    connect(btnRefresh,SIGNAL(released()),this,SLOT(changesStyle()));
    btnBack->setObjectName("backBtn");
    btnNext->setObjectName("nextBtn");
    btnRefresh->setObjectName("refreshBtn");
    btnBack->setFixedSize(16,16);
    btnNext->setFixedSize(16,16);
    btnRefresh->setFixedSize(16,16);
    btnBack->setDisabled(true);
    btnNext->setDisabled(true);

    btnHome = new QPushButton(this);
    btnHome->setText(tr("Home"));
    btnHome->setFlat(true);
    connect(btnHome,SIGNAL(clicked()),this,SLOT(changeBtnHome()));

    btnClass = new QPushButton(this);
    btnClass->setText(tr("Class"));
    btnClass->setFlat(true);
    connect(btnClass,SIGNAL(clicked()),this,SLOT(changeBtnClass()));

    btnUpdate = new QPushButton(this);
    btnUpdate->setText(tr("Update"));
    btnUpdate->setFlat(true);
    connect(btnUpdate,SIGNAL(clicked()),this,SLOT(changeBtnUpdate()));

    btnManager = new QPushButton(this);
    btnManager->setText(tr("Manager"));
    btnManager->setFlat(true);
    connect(btnManager,SIGNAL(clicked()),this,SLOT(changeBtnManager()));

    setBtnNormal();
    btnHome->setObjectName("btnHome");
    btnClass->setObjectName("btnClass");
    btnUpdate->setObjectName("btnUpdate");
    btnManager->setObjectName("btnManager");




    hly6 = new QHBoxLayout;
    hly1 = new QHBoxLayout;

    hly6->setContentsMargins(0,0,0,0);

    frmSearch = new QFrame(this);
    frmSearch->setObjectName("searchFrm");
    frmSearch->setFixedSize(QSize(160,24));

    leSearch = new CustomLineEdit(frmSearch);
    leSearch->setObjectName("searchBox");
    leSearch->setFixedSize(QSize(160,24));
    leSearch->setFrame(false);
    leSearch->setClearButtonEnabled(true);
    leSearch->setFocusPolicy(Qt::ClickFocus);

    lbSearch = new QLabel(frmSearch);
    lbSearch->setFixedSize(QSize(30,24));
    lbSearch->setText(tr("Search"));
    lbSearch->setObjectName("lbSearch");

    lbSearchImg = new QLabel(frmSearch);
    lbSearchImg->setPixmap(QPixmap(":/image/search.png"));
    lbSearchImg->setFixedSize(QSize(30,24));


    connect(leSearch,SIGNAL(hideSearchLabel()),lbSearch,SLOT(hide()));
    connect(leSearch,SIGNAL(hideSearchLabel()),lbSearchImg,SLOT(hide()));
    connect(leSearch,SIGNAL(showSearchLabel()),lbSearch,SLOT(show()));
    connect(leSearch,SIGNAL(showSearchLabel()),lbSearchImg,SLOT(show()));


    hly6->addWidget(lbSearchImg);
    hly6->addWidget(lbSearch);

    //    hly6->setAlignment(lbSearch,Qt::AlignCenter);
    //    hly6->setAlignment(lbSearchImg,Qt::AlignCenter);
    hly1->addLayout(hly6);
    hly1->setAlignment(hly6,Qt::AlignCenter);
    hly1->setContentsMargins(0,0,0,0);
    frmSearch->setLayout(hly1);

    leSearch->raise();


    hly2 = new QHBoxLayout;
    hly2->addSpacerItem(new QSpacerItem(10,1));
    hly2->addWidget(btnBack);
    hly2->addSpacerItem(new QSpacerItem(16,1));
    hly2->addWidget(btnNext);
    hly2->addSpacerItem(new QSpacerItem(16,1));
    hly2->addWidget(btnRefresh);

    hly3 = new QHBoxLayout;
    hly3->addWidget(btnHome);
    hly3->addWidget(btnClass);
    hly3->addWidget(btnUpdate);
    hly3->addWidget(btnManager);



    hly5 = new QHBoxLayout;
    hly5->addLayout(hly2);
    hly5->addLayout(hly3);
    //    hly5->addLayout(hly4);
    hly5->addWidget(frmSearch);
    hly5->setAlignment(hly2,Qt::AlignLeft);
    hly5->setAlignment(hly3,Qt::AlignCenter);
    hly5->setAlignment(frmSearch,Qt::AlignRight);

    line0 = new QFrame(this);
    line0->setFrameShape(QFrame::HLine);


    vly = new QVBoxLayout;
    //    vly->addLayout(hly1);
    vly->addLayout(hly5);
    vly->addWidget(line0);
    //    vly->setAlignment(hly1,Qt::AlignRight);
    vly->setContentsMargins(10,5,8,0);

    setLayout(vly);
}


void TitleBar::mousePressEvent(QMouseEvent *event)
{
    setFocus();
}

void TitleBar::changeStyle()
{
    btnRefresh->setStyleSheet("QPushButton {border-image:url(:/image/refresh_gray.png);}");
}

void TitleBar::changesStyle()
{
    btnRefresh->setStyleSheet("QPushButton {border-image:url(:/image/refresh_black.png);}");
}

void TitleBar::setBtnNormal()
{
    btnHome->setStyleSheet("QPushButton {color:#313131;}");
    btnClass->setStyleSheet("QPushButton {color:#313131;}");
    btnUpdate->setStyleSheet("QPushButton {color:#313131;}");
    btnManager->setStyleSheet("QPushButton {color:#313131;}");

    btnHome->setIcon(QPixmap(":/image/home_16_normal.png"));
    btnClass->setIcon(QPixmap(":/image/class_16_normal.png"));
    btnUpdate->setIcon(QPixmap(":/image/update_16_normal.png"));
    btnManager->setIcon(QPixmap(":/image/manager_16_normal.png"));
}


void TitleBar::changeBtnHome()
{
    btnHome->setStyleSheet("QPushButton {color:#027aff;}");
    btnClass->setStyleSheet("QPushButton {color:#313131;}");
    btnUpdate->setStyleSheet("QPushButton {color:#313131;}");
    btnManager->setStyleSheet("QPushButton {color:#313131;}");

    btnHome->setIcon(QPixmap(":/image/home_16_hover.png"));
    btnClass->setIcon(QPixmap(":/image/class_16_normal.png"));
    btnUpdate->setIcon(QPixmap(":/image/update_16_normal.png"));
    btnManager->setIcon(QPixmap(":/image/manager_16_normal.png"));
}
void TitleBar::changeBtnClass()
{
    btnHome->setStyleSheet("QPushButton {color:#313131;}");
    btnClass->setStyleSheet("QPushButton {color:#027aff;}");
    btnUpdate->setStyleSheet("QPushButton {color:#313131;}");
    btnManager->setStyleSheet("QPushButton {color:#313131;}");

    btnClass->setIcon(QPixmap(":/image/class_16_hover.png"));
    btnHome->setIcon(QPixmap(":/image/home_16_normal.png"));
    btnUpdate->setIcon(QPixmap(":/image/update_16_normal.png"));
    btnManager->setIcon(QPixmap(":/image/manager_16_normal.png"));
}
void TitleBar::changeBtnUpdate()
{
    btnHome->setStyleSheet("QPushButton {color:#313131;}");
    btnClass->setStyleSheet("QPushButton {color:#313131;}");
    btnUpdate->setStyleSheet("QPushButton {color:#027aff;}");
    btnManager->setStyleSheet("QPushButton {color:#313131;}");

    btnUpdate->setIcon(QPixmap(":/image/update_16_hover.png"));
    btnHome->setIcon(QPixmap(":/image/home_16_normal.png"));
    btnClass->setIcon(QPixmap(":/image/class_16_normal.png"));
    btnManager->setIcon(QPixmap(":/image/manager_16_normal.png"));
}
void TitleBar::changeBtnManager()
{
    btnHome->setStyleSheet("QPushButton {color:#313131;}");
    btnClass->setStyleSheet("QPushButton {color:#313131;}");
    btnUpdate->setStyleSheet("QPushButton {color:#313131;}");
    btnManager->setStyleSheet("QPushButton {color:#027aff;}");

    btnManager->setIcon(QPixmap(":/image/manager_16_hover.png"));
    btnHome->setIcon(QPixmap(":/image/home_16_normal.png"));
    btnClass->setIcon(QPixmap(":/image/class_16_normal.png"));
    btnUpdate->setIcon(QPixmap(":/image/update_16_normal.png"));
}
