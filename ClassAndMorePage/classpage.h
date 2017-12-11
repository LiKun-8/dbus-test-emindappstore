#ifndef CLASSPAGE_H
#define CLASSPAGE_H

#include <QWidget>
#include "classwidget.h"
#include "morepage.h"
#include "showmore.h"

class ClassPage : public QWidget
{
    Q_OBJECT

public:
    ClassPage(QWidget *parent,JSONFUNC *json,ShareData *data);
    ~ClassPage();
    MorePage *moreClassWidget;

private:
    ClassWidget *classWidget;//分类对象
    ShareData *shareData;
    JSONFUNC *jsonFunc;//数据读取
    QSpacerItem *pageClassSpacer;
    QVBoxLayout *vbClasslayout;
    QWidget *pageClassWidget;
    int cateNum;
    QScrollArea  *scrollClass;
    QVBoxLayout *mainLayout;

signals:
    void setMore();
    void installpackage(QString,int);
    void updatePackage(QString,int);
    void packageStatus(QString,bool);
    void detailspageSig(int);

public slots:
    void setClassElement();
    void createClassWindow(int catenum);
    void setMoreShow(int i);
    void testUpdateMap();
    void sendInstallPackage(QString name ,int id);
    void sendUpdatePackage(QString name ,int id);
    void updatePackageStatus(QString name, bool bo, int flag);
    void resetStatus();
    void detailspage(int id);
    void updatePkgStatus(QString name);
//    void updateMorePkgStatus(QString name, int id);
};

#endif // CLASSPAGE_H
