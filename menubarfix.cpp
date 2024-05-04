#include "menubarfix.h"

MenuBarFix::MenuBarFix(QWidget *parent) : QMenuBar(parent) {}

void MenuBarFix::mouseMoveEvent(QMouseEvent *event)
{
    if (cursor() != Qt::ArrowCursor) {
        setCursor(Qt::ArrowCursor);
    }
    QMenuBar::mouseMoveEvent(event);
}
