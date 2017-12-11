#ifndef RECOMMENDWIDGET_H
#define RECOMMENDWIDGET_H

#include <QWidget>
#include "classwidget.h"
#include "classtop.h"
#include "sharedata.h"
class RecommendWidget : public QWidget
{
    Q_OBJECT
public:
    RecommendWidget(QWidget *parent,ShareData *data);
    ~RecommendWidget();
    void setElement(const CLASSSTRUCTMAP &classStruct,const RECOMMENDMAP &recommend);
    void setTopName();

private:
    ClassTop *classTop;
    QVBoxLayout *mainLayout;
    Element *element;
    QGridLayout *eleLayout;
    QWidget  *spaceWidget;
    JSONFUNC *jsonFunc;
    QSpacerItem *spacer;

    bool eventFilter(QObject *watched, QEvent *event);
    int categoryFlag;
    ShareData *shareData;

    bool showFlag;

signals:
    void installApp(QString,int);
    void updateApp(QString,int);
    void toDetailSig(int);

protected slots:
    void sendInstallApp(QString name,int id);
    void sendUpdateApp(QString name,int id);
    void updatePackageStatus(QString name, bool bo, int flag);
    void toDetailPage(int id);
    void resetStatus();
    void updatePkgStatus(QString name,int id);
};

#endif // RECOMMENDWIDGET_H
