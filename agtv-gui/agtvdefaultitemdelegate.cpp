#include "agtvdefaultitemdelegate.h"

AgtvDefaultItemDelegate::AgtvDefaultItemDelegate()
{
    showSelectedBorder = false;
}

QWidget *AgtvDefaultItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *editor = new QLineEdit(parent);

    return editor;
}

void AgtvDefaultItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

}

void AgtvDefaultItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}

void AgtvDefaultItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
     editor->setGeometry(option.rect);
}

void AgtvDefaultItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 myOption = option;

    myOption.text = index.data().toString();



   if(option.state & QStyle::State_Selected) {
                myOption.font.setBold(true);//text to be bold when selected
   }  else {
               myOption.font.setBold(false);//text not to be bold when unselected
   }

   if (index.column() == 1) {
        if  (genericHelper::isOnline(myOption.text)) {
            myOption.backgroundBrush = (QBrush(QColor(100, 200, 100, 200)));
        }

        if  (genericHelper::isHosting(myOption.text)) {
            myOption.backgroundBrush = (QBrush(QColor(102, 191, 255)));
        }

        if  (genericHelper::isPlaylist(myOption.text)) {
            myOption.backgroundBrush = (QBrush(QColor(210, 76, 222)));
        }
   }

   // Border around selected item
   if(! showSelectedBorder && myOption.state & QStyle::State_HasFocus)
       myOption.state = myOption.state & ~QStyle::State_HasFocus;


    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
}

void AgtvDefaultItemDelegate::setShowSelectedBorder(const bool _showSelectedBorder)
{
    this->showSelectedBorder = _showSelectedBorder;
}

bool AgtvDefaultItemDelegate::isShowSelectedBorder()
{
    return this->showSelectedBorder;
}
