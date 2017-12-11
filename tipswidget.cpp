#include "tipswidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

TipsWidget::TipsWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumWidth(380);
    hly1 = new QHBoxLayout;
    hly2 = new QHBoxLayout;

    vly2 = new QVBoxLayout;
    lbTips = new QLabel(this);
    lbTips1 = new QLabel(this);
    lbTips->setObjectName("lbTips");
    lbTips1->setObjectName("lbTips1");
    lbWarnImg = new QLabel(this);
    spItem1 = new QSpacerItem(1,200);
    hly1->addWidget(lbWarnImg);
    hly1->addWidget(lbTips);
    hly2->addSpacerItem(new QSpacerItem(40,1));
    hly2->addWidget(lbTips1);
    vly2->addSpacerItem(spItem1);
    vly2->addLayout(hly1);
    vly2->addLayout(hly2);

    lbTips1->setText(tr("Please change another  keyword and search again."));
    lbWarnImg->setPixmap(QPixmap(":/image/warn.png"));
    setLayout(vly2);
}

void TipsWidget::setTips(QString &keyText)
{
    lbTips->setText(tr("Sorry!We Cannot find applications about %1.").arg(keyText));

}
