#ifndef FUNCWIDGET_H
#define FUNCWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>

class FuncWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FuncWidget(QWidget *parent, QString funcStr);
    QTextEdit * getNfuncEdit();
    QPushButton * getHideButton();

signals:
    int sigTextHeight(int);

public slots:
private:
    QString funcText;
    bool event(QEvent *event);
    QPushButton *hideButton;
    QHBoxLayout *funcLayout;
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;
    QLabel *funcLabel;
    QTextEdit *nfuncEdit;
//    QLabel *nfuncLabel;

};

#endif // FUNCWIDGET_H
