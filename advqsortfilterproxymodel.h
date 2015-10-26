#ifndef ADVQSORTFILTERPROXYMODEL_H
#define ADVQSORTFILTERPROXYMODEL_H


#include <QSortFilterProxyModel>
#include <QModelIndex>
#include <QColor>
#include <QBrush>
#include <QDebug>
#include "generichelper.h"

class AdvQSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:

   AdvQSortFilterProxyModel(QObject* parent);
   QVariant data ( const QModelIndex & index, int role ) const;
   bool updateCol(int keycol, QVariant key,  int updatecol, QVariant updatedata);
   QVariant getColData(int keycol, QVariant key, int updatecol);

   bool deleteCol(int keycol, QVariant key);

   void setShowOffline(bool showoffline);
   void setShowApproximateViewerCount(const bool approxviewercount);

protected:
    bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;

private:
    bool showOffline;
    bool showApproximateViewerCount;


};

#endif // ADVQSORTFILTERPROXYMODEL_H




