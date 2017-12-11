#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H


#include <QWidget>
#include <qglobal.h>
#include <QLineEdit>
#include <QFocusEvent>
#include <QObject>

class CustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CustomLineEdit(QWidget* parent);
    ~CustomLineEdit();
protected:
    void focusInEvent(QFocusEvent* event);
    void focusOutEvent(QFocusEvent* event);

signals:
    void hideSearchLabel();
    void showSearchLabel();

public slots:


private:
};

#endif // CUSTOMLINEEDIT_H
