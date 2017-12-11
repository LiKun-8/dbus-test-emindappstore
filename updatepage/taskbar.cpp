#include "taskbar.h"

TaskBar::TaskBar(QWidget *parent,QString taskstr,QString onekeystr):QWidget(parent),taskStr(taskstr),onekeyStr(onekeystr)
{
    this->size().setHeight(64);
    size().setHeight(64);
    hbLayout = new QHBoxLayout();
    hbLayout->setMargin(0);

    vblueLabel = new QLabel();
    vblueLabel->setObjectName("vblueLabel");
    vblueLabel->setFixedSize(4,16);

    taskLabel = new QLabel();
    taskLabel->setObjectName("taskLabel");
    taskLabel->setText(taskStr);

    onekeyButton = new QPushButton();
    onekeyButton->setObjectName("onekeyButton");
    onekeyButton->setFixedSize(80,32);
    onekeyButton->setText(onekeyStr);

    hbLayout->addWidget(vblueLabel);
    hbLayout->addSpacing(2);
    hbLayout->addWidget(taskLabel);
    hbLayout->addStretch();
    hbLayout->addWidget(onekeyButton);

    setLayout(hbLayout);
}

void TaskBar::setTaskLabel(int count)
{
    QString countStr = QString::number(count, 10);
    QString taskStr = countStr + tr(" Applications Can Be Updated");
    taskLabel->setText(taskStr);
}

void TaskBar::hideOnekeyButton()
{
    onekeyButton->hide();
}

QPushButton *TaskBar::getOnekeyBtn()
{
    return onekeyButton;
}
