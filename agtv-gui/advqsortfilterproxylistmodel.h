#ifndef ADVQSORTFILTERPROXYLISTMODEL_H
#define ADVQSORTFILTERPROXYLISTMODEL_H

#include <QObject>

#include "advqsortfilterproxymodel.h"

class AdvQSortFilterProxyListModel : public AdvQSortFilterProxyModel
{
    Q_OBJECT
public:
    AdvQSortFilterProxyListModel(QObject *parent);
    QVariant data ( const QModelIndex & index, int role ) const;
    QString buildHtmlString(const int row) const;
};

#endif // ADVQSORTFILTERPROXYLISTMODEL_H
