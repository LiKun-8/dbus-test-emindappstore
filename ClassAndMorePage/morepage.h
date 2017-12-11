#ifndef MOREPAGE_H
#define MOREPAGE_H

#include <QWidget>
#include "showmore.h"

class MorePage: public QWidget
{
    Q_OBJECT

public:
    MorePage(QWidget *parent = 0);
    ~MorePage();
    QScrollArea  *scrollMore;
    ShowMore *showMore;

private:
    QSpacerItem *pageMoreSpacer;
    QVBoxLayout *vbClasslayoutMore;
    QWidget *pageMoreWidget;
    QVBoxLayout *mainLayout;

signals:
    void installpackage(QString,int);
    void updatePackage(QString,int);
    void detailspageSig(int);

protected slots:
    void sendInstallPackage(QString name ,int id);
    void sendUpdatePackage(QString name ,int id);
    void createMorewindow();
    void sendDetailSig(int id);
};

#endif // MOREPAGE_H
