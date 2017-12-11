#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QObject>
#include <qglobal.h>
#include <QWidget>
#include <QQuickView>

class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QFrame;
class QQuickWidget;
class QQmlEngine;
class QScrollArea;
class RecommendWidget;
class ShareData;
class JSONFUNC;
class QQmlContext;
class DataManager;

class HomeWidget : public QWidget
{
    Q_OBJECT
public:
    HomeWidget(QWidget *parent, ShareData *data, JSONFUNC *json);
    ~HomeWidget();

    RecommendWidget *recommWidget;

protected:
    void mousePressEvent(QMouseEvent* event);


signals:
    void toDetailPage(int);

public slots:
    void refreshPage(int);
    void setElement();
    void bannerJsonOk();

private:
    QHBoxLayout *hlyPicture;
    QVBoxLayout *vly;
    QVBoxLayout *vly1;
    QLabel *lbHots;
    QFrame *line1;
    QFrame *line2;
    QHBoxLayout *hly1;
    QHBoxLayout *hly2;

    QQuickWidget *imgBox;
    QQmlEngine *qmlEngine;
    DataManager *bannerImgData;
    QQmlContext *ctx1;
    ShareData *shareData;
    JSONFUNC *jsonFunc;
    QScrollArea *scrollArea;
    QWidget *homeArea;
    QWidget *bannerWidget;
    QQuickView* bannerView;
};

#endif // HOMEWIDGET_H
