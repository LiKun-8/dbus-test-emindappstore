#ifndef CLASSTOP
#define CLASSTOP
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QWidget>
#include <QObject>

class ClassTop:public QWidget
{
    Q_OBJECT

public:
    ClassTop();
    ~ClassTop();
    //设置更多按钮文本
    void setBtnData();
    //设置软件项名称
    void setLabelData(QString data);
    //分类项
    int category;
    //设置分类项
    void setcategory(int cate);
    void setTopbtnHide();

private:
    //更多按钮
    QPushButton *btnTop;
    //软件项名称
    QLabel *labelTop;
    //横线
    QLabel *lineTop;
    //弹簧布局
    QSpacerItem *spacerTop;
    //布局
    QHBoxLayout *hbTopLayout;
    QVBoxLayout *vbTopLayout;

signals:
    void showAll(int flag);

protected slots:
    void sendSlotBtn();
};

#endif // CLASSTOP

