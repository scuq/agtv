#ifndef AGTVDEFAULTITEMDELEGATE_H
#define AGTVDEFAULTITEMDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QLabel>
#include <QLineEdit>
#include <QModelIndex>
#include <generichelper.h>
#include <QPainter>
#include <QStylePainter>
#include <QApplication>
#include <QStyle>
#include <QUrl>
#include <QRegExpValidator>
#include <QMenu>
#include <QAction>

class AgtvDefaultItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    AgtvDefaultItemDelegate();
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setShowSelectedBorder(const bool _showSelectedBorder);
    bool isShowSelectedBorder();

private:
    bool showSelectedBorder;

};

#endif // AGTVDEFAULTITEMDELEGATE_H
