#ifndef COMMENTLISTMODEL_H
#define COMMENTLISTMODEL_H

#include <QObject>
#include <qglobal.h>
#include <QByteArray>
#include <QHash>
#include <QVector>
#include <QAbstractListModel>

typedef QVector<QString> CommentData;
class CommentListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    CommentListModel(QObject* parent=0);
    ~CommentListModel();
//    QVector<CommentData*> m_CommDatas;
    QVector<CommentData> m_CommDatas;

    qint32 mCommentId;
    void reload();
    void insert(int index,CommentData& data);
    void clear();
    void reset();
    void setCount(qint32 commCounts);
    void remove(int index);



    Q_PROPERTY(qint32 pageNumber READ pageNumber WRITE setPageNumber NOTIFY pageNumberChanged)
    qint32 pageNumber();
    void setPageNumber(const qint32 pgNum);
    Q_PROPERTY(qint32 commCount READ commCount WRITE setCommCount NOTIFY commCountChanged)
    qint32 commCount();
    void setCommCount(const qint32 commentCount);
    Q_INVOKABLE void sigMorePage();



signals:
    void pageNumberChanged();
    void nextPage(int commentId);
    void commCountChanged();
    void sigSetCount();
    void sigClear();
    void sigInsert();
    Q_INVOKABLE void sigCommCurrentVer();
    Q_INVOKABLE void sigCommHistoryVer();

private:
    void load();
    QHash<int,QByteArray> roleNames() const;

protected:
    int rowCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index,int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

private:
    QString m_strError;
    bool m_bError;
    QHash<int, QByteArray> m_roleNames;
    CommentData commData;
    qint32 mPageNumber;
    qint32 mCommCount;
};

#endif // COMMENTLISTMODEL_H
