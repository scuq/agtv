#include "advqsortfilterproxymodel.h"





AdvQSortFilterProxyModel::AdvQSortFilterProxyModel(QObject *parent)
{
    showOffline = true;
}

QVariant AdvQSortFilterProxyModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::BackgroundRole ) {

        QBrush demonicBackground(QColor(255, 60, 43));
        QBrush evilBackground(QColor(255,123,71));
        QBrush unkownBackground(QColor(102, 191, 255));
        QBrush goodBackground(QColor(110, 255, 96));


        if (index.column() == 1) {
            if (index.data().toString() == "online") {
                return QVariant(goodBackground);
            }
        }

    }






    if ( role == Qt::TextAlignmentRole )
    {
        if (index.column() == 1) {
            return Qt::AlignVCenter | Qt::AlignHCenter;

        }
    }

    return QSortFilterProxyModel::data( index, role );
}

bool AdvQSortFilterProxyModel::updateCol(int keycol, QVariant key, int updatecol, QVariant updatedata)
{

    bool updateok = false;

    for(int i = 0; i<this->sourceModel()->rowCount(); ++i)
    {
        QModelIndex key_index = this->sourceModel()->index(i,keycol);
        QModelIndex update_index = this->sourceModel()->index(i,updatecol);

        if ( this->sourceModel()->itemData(key_index)[0].toString() == key.toString() )  {
            updateok = this->sourceModel()->setData(update_index,updatedata);
        }
    }

    return updateok;


}

QVariant AdvQSortFilterProxyModel::getColData(int keycol, QVariant key, int updatecol)
{

    QVariant data = "";

    for(int i = 0; i<this->sourceModel()->rowCount(); ++i)
    {
        QModelIndex key_index = this->sourceModel()->index(i,keycol);
        QModelIndex update_index = this->sourceModel()->index(i,updatecol);

        if ( this->sourceModel()->itemData(key_index)[0].toString() == key.toString() )  {

            data = this->sourceModel()->data(update_index,0);
        }
    }

    return data;


}

void AdvQSortFilterProxyModel::setShowOffline(bool showoffline)
{
    this->showOffline = showoffline;
}

bool AdvQSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{

    QModelIndex index0 = sourceModel()->index(source_row, 0, source_parent);
    QModelIndex index1 = sourceModel()->index(source_row, 1, source_parent);
    QModelIndex index2 = sourceModel()->index(source_row, 2, source_parent);

    if (showOffline == true) {

        return (sourceModel()->data(index0).toString().contains(filterRegExp()) || sourceModel()->data(index2).toString().contains(filterRegExp()));
    } else {
        return ((sourceModel()->data(index0).toString().contains(filterRegExp()) || sourceModel()->data(index2).toString().contains(filterRegExp())) && sourceModel()->data(index1).toString() != "offline");
    }
}
