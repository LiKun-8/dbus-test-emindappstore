#ifndef SOFTWARECENTERWINDOWH
#define SOFTWARECENTERWINDOWH


#include <QWidget>
#include <QFile>
#include <QApplication>
#include <QDebug>
#include <QMap>
#include "jsonfunc.h"
#include "sharedata.h"
#include "managerpage/pkdetailthread.h"
//#include "managerpage/pkgdetailthread.h"
#include "managerpage/managerpage.h"
#include "updatepage/updatepage.h"
#include <QLockFile>

class TitleBar;
class QStackedWidget;
class QHBoxLayout;
class QVBoxLayout;
class HomeWidget;
class SearchWidget;
class DetailWidget;
class QScrollArea;
class ClassPage;
class UpdatePage;
class ManagerPage;
class ShareData;
class JSONFUNC;


class SoftwareCenterWindow : public QWidget
{
    Q_OBJECT

public:
    SoftwareCenterWindow(QWidget *parent = 0);
    ~SoftwareCenterWindow();

    static void setWinStyle(const QString& qssFile){
        QFile qss(qssFile);
        if(qss.open(QFile::ReadOnly)){
        qApp->setStyleSheet(qss.readAll());
        qss.close();
        }else{
            qDebug()<<__FUNCTION__<<" QSS file Open Failed! "<<endl;
        }
    }

    static QLockFile lockFile;
public slots:
    void setWinStatus();
protected:
    void closeEvent(QCloseEvent* event);

signals:
    void toSearch(QString);
    void pageChanged();
    void refreshPg(int);
    void getPkgSizeOk(QMap<QString, QString>);

protected slots:
    void onBtnHome();
    void onBtnClass();
    void onBtnMore();
    void onBtnUpdate();
    void onBtnManager();
    void mousePressEvent(QMouseEvent* event);
    void toPageSearch();
    void toPageDetail(int);
    void nextPage();
    void backPage();
    void refreshPage();
    void onPageChanged(int index);
    void requestFinished(QNetworkReply*);
    void startPkDetailThread(QMap<QString,INSTALLEDSTRUCT> insdMap);
    void sendPkgSize(QMap<QString, QString> insdSizeMap);
    void setBtnRefreshState();

private:
    void createGui();
    void createConnections();
    void createWindow();
    void initVariables();
    void layoutWindow();
    void showSearchImg();
    void checkServer();
    void initTitleBarConns();
    void delConnections();
    int refreshFlag;  //test

private:

    QStackedWidget *stackWidget;
    TitleBar *titleBar;
    QVBoxLayout *vly;

    HomeWidget *pageHome;
    ClassPage *pageClass;
    UpdatePage *pageUpdate;
    ManagerPage *pageManager;
    SearchWidget *pageSearch;
    DetailWidget *pageDetail;

    Qt::WindowFlags flags;
    QList<int> pageList;
//    QScrollArea *scrollArea;
    qint32 pageIndex;
    qint32 mOldIndex;
    qint32 mLastPage;

    ShareData *shareData;
    JSONFUNC *jsonFunc;
    qint32 mConnState;
    bool mConnFlag;

//managerpage get packages details
    QThread *pkthread;
    PkDetailThread *pkDetailThread;
    /*QMap<QString, QString> insdSizeMap;*/     // <packageid, installedsize>
};

#endif // SoftwareCenterWindowH
