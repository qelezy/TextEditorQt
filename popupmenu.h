#ifndef POPUPMENU_H
#define POPUPMENU_H

#include <QMenu>
#include <QScreen>

class PopupMenu : public QMenu
{
    Q_OBJECT
public:
    PopupMenu(QWidget *parent = nullptr);
protected:
    void showEvent(QShowEvent *event) override;
};

#endif // POPUPMENU_H
