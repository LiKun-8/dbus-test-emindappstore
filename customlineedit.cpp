#include "customlineedit.h"
#include <QDebug>

CustomLineEdit::CustomLineEdit(QWidget* parent)
    : QLineEdit(parent)
{
    setClearButtonEnabled(true);
    setFocusPolicy(Qt::ClickFocus);

}

CustomLineEdit::~CustomLineEdit()
{

}

void CustomLineEdit::focusInEvent(QFocusEvent* event)
{
    if(event->gotFocus()){
        emit hideSearchLabel();
        setReadOnly(false);
        setCursor(QCursor(Qt::IBeamCursor));
    }
}

void CustomLineEdit::focusOutEvent(QFocusEvent* event)
{
    if(event->lostFocus() && text().isEmpty() ){
        qDebug()<<"lost focus";
        emit showSearchLabel();
        setReadOnly(true);
    }
}


