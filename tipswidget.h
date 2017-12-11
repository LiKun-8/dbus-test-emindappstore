#ifndef TIPSWIDGET_H
#define TIPSWIDGET_H

#include <QObject>
#include <QWidget>
#include <qglobal.h>
class QLabel;
class QHBoxLayout;
class QVBoxLayout;
class QSpacerItem;

class TipsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TipsWidget(QWidget *parent = 0);
    void setTips(QString& keyText);

signals:

public slots:


private:
    QLabel *lbTips;
    QLabel *lbTips1;
    QLabel *lbWarnImg;
    QHBoxLayout *hly1;
    QHBoxLayout *hly2;
    QVBoxLayout *vly2;
    QSpacerItem *spItem1;

};

#endif // TIPSWIDGET_H
