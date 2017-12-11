#ifndef SearchWidgetH
#define SearchWidgetH

#include <QWidget>
#include "ClassAndMorePage/classwidget.h"
#include "ClassAndMorePage/classtop.h"
#include "jsonfunc.h"
#include "sharedata.h"
#include <QVBoxLayout>
class QSPacerItem;
class QScrollArea;
class TipsWidget;

class SearchWidget : public QWidget
{
    Q_OBJECT

public:
    SearchWidget(QWidget *parent,ShareData* shareData,JSONFUNC *jsonFunc);
    ~SearchWidget();
    QWidget *moreWidget;
    void setElement(int category,const CLASSSTRUCTMAP &classStruct);
    void setElement(QString keyText1,const CLASSSTRUCTMAP &classStruct);
    void setTopName(QString keyText);
    void setElementNum(const ELEMENTNUMBERMAP &elementNum);

private:
    ClassTop *moreClassTop;
    QVBoxLayout *mainLayout;
    Element *moreElement;
    QGridLayout *eleLayout;
    QWidget  **spaceWidget;
//    QLabel *lbTips;
//    QLabel *lbTips1;
//    QLabel *lbWarnImg;
//    QHBoxLayout *hly1;
//    QVBoxLayout *vly2;
//    QSpacerItem *spItem1;
    TipsWidget *tipsWidget;

    bool eventFilter(QObject *watched, QEvent *event);
    int categoryFlag;
    int elementNumber;
    ShareData *sdata;
    JSONFUNC *jdata;
    QVBoxLayout *vly;
    QVBoxLayout *vly1;
    QSpacerItem *spItem;
    QScrollArea *scrollArea;
    QWidget *searchArea;
    QString mKeyText;

signals:
    void installApp(QString,int);
    void updateApp(QString,int);
    void toDetailSig(int);

protected slots:
    void sendInstallApp(QString name,int id);
    void sendUpdateApp(QString name,int id);
    void updatePackageStatus(QString name, bool bo, int flag);

    void doSearch(QString keyText);
    void refreshPage(int);
    void toDetailPage(int id);

};

#endif // SearchWidgetH
