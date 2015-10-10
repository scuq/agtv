#ifndef ADVQSORTFILTERPROXYMODEL_H
#define ADVQSORTFILTERPROXYMODEL_H


#include <QSortFilterProxyModel>
#include <QModelIndex>
#include <QColor>
#include <QBrush>
#include <QDebug>

class AdvQSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:

   AdvQSortFilterProxyModel(QObject* parent);
   QVariant data ( const QModelIndex & index, int role ) const;
   bool updateCol(int keycol, QVariant key,  int updatecol, QVariant updatedata);
   void setShowOffline(bool showoffline);

protected:
    bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;

private:
    bool showOffline;


};

#endif // ADVQSORTFILTERPROXYMODEL_H




