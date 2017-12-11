#ifndef DETAILIMGLISTMODEL_H
#define DETAILIMGLISTMODEL_H

#include <QObject>
#include <qglobal.h>

class DetailImgListModel : public QObject
{
    Q_OBJECT
public:
    explicit DetailImgListModel(QObject *parent = 0);

signals:

public slots:
};

#endif // DETAILIMGLISTMODEL_H