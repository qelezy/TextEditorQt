#ifndef MENUBARFIX_H
#define MENUBARFIX_H

#include <QMenuBar>

class MenuBarFix : public QMenuBar
{
public:
    MenuBarFix(QWidget *parent = nullptr);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // MENUBARFIX_H
