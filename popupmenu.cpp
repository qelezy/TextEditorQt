#include "popupmenu.h"

PopupMenu::PopupMenu(QWidget *parent) : QMenu(parent)
{
    setStyleSheet("QMenu { border: 1px solid rgb(150, 150, 150); border-radius: 5px; background-color: white; } QMenu::item { color: black; border-bottom-left-radius: 5px; border-bottom-right-radius: 5px; padding-left: 10px; padding-top: 5px; padding-bottom: 5px; } QMenu::item:selected { background-color: rgb(235, 235, 235); } QMenu::icon { padding: 10px; } QMenu::separator { height: 1px; background-color: rgb(200, 200, 200); } QFrame { border: none; } QPushButton { border: none; border-radius: 5px; } QPushButton::hover::!pressed { margin: 1px; border-radius: 9px; } QPushButton::pressed { margin: 3px; border-radius: 7px; }");
}

void PopupMenu::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    QWidget* parentWidget = this->parentWidget();
    if (parentWidget)
    {
        qreal menuX;
        if (geometry().x() - width() + parentWidget->width() < 0) {
            menuX = 2;
        } else {
            menuX = qMin(parentWidget->window()->screen()->size().width(), parentWidget->mapToGlobal(parentWidget->rect().bottomRight()).x()) - sizeHint().width();
        }
        move(menuX, y());
    }
}
