#ifndef SHOWMOREH
#define SHOWMOREH

#include <QWidget>
#include "classwidget.h"
#include "classtop.h"
#include "jsonfunc.h"

class ShowMore : public QWidget
{
    Q_OBJECT

public:
    explicit ShowMore(QWidget *parent = 0);
    ~ShowMore();
    void setElement(int category,const CLASSSTRUCTMAP &classStruct);
    void setTopName(int category,const CATEGORYMAP &cateGoryMap);
    void setElementNum(const ELEMENTNUMBERMAP &elementNum);

private:
    ClassTop *moreClassTop;
    QVBoxLayout *mainLayout;
    Element *moreElement;
    QGridLayout *eleLayout;
    QWidget  **spaceWidget;

    bool eventFilter(QObject *watched, QEvent *event);
    int categoryFlag;
    int elementNumber;

signals:
    void installApp(QString,int);
    void updateApp(QString,int);
    void detailspageSig(int);

protected slots:
    void sendInstallApp(QString name,int id);
    void sendUpdateApp(QString name,int id);
    void updatePackageStatus(QString name, bool bo, int flag);
    void sendDetailSig(int id);
    void updateMorePkgStatus(QString name);
};

#endif // SHOWMOREH
