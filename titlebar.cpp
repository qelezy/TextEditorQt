#include "titlebar.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

TitleBar::TitleBar(QWidget *parent) : QFrame(parent)
{
    isDragging = false;
    setMouseTracking(true);
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (qobject_cast<QPushButton*>(childAt(event->pos())) == nullptr)
        {
            dragPosition = event->globalPosition().toPoint() - window()->pos();
            isDragging = true;
        }
    }
    QFrame::mousePressEvent(event);
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging) {
        if (window()->isMaximized()) {
            MainWindow *mainWindow = qobject_cast<MainWindow*>(window());
            Ui::MainWindow *ui = mainWindow->getUI();
            QIcon icon;
            icon.addFile(":/icons/fullscreen_icon.svg", QSize(18, 18));
            ui->buttonMaximize->setIcon(icon);
            mainWindow->setMaximizedFlag(false);
            ui->window->setStyleSheet("QFrame {background-color: rgb(28, 31, 41); color: white; border: 1px solid rgb(47, 50, 60); border-radius: 10px;}");
            ui->statusBar->setStyleSheet("QFrame {border: none; border-top-left-radius: 0; border-top-right-radius: 0; border-bottom-left-radius: 10px; border-bottom-right-radius: 10px; background-color: rgb(28, 31, 41); }");
            double windowWidth = mainWindow->width();
            mainWindow->showNormal();
            double offset = dragPosition.x() / windowWidth * mainWindow->width();
            if (offset > mainWindow->width() - 109) { // 109 - Ширина правой части заголовка (кнопки закрыть, свернуть, развернуть на весь экран)
                offset = mainWindow->width() - 109;
            }
            mainWindow->move(event->globalPosition().toPoint().x() - offset, event->globalPosition().toPoint().y() - dragPosition.y());
            dragPosition = event->globalPosition().toPoint() - window()->pos();
        }
        window()->move(event->globalPosition().toPoint() - dragPosition);
    }
    QFrame::mouseMoveEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (isDragging) {
        isDragging = false;
    }
    QFrame::mouseReleaseEvent(event);
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        MainWindow *mainWindow = qobject_cast<MainWindow*>(window());
        QIcon icon;
        if (mainWindow->getMaximizedFlag()) {
            mainWindow->showNormal();
            mainWindow->setMaximizedFlag(false);
            icon.addFile(":/icons/fullscreen_icon.svg", QSize(18, 18));
            findChild<QPushButton*>("buttonMaximize")->setIcon(icon);
            qobject_cast<QFrame*>(parent())->setStyleSheet("QFrame {background-color: rgb(28, 31, 41); color: white; border: 1px solid rgb(47, 50, 60); border-radius: 10px;}");
        } else {
            mainWindow->showMaximized();
            mainWindow->setMaximizedFlag(true);
            icon.addFile(":/icons/fullscreen_exit_icon.svg", QSize(18, 18));
            findChild<QPushButton*>("buttonMaximize")->setIcon(icon);
            qobject_cast<QFrame*>(parent())->setStyleSheet("QFrame {background-color: rgb(28, 31, 41); color: white;}");
        }
    }
    QFrame::mouseDoubleClickEvent(event);
}

