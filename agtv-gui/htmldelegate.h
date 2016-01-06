#ifndef HTMLDELEGATE_H
#define HTMLDELEGATE_H

#include <QObject>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QTextDocument>

class HTMLDelegate : public QStyledItemDelegate
{
protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};


#endif // HTMLDELEGATE_H
