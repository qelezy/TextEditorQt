#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>

class ColorButton : public QPushButton
{
    Q_OBJECT
public:
    ColorButton(QColor color, QWidget *parent = nullptr);
    QColor getColor();
private:
    QColor color;
};

#endif // COLORBUTTON_H
