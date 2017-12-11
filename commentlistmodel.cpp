#include "commentlistmodel.h"
#include <QDebug>

CommentListModel::CommentListModel(QObject* parent): QAbstractListModel (parent),m_bError(false)
{
    int role = Qt::UserRole;
    m_roleNames.insert(role++,"comment");
    m_roleNames.insert(role++,"commentMark");
    m_roleNames.insert(role++,"date");
    m_roleNames.insert(role++,"userName");
    m_roleNames.insert(role++,"userHeadImg");
    mCommentId = 0;
}

CommentListModel::~CommentListModel()
{
    clear();
}

void CommentListModel::load()
{

}

void CommentListModel::reset()
{
    clear();
}

void CommentListModel::clear()
{
    beginRemoveRows(QModelIndex(),0,m_CommDatas.size());
    m_CommDatas.clear();
    endRemoveRows();
//    int count = m_CommDatas.size();
//    if(count>0){
//        for(int i=0;i<count;i++){
//            delete m_CommDatas.at(i);
//        }
//        m_CommDatas.clear();
//    }
}

void CommentListModel::remove(int index)
{
    beginRemoveRows(QModelIndex(),index,index);
    m_CommDatas.takeAt(index);
    endRemoveRows();
}

QHash<int,QByteArray> CommentListModel::roleNames() const
{
    return m_roleNames;
}

int CommentListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_CommDatas.size();
}

QVariant CommentListModel::data(const QModelIndex& index,int role) const
{
//    CommentData *d = m_CommDatas[index.row()];
//    return d->at(role - Qt::UserRole);
    CommentData d = m_CommDatas[index.row()];
    return d.at(role - Qt::UserRole);
}

void CommentListModel::reload()
{
    beginResetModel();
    clear();
    endResetModel();
}

void CommentListModel::insert(int index,CommentData& data)
{
//    emit sigInsert();
    beginInsertRows(QModelIndex(),index,index);
    m_CommDatas.append(data);
    endInsertRows();
}


qint32 CommentListModel::pageNumber(){
    return mPageNumber;
}

//set qml listView page button's number
void CommentListModel::setPageNumber(const qint32 pgNum){
    mPageNumber = pgNum;
    emit pageNumberChanged();
}

void CommentListModel::sigMorePage()
{
    emit nextPage(mCommentId);
}

qint32 CommentListModel::commCount()
{
    return mCommCount;
}

void CommentListModel::setCommCount(const qint32 commentCount)
{
    mCommCount = commentCount;
    emit commCountChanged();
}

void CommentListModel::setCount(qint32 commCounts)
{
    setCommCount(commCounts);
    emit sigSetCount();
}

bool CommentListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole )
        emit dataChanged(index,index);
    return false;
}
