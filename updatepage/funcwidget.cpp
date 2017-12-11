#include "funcwidget.h"
#include <QDebug>

FuncWidget::FuncWidget(QWidget *parent, QString funcStr) : QWidget(parent),funcText(funcStr)
{
    funcLayout = new QHBoxLayout;

    funcLayout->setMargin(0);
    funcLayout->setSpacing(0);
    leftLayout = new QVBoxLayout;
    leftLayout->setMargin(0);
    rightLayout = new QVBoxLayout;
    funcLabel = new QLabel();
    funcLabel->setObjectName("funcLabel");
    nfuncEdit = new QTextEdit();
    nfuncEdit->setObjectName("nfuncEdit");
    hideButton = new QPushButton();
    hideButton->setObjectName("hideButton");

    funcLabel->setText(tr(" New Features"));

    nfuncEdit->setText(funcStr);
    nfuncEdit->setReadOnly(true);
    nfuncEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QPixmap pixmap(":/image/hide.png");
    hideButton->setFlat(true);
    hideButton->setIcon(pixmap);
    hideButton->setFixedSize(24,24);

    funcLayout->addSpacing(80);
    funcLayout->addLayout(leftLayout);
    leftLayout->addSpacing(15);
    leftLayout->addWidget(funcLabel);
    leftLayout->addSpacing(8);
    leftLayout->addWidget(nfuncEdit);
    funcLayout->addSpacing(100);
    funcLayout->addLayout(rightLayout);
    rightLayout->addStretch();
    rightLayout->addWidget(hideButton);
    rightLayout->addSpacing(10);
    funcLayout->addSpacing(10);
    setLayout(funcLayout);

}

QTextEdit *FuncWidget::getNfuncEdit()
{
    return nfuncEdit;
}

QPushButton *FuncWidget::getHideButton()
{
    return hideButton;
}

bool FuncWidget::event(QEvent *event)
{
    if(event->type() == QEvent::Paint)
    {
        int docheight = nfuncEdit->document()->size().height()-30;
        nfuncEdit->size().setHeight(docheight);
        emit sigTextHeight(docheight);
        return true;
    }
    return QWidget::event(event);
}
