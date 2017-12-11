#ifndef HOMEIMGLISTMODEL_H
#define HOMEIMGLISTMODEL_H

#include <QObject>
#include <qglobal.h>

class HomeImgListModel : public QObject
{
    Q_OBJECT
public:
    explicit HomeImgListModel(QObject *parent = 0);

signals:

public slots:
};

#endif // HOMEIMGLISTMODEL_H