#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <qglobal.h>
#include <QVBoxLayout>
#include <QSpacerItem>
#include "customlineedit.h"

class QPushButton;
class QHBoxLayout;
class QLineEdit;
class QLabel;
class QFrame;
class QFocusEvent;

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = 0);

signals:

public slots:
    void changeStyle();
    void changesStyle();
    void changeBtnHome();
    void changeBtnClass();
    void changeBtnUpdate();
    void changeBtnManager();
    void setBtnNormal();




protected:
    void mousePressEvent(QMouseEvent *event);

public:
    QPushButton *btnBack;
    QPushButton *btnNext;
    QPushButton *btnRefresh;

    QPushButton *btnHome;
    QPushButton *btnClass;
    QPushButton *btnUpdate;
    QPushButton *btnManager;

    QPushButton *btnMinimum;
    QPushButton *btnMaximum;
    QPushButton *btnClose;

    QHBoxLayout *hly1;
    QHBoxLayout *hly2;
    QHBoxLayout *hly3;
    QHBoxLayout *hly4;
    QHBoxLayout *hly5;
    QHBoxLayout *hly6;
    QVBoxLayout *vly;

    CustomLineEdit *leSearch;
    QLabel *lbSearch;
    QLabel *lbSearchImg;
    QFrame *frmSearch;
    QPixmap mImgSearch;

    QFrame *line0;



private:
    QPoint mPressPoint;

};

#endif // TITLEBAR_H
