#include "advqsortfilterproxylistmodel.h"

AdvQSortFilterProxyListModel::AdvQSortFilterProxyListModel(QObject *parent) : AdvQSortFilterProxyModel(parent)
{

}

QString AdvQSortFilterProxyListModel::buildHtmlString(const int row) const
{
    QString channelname = QSortFilterProxyModel::data(this->index(row,1)).toString();
    QString viewers = "Viewers: " + QSortFilterProxyModel::data(this->index(row,2)).toString();
    return channelname + "<br>" + viewers;
}

QVariant AdvQSortFilterProxyListModel::data(const QModelIndex &index, int role) const
{
//    if(role == Qt::DisplayRole) {
//        if (index.column() == 1 ) {
//            QString retString = buildHtmlString(index.row());
//            return retString;
//        }
//    }

//    if (role == Qt::DecorationRole) {
//          QIcon icon = QIcon(":/images/agtv_16.png");
//          return icon;
//   }

    return AdvQSortFilterProxyModel::data( index, role );
}
