#include "colorbutton.h"

ColorButton::ColorButton(QColor color, QWidget *parent) : QPushButton(parent), color(color)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setMinimumSize(20, 20);
    setMaximumSize(20, 20);
}

QColor ColorButton::getColor()
{
    return color;
}
