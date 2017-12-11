#ifndef TASKBAR_H
#define TASKBAR_H

#include <QWidget>
#include <QTableWidget>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class TaskBar : public QWidget
{
    Q_OBJECT
public:
    explicit TaskBar(QWidget *parent,QString taskstr,QString onekeystr);
    void setTaskLabel(int count);
    void hideOnekeyButton();
    QPushButton* getOnekeyBtn();
private:
    QHBoxLayout *hbLayout;
    QLabel *vblueLabel;
    QLabel *taskLabel;
    QPushButton *onekeyButton;
    QString taskStr;
    QString onekeyStr;


signals:

public slots:
};

#endif // TASKBAR_H
