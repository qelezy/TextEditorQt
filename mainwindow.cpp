#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "popupmenu.h"
#include "colorbutton.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    isMaximized = false;
    re = QRegularExpression("(\\s|\\n|\\r)+");
    connectButtons();
    setupMenuBar();
    setupToolBar();
    ui->fontSizeComboBox->setCurrentIndex(4);
    currentBulletStyle = QTextListFormat::ListDisc, currentNumberedStyle = QTextListFormat::ListDecimal;
    currentFGColor = Qt::black, currentBGColor = Qt::yellow;

    ui->textEdit->document()->setUndoRedoEnabled(true);

    connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChange()));
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(onTextChanged()));

    connectMenuActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

Ui::MainWindow* MainWindow::getUI()
{
    return ui;
}

bool MainWindow::getMaximizedFlag()
{
    return isMaximized;
}

void MainWindow::setMaximizedFlag(bool flag)
{
    isMaximized = flag;
}

void MainWindow::onButtonMaximize()
{
    QIcon icon;
    if (isMaximized) {
        showNormal();
        isMaximized = false;
        icon.addFile(":/icons/fullscreen_icon.svg", QSize(18, 18));
        ui->buttonMaximize->setIcon(icon);
        ui->window->setStyleSheet("QFrame {background-color: rgb(28, 31, 41); color: white; border: 1px solid rgb(47, 50, 60); border-radius: 10px;}");
    } else {
        showMaximized();
        isMaximized = true;
        icon.addFile(":/icons/fullscreen_exit_icon.svg", QSize(18, 18));
        ui->buttonMaximize->setIcon(icon);
        ui->window->setStyleSheet("QFrame {background-color: rgb(28, 31, 41); color: white;}");
    }
}

void MainWindow::onCursorPositionChange()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat charFormat = cursor.charFormat();
    QTextBlockFormat blockFormat = cursor.blockFormat();
    QTextList *list = cursor.currentList();

    // Жирный шрифт
    if (charFormat.fontWeight() == QFont::Bold) {
        ui->boldButton->setChecked(true);
    } else {
        ui->boldButton->setChecked(false);
    }

    // Курсив
    if (charFormat.fontItalic()) {
        ui->italicButton->setChecked(true);
    } else {
        ui->italicButton->setChecked(false);
    }

    // Подчеркнутый
    if (charFormat.fontUnderline()) {
        ui->underlineButton->setChecked(true);
    } else {
        ui->underlineButton->setChecked(false);
    }

    // Перечеркнутый
    if (charFormat.fontStrikeOut()) {
        ui->strikethroughButton->setChecked(true);
    } else {
        ui->strikethroughButton->setChecked(false);
    }

    if (charFormat.verticalAlignment() == QTextCharFormat::AlignSubScript) { // Нижний индекс
        ui->subscriptButton->setChecked(true);
        ui->superscriptButton->setChecked(false);
    } else if (charFormat.verticalAlignment() == QTextCharFormat::AlignSuperScript){ // Верхний индекс
        ui->subscriptButton->setChecked(false);
        ui->superscriptButton->setChecked(true);
    } else {
        ui->subscriptButton->setChecked(false);
        ui->superscriptButton->setChecked(false);
    }

    // Шрифт
    ui->fontComboBox->setCurrentFont(charFormat.font().family());
    // Размер шрифта
    ui->fontSizeComboBox->setCurrentText(QString::number(charFormat.font().pointSize()));

    // Выравнивание
    switch (blockFormat.alignment()) {
    case Qt::AlignLeft:
        ui->leftAlignButton->setChecked(true);
        ui->centerAlignButton->setChecked(false);
        ui->rightAlignButton->setChecked(false);
        ui->justifyAlignButton->setChecked(false);
        break;
    case Qt::AlignCenter:
        ui->leftAlignButton->setChecked(false);
        ui->centerAlignButton->setChecked(true);
        ui->rightAlignButton->setChecked(false);
        ui->justifyAlignButton->setChecked(false);
        break;
    case Qt::AlignRight:
        ui->leftAlignButton->setChecked(false);
        ui->centerAlignButton->setChecked(false);
        ui->rightAlignButton->setChecked(true);
        ui->justifyAlignButton->setChecked(false);
        break;
    case Qt::AlignJustify:
        ui->leftAlignButton->setChecked(false);
        ui->centerAlignButton->setChecked(false);
        ui->rightAlignButton->setChecked(false);
        ui->justifyAlignButton->setChecked(true);
        break;
    }

    // Списки
    if (list) {
        QTextListFormat listFormat = list->format();
        switch (listFormat.style()) {
        case QTextListFormat::ListDisc: {
            ui->currentBulletedListButton->setChecked(true);
            ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
            ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
            QButtonGroup *buttonGroup = ui->bulletedListButton->menu()->findChild<QButtonGroup*>();
            buttonGroup->button(1)->setChecked(true);
            buttonGroup->button(2)->setChecked(false);
            buttonGroup->button(3)->setChecked(false);
            ui->currentNumberedListButton->setChecked(false);
            ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
            ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
            break;
        }
        case QTextListFormat::ListCircle: {
            ui->currentBulletedListButton->setChecked(true);
            ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
            ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
            QButtonGroup *buttonGroup = ui->bulletedListButton->menu()->findChild<QButtonGroup*>();
            buttonGroup->button(1)->setChecked(false);
            buttonGroup->button(2)->setChecked(true);
            buttonGroup->button(3)->setChecked(false);
            ui->currentNumberedListButton->setChecked(false);
            ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
            ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
            break;
        }
        case QTextListFormat::ListSquare: {
            ui->currentBulletedListButton->setChecked(true);
            ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
            ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
            QButtonGroup *buttonGroup = ui->bulletedListButton->menu()->findChild<QButtonGroup*>();
            buttonGroup->button(1)->setChecked(false);
            buttonGroup->button(2)->setChecked(false);
            buttonGroup->button(3)->setChecked(true);
            ui->currentNumberedListButton->setChecked(false);
            ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
            ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
            break;
        }

        case QTextListFormat::ListDecimal: {
            ui->currentBulletedListButton->setChecked(false);
            ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
            ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
            QButtonGroup *buttonGroup = ui->numberedListButton->menu()->findChild<QButtonGroup*>();
            buttonGroup->button(1)->setChecked(true);
            buttonGroup->button(2)->setChecked(false);
            buttonGroup->button(3)->setChecked(false);
            buttonGroup->button(4)->setChecked(false);
            buttonGroup->button(5)->setChecked(false);
            ui->currentNumberedListButton->setChecked(true);
            ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
            ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
            break;
        }
        case QTextListFormat::ListLowerRoman: {
            ui->currentBulletedListButton->setChecked(false);
            ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
            ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
            QButtonGroup *buttonGroup = ui->numberedListButton->menu()->findChild<QButtonGroup*>();
            buttonGroup->button(1)->setChecked(false);
            buttonGroup->button(2)->setChecked(true);
            buttonGroup->button(3)->setChecked(false);
            buttonGroup->button(4)->setChecked(false);
            buttonGroup->button(5)->setChecked(false);
            ui->currentNumberedListButton->setChecked(true);
            ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
            ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
            break;
        }
        case QTextListFormat::ListUpperRoman: {
            ui->currentBulletedListButton->setChecked(false);
            ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
            ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
            QButtonGroup *buttonGroup = ui->numberedListButton->menu()->findChild<QButtonGroup*>();
            buttonGroup->button(1)->setChecked(false);
            buttonGroup->button(2)->setChecked(false);
            buttonGroup->button(3)->setChecked(true);
            buttonGroup->button(4)->setChecked(false);
            buttonGroup->button(5)->setChecked(false);
            ui->currentNumberedListButton->setChecked(true);
            ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
            ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
            break;
        }
        case QTextListFormat::ListLowerAlpha: {
            ui->currentBulletedListButton->setChecked(false);
            ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
            ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
            QButtonGroup *buttonGroup = ui->numberedListButton->menu()->findChild<QButtonGroup*>();
            buttonGroup->button(1)->setChecked(false);
            buttonGroup->button(2)->setChecked(false);
            buttonGroup->button(3)->setChecked(false);
            buttonGroup->button(4)->setChecked(true);
            buttonGroup->button(5)->setChecked(false);
            ui->currentNumberedListButton->setChecked(true);
            ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
            ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
            break;
        }
        case QTextListFormat::ListUpperAlpha: {
            ui->currentBulletedListButton->setChecked(false);
            ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
            ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
            QButtonGroup *buttonGroup = ui->numberedListButton->menu()->findChild<QButtonGroup*>();
            buttonGroup->button(1)->setChecked(false);
            buttonGroup->button(2)->setChecked(false);
            buttonGroup->button(3)->setChecked(false);
            buttonGroup->button(4)->setChecked(false);
            buttonGroup->button(5)->setChecked(true);
            ui->currentNumberedListButton->setChecked(true);
            ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
            ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
            break;
        }

        default:
            break;
        }
    } else {
        ui->currentBulletedListButton->setChecked(false);
        ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
        ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
        ui->currentNumberedListButton->setChecked(false);
        ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
        ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
    }
}

void MainWindow::onTextChanged()
{
    // Подсчет количества слов
    int wordCount = ui->textEdit->toPlainText().split(re, Qt::SkipEmptyParts).count();
    ui->wordCount->setText("Число слов: " + QString::number(wordCount));

    // Фикс списков
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextList *list = cursor.currentList();
    if (list) {
        QTextListFormat listFormat = list->format();
        switch (listFormat.style()) {
        case QTextListFormat::ListDisc: {
            ui->currentBulletedListButton->setChecked(true);
            ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
            ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
            QButtonGroup *buttonGroup = ui->bulletedListButton->menu()->findChild<QButtonGroup*>();
            buttonGroup->button(1)->setChecked(true);
            buttonGroup->button(2)->setChecked(false);
            buttonGroup->button(3)->setChecked(false);
            ui->currentNumberedListButton->setChecked(false);
            ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
            ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
            break;
        }
        case QTextListFormat::ListCircle: {
            ui->currentBulletedListButton->setChecked(true);
            ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
            ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
            QButtonGroup *buttonGroup = ui->bulletedListButton->menu()->findChild<QButtonGroup*>();
            buttonGroup->button(1)->setChecked(false);
            buttonGroup->button(2)->setChecked(true);
            buttonGroup->button(3)->setChecked(false);
            ui->currentNumberedListButton->setChecked(false);
            ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
            ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
            break;
        }
        case QTextListFormat::ListSquare: {
            ui->currentBulletedListButton->setChecked(true);
            ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
            ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
            QButtonGroup *buttonGroup = ui->bulletedListButton->menu()->findChild<QButtonGroup*>();
            buttonGroup->button(1)->setChecked(false);
            buttonGroup->button(2)->setChecked(false);
            buttonGroup->button(3)->setChecked(true);
            ui->currentNumberedListButton->setChecked(false);
            ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
            ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
            break;
        }

        case QTextListFormat::ListDecimal: {
            ui->currentBulletedListButton->setChecked(false);
            ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
            ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
            QButtonGroup *buttonGroup = ui->numberedListButton->menu()->findChild<QButtonGroup*>();
            buttonGroup->button(1)->setChecked(true);
            buttonGroup->button(2)->setChecked(false);
            buttonGroup->button(3)->setChecked(false);
            buttonGroup->button(4)->setChecked(false);
            buttonGroup->button(5)->setChecked(false);
            ui->currentNumberedListButton->setChecked(true);
            ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
            ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
            break;
        }
        case QTextListFormat::ListLowerRoman: {
            ui->currentBulletedListButton->setChecked(false);
            ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
            ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
            QButtonGroup *buttonGroup = ui->numberedListButton->menu()->findChild<QButtonGroup*>();
            buttonGroup->button(1)->setChecked(false);
            buttonGroup->button(2)->setChecked(true);
            buttonGroup->button(3)->setChecked(false);
            buttonGroup->button(4)->setChecked(false);
            buttonGroup->button(5)->setChecked(false);
            ui->currentNumberedListButton->setChecked(true);
            ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
            ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
            break;
        }
        case QTextListFormat::ListUpperRoman: {
            ui->currentBulletedListButton->setChecked(false);
            ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
            ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
            QButtonGroup *buttonGroup = ui->numberedListButton->menu()->findChild<QButtonGroup*>();
            buttonGroup->button(1)->setChecked(false);
            buttonGroup->button(2)->setChecked(false);
            buttonGroup->button(3)->setChecked(true);
            buttonGroup->button(4)->setChecked(false);
            buttonGroup->button(5)->setChecked(false);
            ui->currentNumberedListButton->setChecked(true);
            ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
            ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
            break;
        }
        case QTextListFormat::ListLowerAlpha: {
            ui->currentBulletedListButton->setChecked(false);
            ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
            ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
            QButtonGroup *buttonGroup = ui->numberedListButton->menu()->findChild<QButtonGroup*>();
            buttonGroup->button(1)->setChecked(false);
            buttonGroup->button(2)->setChecked(false);
            buttonGroup->button(3)->setChecked(false);
            buttonGroup->button(4)->setChecked(true);
            buttonGroup->button(5)->setChecked(false);
            ui->currentNumberedListButton->setChecked(true);
            ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
            ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
            break;
        }
        case QTextListFormat::ListUpperAlpha: {
            ui->currentBulletedListButton->setChecked(false);
            ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
            ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
            QButtonGroup *buttonGroup = ui->numberedListButton->menu()->findChild<QButtonGroup*>();
            buttonGroup->button(1)->setChecked(false);
            buttonGroup->button(2)->setChecked(false);
            buttonGroup->button(3)->setChecked(false);
            buttonGroup->button(4)->setChecked(false);
            buttonGroup->button(5)->setChecked(true);
            ui->currentNumberedListButton->setChecked(true);
            ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
            ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
            break;
        }

        default:
            break;
        }
    } else {
        ui->currentBulletedListButton->setChecked(false);
        ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
        ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
        ui->currentNumberedListButton->setChecked(false);
        ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
        ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
    }
}

void MainWindow::textBold()
{
    QTextCharFormat format;
    format.setFontWeight(ui->boldButton->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnSelection(format);
}

void MainWindow::textItalic()
{
    QTextCharFormat format;
    format.setFontItalic(ui->italicButton->isChecked());
    mergeFormatOnSelection(format);
}

void MainWindow::textUnderline()
{
    QTextCharFormat format;
    format.setFontUnderline(ui->underlineButton->isChecked());
    mergeFormatOnSelection(format);
}

void MainWindow::textStrikethrough()
{
    QTextCharFormat format;
    format.setFontStrikeOut(ui->strikethroughButton->isChecked());
    mergeFormatOnSelection(format);
}

void MainWindow::textSubscript()
{
    if (ui->superscriptButton->isChecked()) {
        ui->superscriptButton->setChecked(false);
    }
    QTextCharFormat format;
    format.setVerticalAlignment(ui->subscriptButton->isChecked() ? QTextCharFormat::AlignSubScript : QTextCharFormat::AlignNormal);
    mergeFormatOnSelection(format);
}

void MainWindow::textSuperscript()
{
    if (ui->subscriptButton->isChecked()) {
        ui->subscriptButton->setChecked(false);
    }
    QTextCharFormat format;
    format.setVerticalAlignment(ui->superscriptButton->isChecked() ? QTextCharFormat::AlignSuperScript : QTextCharFormat::AlignNormal);
    mergeFormatOnSelection(format);
}

void MainWindow::textFontFamily(const QFont &font)
{
    QTextCharFormat format;
    format.setFont(font);
    mergeFormatOnSelection(format);
}

void MainWindow::textFontSize(int index)
{
    QTextCharFormat format;
    format.setFontPointSize(ui->fontSizeComboBox->itemText(index).toInt());
    mergeFormatOnSelection(format);
}

void MainWindow::onColorButton()
{
    ColorButton *button = qobject_cast<ColorButton*>(sender());
    PopupMenu *menu = qobject_cast<PopupMenu*>(button->parent()->parent());
    menu->hide();
    acceptColor(button->getColor());
    changeCurrentColorButton(button->getColor());
}

void MainWindow::onColorBGButton()
{
    ColorButton *button = qobject_cast<ColorButton*>(sender());
    PopupMenu *menu = qobject_cast<PopupMenu*>(button->parent()->parent());
    menu->hide();
    acceptBGColor(button->getColor());
    changeCurrentBGColorButton(button->getColor());
}

void MainWindow::onFGColorButton()
{
    acceptColor(currentFGColor);
}

void MainWindow::onBGColorButton()
{
    acceptBGColor(currentBGColor);
}

void MainWindow::onBulletedList(bool checked)
{
    if (ui->currentNumberedListButton->isChecked()) {
        ui->currentNumberedListButton->setChecked(false);
        ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
        ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
    }
    if (checked) {
        ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
        ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
    } else {
        ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
        ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
    }
    list(checked, currentBulletStyle);
}

void MainWindow::onNumberedList(bool checked)
{
    if (ui->currentBulletedListButton->isChecked()) {
        ui->currentBulletedListButton->setChecked(false);
        ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
        ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
    }
    if (checked) {
        ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
        ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
    } else {
        ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); }");
        ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
        QButtonGroup *buttonGroup = ui->numberedListButton->menu()->findChild<QButtonGroup*>();
        for (auto *button : buttonGroup->buttons()) {
            if (button->isChecked()) {
                button->setChecked(false);
            }
        }
    }
    list(checked, currentNumberedStyle);
}

void MainWindow::onCreate()
{
    MainWindow *newWindow = new MainWindow();
    newWindow->show();
}

void MainWindow::onOpen()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Открыть файл", QString(), "HTML Файл (*.html);;Текстовый файл (*.txt)");
    if (filePath.isEmpty()) {
        return;
    }
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл");
        return;
    }
    QTextStream stream(&file);
    if (filePath.contains(".html")) {
        ui->textEdit->setHtml(stream.readAll());
    } else if (filePath.contains(".txt")) {
        ui->textEdit->setPlainText(stream.readAll());
    }
    ui->fileName->setText(filePath);
}

void MainWindow::onSave()
{
    if (ui->fileName->text() == "Новый документ") {
        onSaveAs();
        return;
    }
    QFile file(ui->fileName->text());
    if (!file.open(QIODevice::WriteOnly | QIODevice::ExistingOnly)) {
        onSaveAs();
        return;
    }
    QTextStream stream(&file);
    if (ui->fileName->text().contains(".txt")) {
        stream << ui->textEdit->toPlainText();
    } else {
        stream << ui->textEdit->toHtml();
    }
    file.close();
}

void MainWindow::onSaveAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Сохранить файл", QString(), "HTML Файл (*.html)");
    if (filePath.isEmpty()) {
        return;
    }
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить файл");
        return;
    }
    QTextStream stream(&file);
    stream << ui->textEdit->toHtml();
    file.close();
    ui->fileName->setText(filePath);
}

void MainWindow::connectButtons()
{
    // Кнопки окна приложения
    connect(ui->buttonClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->buttonMinimize, SIGNAL(clicked()), this, SLOT(showMinimized()));
    connect(ui->buttonMaximize, SIGNAL(clicked()), this, SLOT(onButtonMaximize()));

    connect(ui->boldButton, SIGNAL(clicked()), this, SLOT(textBold()));
    connect(ui->italicButton, SIGNAL(clicked()), this, SLOT(textItalic()));
    connect(ui->underlineButton, SIGNAL(clicked()), this, SLOT(textUnderline()));
    connect(ui->strikethroughButton, SIGNAL(clicked()), this, SLOT(textStrikethrough()));
    connect(ui->subscriptButton, SIGNAL(clicked()), this, SLOT(textSubscript()));
    connect(ui->superscriptButton, SIGNAL(clicked()), this, SLOT(textSuperscript()));

    connect(ui->currentBulletedListButton, SIGNAL(clicked(bool)), this, SLOT(onBulletedList(bool)));
    connect(ui->currentNumberedListButton, SIGNAL(clicked(bool)), this, SLOT(onNumberedList(bool)));

    connect(ui->currentTextColorButton, SIGNAL(clicked()), this, SLOT(onFGColorButton()));
    connect(ui->currentTextBGColorButton, SIGNAL(clicked()), this, SLOT(onBGColorButton()));
}

void MainWindow::connectMenuActions()
{
    connect(ui->create, SIGNAL(triggered()), this, SLOT(onCreate()));
    connect(ui->open, SIGNAL(triggered()), this, SLOT(onOpen()));
    connect(ui->save, SIGNAL(triggered()), this, SLOT(onSave()));
    connect(ui->saveAs, SIGNAL(triggered()), this, SLOT(onSaveAs()));
    connect(ui->copy, SIGNAL(triggered()), ui->textEdit, SLOT(copy()));
    connect(ui->cut, SIGNAL(triggered()), ui->textEdit, SLOT(cut()));
    connect(ui->paste, SIGNAL(triggered()), ui->textEdit, SLOT(paste()));
    connect(ui->undo, SIGNAL(triggered()), ui->textEdit, SLOT(undo()));
    connect(ui->redo, SIGNAL(triggered()), ui->textEdit, SLOT(redo()));
}

void MainWindow::setupMenuBar()
{
    ui->titleLayout->addWidget(ui->menuBar, 1, 2);

    ui->fileMenu->setWindowFlag(Qt::FramelessWindowHint);
    ui->fileMenu->setAttribute(Qt::WA_TranslucentBackground);
    ui->fileMenu->setWindowFlag(Qt::NoDropShadowWindowHint);

    ui->saveAs->setShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_S));

    ui->editMenu->setWindowFlag(Qt::FramelessWindowHint);
    ui->editMenu->setAttribute(Qt::WA_TranslucentBackground);
    ui->editMenu->setWindowFlag(Qt::NoDropShadowWindowHint);
}

void MainWindow::setupToolBar()
{
    ui->fontComboBox->view()->window()->setWindowFlag(Qt::FramelessWindowHint);
    ui->fontComboBox->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
    ui->fontComboBox->view()->window()->setWindowFlag(Qt::NoDropShadowWindowHint);
    connect(ui->fontComboBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(textFontFamily(QFont)));

    ui->fontSizeComboBox->view()->window()->setWindowFlag(Qt::FramelessWindowHint);
    ui->fontSizeComboBox->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
    ui->fontSizeComboBox->view()->window()->setWindowFlag(Qt::NoDropShadowWindowHint);
    connect(ui->fontSizeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(textFontSize(int)));

    createTextColorWidget();
    createTextBGColorWidget();
    createBulletedListWidget();
    createNumberedListWidget();
    setupAlignButtons();
}

void MainWindow::createTextColorWidget()
{
    PopupMenu *menu = new PopupMenu(ui->textColorButton);
    menu->setWindowFlag(Qt::FramelessWindowHint);
    menu->setAttribute(Qt::WA_TranslucentBackground);
    menu->setWindowFlag(Qt::NoDropShadowWindowHint);
    connect(menu, &QMenu::aboutToShow, this, [=]() {
        QPushButton *button = menu->parentWidget()->parentWidget()->findChild<QPushButton*>("currentTextColorButton");
        button->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; } QLabel { background-color: transparent;}");
        menu->parentWidget()->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
    });
    connect(menu, &QMenu::aboutToHide, this, [=]() {
        QPushButton *button = menu->parentWidget()->parentWidget()->findChild<QPushButton*>("currentTextColorButton");
        button->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); } QLabel { background-color: transparent; }");
        menu->parentWidget()->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
    });

    // Окно с базовыми цветами
    QFrame *basicColors = new QFrame(menu);

    QHBoxLayout *layout = new QHBoxLayout();

    ColorButton *darkRed = new ColorButton(QColor(192, 0, 0), basicColors);
    darkRed->setStyleSheet("background-color: rgb(192, 0, 0);");

    ColorButton *red = new ColorButton(QColor(Qt::red), basicColors);
    red->setStyleSheet("background-color: red;");

    ColorButton *orange = new ColorButton(QColor(255, 192, 0), basicColors);
    orange->setStyleSheet("background-color: rgb(255, 192, 0);");

    ColorButton *yellow = new ColorButton(QColor(Qt::yellow), basicColors);
    yellow->setStyleSheet("background-color: yellow;");

    ColorButton *lightGreen = new ColorButton(QColor(146, 208, 80), basicColors);
    lightGreen->setStyleSheet("background-color: rgb(146, 208, 80);");

    ColorButton *darkGreen = new ColorButton(QColor(0, 176, 80), basicColors);
    darkGreen->setStyleSheet("background-color: rgb(0, 176, 80);");

    ColorButton *lightBlue = new ColorButton(QColor(0, 176, 240), basicColors);
    lightBlue->setStyleSheet("background-color: rgb(0, 176, 240);");

    ColorButton *blue = new ColorButton(QColor(0, 112, 192), basicColors);
    blue->setStyleSheet("background-color: rgb(0, 112, 192);");

    ColorButton *darkBlue = new ColorButton(QColor(0, 32, 96), basicColors);
    darkBlue->setStyleSheet("background-color: rgb(0, 32, 96);");

    ColorButton *violet = new ColorButton(QColor(112, 48, 160), basicColors);
    violet->setStyleSheet("background-color: rgb(112, 48, 160);");

    connect(darkRed, SIGNAL(clicked()), this, SLOT(onColorButton()));
    connect(red, SIGNAL(clicked()), this, SLOT(onColorButton()));
    connect(orange, SIGNAL(clicked()), this, SLOT(onColorButton()));
    connect(yellow, SIGNAL(clicked()), this, SLOT(onColorButton()));
    connect(lightGreen, SIGNAL(clicked()), this, SLOT(onColorButton()));
    connect(darkGreen, SIGNAL(clicked()), this, SLOT(onColorButton()));
    connect(lightBlue, SIGNAL(clicked()), this, SLOT(onColorButton()));
    connect(blue, SIGNAL(clicked()), this, SLOT(onColorButton()));
    connect(darkBlue, SIGNAL(clicked()), this, SLOT(onColorButton()));
    connect(violet, SIGNAL(clicked()), this, SLOT(onColorButton()));

    layout->addWidget(darkRed);
    layout->addWidget(red);
    layout->addWidget(orange);
    layout->addWidget(yellow);
    layout->addWidget(lightGreen);
    layout->addWidget(darkGreen);
    layout->addWidget(lightBlue);
    layout->addWidget(blue);
    layout->addWidget(darkBlue);
    layout->addWidget(violet);

    basicColors->setLayout(layout);

    QWidgetAction *basicColorsAction = new QWidgetAction(menu);
    basicColorsAction->setDefaultWidget(basicColors);

    QAction *moreColorsAction = new QAction("Другие цвета...");
    moreColorsAction->setIcon(QIcon(":/icons/palette_black.svg"));
    connect(moreColorsAction, &QAction::triggered, this, [=]() {
        QColor color = QColorDialog::getColor(Qt::black, this, "Выберите цвет");
        acceptColor(color);
        changeCurrentColorButton(color);
    });

    QAction *removeColorAction = new QAction("Сбросить цвет");
    removeColorAction->setIcon(QIcon(":/icons/format_color_reset_black.svg"));
    connect(removeColorAction, &QAction::triggered, this, [=]() {
        acceptColor(Qt::black);
        changeCurrentColorButton(Qt::black);
    });

    menu->addAction(removeColorAction);
    menu->addSeparator();
    menu->addAction(basicColorsAction);
    menu->addSeparator();
    menu->addAction(moreColorsAction);

    ui->textColorButton->setMenu(menu);

    QVBoxLayout *buttonLayout = new QVBoxLayout(ui->currentTextColorButton);
    buttonLayout->setSpacing(0);
    buttonLayout->setContentsMargins(0, 0, 0, 4);

    QPixmap pm(16, 3);
    pm.fill(Qt::transparent);

    QPainter painter(&pm);

    painter.setPen(Qt::black);

    QPainterPath path;

    path.moveTo(1, 0);
    path.lineTo(14, 0);
    path.moveTo(0, 1);
    path.lineTo(15, 1);
    path.moveTo(1, 2);
    path.lineTo(14, 2);

    painter.drawPath(path);

    QLabel *label1 = new QLabel(ui->currentTextColorButton), *label2 = new QLabel(ui->currentTextColorButton);
    label1->setAlignment(Qt::AlignCenter);
    label2->setAlignment(Qt::AlignCenter);

    label1->setText("A");
    label2->setPixmap(pm);

    buttonLayout->addWidget(label1);
    buttonLayout->addWidget(label2);
}

void MainWindow::createTextBGColorWidget()
{
    PopupMenu *menu = new PopupMenu(ui->textBGColorButton);
    menu->setWindowFlag(Qt::FramelessWindowHint);
    menu->setAttribute(Qt::WA_TranslucentBackground);
    menu->setWindowFlag(Qt::NoDropShadowWindowHint);
    connect(menu, &QMenu::aboutToShow, this, [=]() {
        QPushButton *button = menu->parentWidget()->parentWidget()->findChild<QPushButton*>("currentTextBGColorButton");
        button->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; } QLabel { background-color: transparent;}");
        menu->parentWidget()->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
    });
    connect(menu, &QMenu::aboutToHide, this, [=]() {
        QPushButton *button = menu->parentWidget()->parentWidget()->findChild<QPushButton*>("currentTextBGColorButton");
        button->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); } QLabel { background-color: transparent; }");
        menu->parentWidget()->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
    });

    // Окно с базовыми цветами
    QFrame *basicColors = new QFrame(menu);

    QHBoxLayout *layout = new QHBoxLayout();

    ColorButton *red = new ColorButton(QColor(Qt::red), basicColors);
    red->setStyleSheet("background-color: red;");

    ColorButton *yellow = new ColorButton(QColor(Qt::yellow), basicColors);
    yellow->setStyleSheet("background-color: yellow;");

    ColorButton *green = new ColorButton(QColor(0, 255, 0), basicColors);
    green->setStyleSheet("background-color: rgb(0, 255, 0);");

    ColorButton *cyan = new ColorButton(QColor(Qt::cyan), basicColors);
    cyan->setStyleSheet("background-color: cyan;");

    ColorButton *lilac = new ColorButton(QColor(255, 0, 255), basicColors);
    lilac->setStyleSheet("background-color: rgb(255, 0, 255);");

    connect(red, SIGNAL(clicked()), this, SLOT(onColorBGButton()));
    connect(yellow, SIGNAL(clicked()), this, SLOT(onColorBGButton()));
    connect(green, SIGNAL(clicked()), this, SLOT(onColorBGButton()));
    connect(cyan, SIGNAL(clicked()), this, SLOT(onColorBGButton()));
    connect(lilac, SIGNAL(clicked()), this, SLOT(onColorBGButton()));

    layout->addWidget(red);
    layout->addWidget(yellow);
    layout->addWidget(green);
    layout->addWidget(cyan);
    layout->addWidget(lilac);

    basicColors->setLayout(layout);

    QWidgetAction *basicColorsAction = new QWidgetAction(menu);
    basicColorsAction->setDefaultWidget(basicColors);

    QAction *moreColorsAction = new QAction("Другие цвета...");
    moreColorsAction->setIcon(QIcon(":/icons/palette_black.svg"));
    connect(moreColorsAction, &QAction::triggered, this, [=]() {
        QColor color = QColorDialog::getColor(Qt::black, this, "Выберите цвет");
        qDebug() << color.isValid();
        acceptBGColor(color);
        changeCurrentBGColorButton(color);
    });

    QAction *removeColorAction = new QAction("Сбросить цвет");
    removeColorAction->setIcon(QIcon(":/icons/format_color_reset_black.svg"));
    connect(removeColorAction, &QAction::triggered, this, [=]() {
        acceptBGColor(Qt::transparent);
        changeCurrentBGColorButton(Qt::transparent);
    });

    menu->addAction(removeColorAction);
    menu->addSeparator();
    menu->addAction(basicColorsAction);
    menu->addSeparator();
    menu->addAction(moreColorsAction);

    ui->textBGColorButton->setMenu(menu);

    QVBoxLayout *buttonLayout = new QVBoxLayout(ui->currentTextBGColorButton);
    buttonLayout->setSpacing(0);
    buttonLayout->setContentsMargins(0, 3, 0, 4);

    QPixmap pm1(":/icons/color_fill.svg"), pm2(16, 3);
    pm2.fill(Qt::transparent);

    QPainter painter(&pm2);

    painter.setPen(Qt::yellow);

    QPainterPath path;

    path.moveTo(1, 0);
    path.lineTo(14, 0);
    path.moveTo(0, 1);
    path.lineTo(15, 1);
    path.moveTo(1, 2);
    path.lineTo(14, 2);

    painter.drawPath(path);

    QLabel *label1 = new QLabel(ui->currentTextBGColorButton), *label2 = new QLabel(ui->currentTextBGColorButton);

    label1->setAlignment(Qt::AlignCenter);
    label2->setAlignment(Qt::AlignCenter);

    label1->setPixmap(pm1);
    label2->setPixmap(pm2);

    label1->setFixedWidth(22);
    label1->setScaledContents(true);
    label1->setContentsMargins(10, 0, 0, 0);

    buttonLayout->addWidget(label1);
    buttonLayout->addWidget(label2);
}

void MainWindow::createBulletedListWidget()
{
    PopupMenu *menu = new PopupMenu(ui->bulletedListButton);
    menu->setWindowFlag(Qt::FramelessWindowHint);
    menu->setAttribute(Qt::WA_TranslucentBackground);
    menu->setWindowFlag(Qt::NoDropShadowWindowHint);
    connect(menu, &QMenu::aboutToShow, this, [=]() {
        ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; } QLabel { background-color: transparent;}");
        ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
    });
    connect(menu, &QMenu::aboutToHide, this, [=]() {
        ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); } QLabel { background-color: transparent; }");
        ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
    });

    QFrame *list = new QFrame(menu);
    list->setStyleSheet("QPushButton { background-color: white; border-radius: 10px; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton:checked { background-color: rgb(225, 225, 225); }");

    QHBoxLayout *listLayout = new QHBoxLayout();

    QPushButton *disc = new QPushButton(list);
    disc->setCheckable(true);
    disc->setChecked(true);
    disc->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    disc->setMinimumSize(50, 50);
    disc->setIcon(QIcon(":/icons/filled_circle.svg"));
    disc->setIconSize(QSize(32, 32));

    QPushButton *circle = new QPushButton(list);
    circle->setCheckable(true);
    circle->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    circle->setMinimumSize(50, 50);
    circle->setIcon(QIcon(":/icons/circle.svg"));
    circle->setIconSize(QSize(32, 32));

    QPushButton *square = new QPushButton(list);
    square->setCheckable(true);
    square->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    square->setMinimumSize(50, 50);
    square->setIcon(QIcon(":/icons/square.svg"));
    square->setIconSize(QSize(32, 32));

    QButtonGroup *buttonGroup = new QButtonGroup(list);
    buttonGroup->addButton(disc, 1);
    buttonGroup->addButton(circle, 2);
    buttonGroup->addButton(square, 3);

    connect(buttonGroup, &QButtonGroup::idClicked, this, [=](int id) {
        buttonGroup->button(id)->setChecked(true);
        for (auto *button : buttonGroup->buttons()) {
            if (button->isChecked() && button->objectName() != QString::number(id)) {
                button->setChecked(false);
            }
        }

        switch (id) {
        case 1:
            currentBulletStyle = QTextListFormat::ListDisc;
            break;
        case 2:
            currentBulletStyle = QTextListFormat::ListCircle;
            break;
        case 3:
            currentBulletStyle = QTextListFormat::ListSquare;
            break;
        }
        menu->hide();
        ui->textEdit->setFocus();
        ui->currentBulletedListButton->setChecked(true);
        ui->currentBulletedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
        ui->bulletedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.beginEditBlock();
        QTextListFormat listFormat;
        if (cursor.currentList()) {
            listFormat = cursor.currentList()->format();
        }
        listFormat.setStyle(currentBulletStyle);
        cursor.createList(listFormat);
        cursor.endEditBlock();
    });

    listLayout->addWidget(disc);
    listLayout->addWidget(circle);
    listLayout->addWidget(square);

    list->setLayout(listLayout);

    QWidgetAction *bulletListAction = new QWidgetAction(menu);
    bulletListAction->setDefaultWidget(list);

    menu->addAction(bulletListAction);

    ui->bulletedListButton->setMenu(menu);
}

void MainWindow::createNumberedListWidget()
{
    PopupMenu *menu = new PopupMenu(ui->numberedListButton);
    menu->setWindowFlag(Qt::FramelessWindowHint);
    menu->setAttribute(Qt::WA_TranslucentBackground);
    menu->setWindowFlag(Qt::NoDropShadowWindowHint);
    connect(menu, &QMenu::aboutToShow, this, [=]() {
        QPushButton *button = menu->parentWidget()->parentWidget()->findChild<QPushButton*>("currentNumberedListButton");
        button->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; } QLabel { background-color: transparent;}");
        menu->parentWidget()->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
    });
    connect(menu, &QMenu::aboutToHide, this, [=]() {
        QPushButton *button = menu->parentWidget()->parentWidget()->findChild<QPushButton*>("currentNumberedListButton");
        button->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0; border-left: 1px solid rgb(150, 150, 150); border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); outline: none; } QPushButton:hover:!pressed { background-color: rgb(240, 240, 240); } QPushButton:pressed { background-color: rgb(225, 225, 225); } QLabel { background-color: transparent; }");
        menu->parentWidget()->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border: 1px solid rgb(150, 150, 150); background-color: white; outline: none; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton::menu-indicator { image: none; }");
    });

    QFrame *list = new QFrame(menu);
    list->setStyleSheet("QPushButton { background-color: white; border-radius: 10px; } QPushButton:hover { background-color: rgb(240, 240, 240); } QPushButton:checked { background-color: rgb(225, 225, 225); }");

    QGridLayout *listLayout = new QGridLayout();

    QPushButton *numbered = new QPushButton(list);
    numbered->setCheckable(true);
    numbered->setChecked(true);
    numbered->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    numbered->setMinimumSize(50, 50);
    numbered->setIcon(QIcon(":/icons/numbered_list.svg"));
    numbered->setIconSize(QSize(32, 32));

    QPushButton *lowerRoman = new QPushButton(list);
    lowerRoman->setCheckable(true);
    lowerRoman->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    lowerRoman->setMinimumSize(50, 50);
    lowerRoman->setIcon(QIcon(":/icons/lower_roman_list.svg"));
    lowerRoman->setIconSize(QSize(32, 32));

    QPushButton *upperRoman = new QPushButton(list);
    upperRoman->setCheckable(true);
    upperRoman->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    upperRoman->setMinimumSize(50, 50);
    upperRoman->setIcon(QIcon(":/icons/upper_roman_list.svg"));
    upperRoman->setIconSize(QSize(32, 32));

    QPushButton *lowerAlpha = new QPushButton(list);
    lowerAlpha->setCheckable(true);
    lowerAlpha->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    lowerAlpha->setMinimumSize(50, 50);
    lowerAlpha->setIcon(QIcon(":/icons/lower_alpha_list.svg"));
    lowerAlpha->setIconSize(QSize(32, 32));

    QPushButton *upperAlpha = new QPushButton(list);
    upperAlpha->setCheckable(true);
    upperAlpha->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    upperAlpha->setMinimumSize(50, 50);
    upperAlpha->setIcon(QIcon(":/icons/upper_alpha_list.svg"));
    upperAlpha->setIconSize(QSize(32, 32));

    QButtonGroup *buttonGroup = new QButtonGroup(list);
    buttonGroup->addButton(numbered, 1);
    buttonGroup->addButton(lowerRoman, 2);
    buttonGroup->addButton(upperRoman, 3);
    buttonGroup->addButton(lowerAlpha, 4);
    buttonGroup->addButton(upperAlpha, 5);

    connect(buttonGroup, &QButtonGroup::idClicked, this, [=](int id) {
        buttonGroup->button(id)->setChecked(true);
        for (auto *button : buttonGroup->buttons()) {
            if (button->isChecked() && button->objectName() != QString::number(id)) {
                button->setChecked(false);
            }
        }

        switch (id) {
        case 1:
            currentNumberedStyle = QTextListFormat::ListDecimal;
            break;
        case 2:
            currentNumberedStyle = QTextListFormat::ListLowerRoman;
            break;
        case 3:
            currentNumberedStyle = QTextListFormat::ListUpperRoman;
            break;
        case 4:
            currentNumberedStyle = QTextListFormat::ListLowerAlpha;
            break;
        case 5:
            currentNumberedStyle = QTextListFormat::ListUpperAlpha;
            break;
        }
        menu->hide();
        ui->textEdit->setFocus();
        ui->currentNumberedListButton->setChecked(true);
        ui->currentNumberedListButton->setStyleSheet("QPushButton { border-top-right-radius: 0; border-bottom-right-radius: 0;border-left: 1px solid rgb(150, 150, 150);border-right: none; border-top: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); background-color: rgb(225, 225, 225); outline: none; }");
        ui->numberedListButton->setStyleSheet("QPushButton { text-align: center; border-top-left-radius: 0; border-bottom-left-radius: 0; border-top: 1px solid rgb(150, 150, 150); border-right: 1px solid rgb(150, 150, 150); border-bottom: 1px solid rgb(150, 150, 150); border-left: none; background-color: rgb(225, 225, 225); outline: none; } QPushButton::menu-indicator { image: none; }");
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.beginEditBlock();
        QTextListFormat listFormat;
        if (cursor.currentList()) {
            listFormat = cursor.currentList()->format();
        }
        listFormat.setStyle(currentNumberedStyle);
        cursor.createList(listFormat);
        cursor.endEditBlock();
    });

    listLayout->addWidget(numbered, 0, 0);
    listLayout->addWidget(lowerRoman, 0, 1);
    listLayout->addWidget(upperRoman, 0, 2);
    listLayout->addWidget(lowerAlpha, 1, 0);
    listLayout->addWidget(upperAlpha, 1, 1);

    list->setLayout(listLayout);

    QWidgetAction *numberedListAction = new QWidgetAction(menu);
    numberedListAction->setDefaultWidget(list);

    menu->addAction(numberedListAction);

    ui->numberedListButton->setMenu(menu);
}

void MainWindow::setupAlignButtons()
{
    QButtonGroup *buttonGroup = new QButtonGroup();

    buttonGroup->addButton(ui->leftAlignButton, 1);
    buttonGroup->addButton(ui->centerAlignButton, 2);
    buttonGroup->addButton(ui->rightAlignButton, 3);
    buttonGroup->addButton(ui->justifyAlignButton, 4);

    connect(buttonGroup, &QButtonGroup::idClicked, this, [=](int id) {
        buttonGroup->button(id)->setChecked(true);
        for (auto *button : buttonGroup->buttons()) {
            if (button->isChecked() && button->objectName() != QString::number(id)) {
                button->setChecked(false);
            }
        }

        Qt::Alignment alignment;
        switch (id) {
        case 1:
            alignment = Qt::AlignLeft;
            break;
        case 2:
            alignment = Qt::AlignCenter;
            break;
        case 3:
            alignment = Qt::AlignRight;
            break;
        case 4:
            alignment = Qt::AlignJustify;
            break;
        }

        QTextBlockFormat format;
        format.setAlignment(alignment);
        QTextCursor cursor = ui->textEdit->textCursor();
        if (!cursor.hasSelection()) {
            cursor.select(QTextCursor::LineUnderCursor);
        }
        cursor.mergeBlockFormat(format);
        ui->textEdit->setFocus();
    });
}

void MainWindow::list(bool checked, QTextListFormat::Style style)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.beginEditBlock();
    if (!cursor.hasSelection()) {
        cursor.select(QTextCursor::LineUnderCursor);
    }
    if (checked) {
        QTextListFormat listFormat;
        if (cursor.currentList()) {
            listFormat = cursor.currentList()->format();
        }
        listFormat.setStyle(style);
        cursor.createList(listFormat);
    } else {
        QTextBlockFormat format;
        cursor.setBlockFormat(format);
    }

    cursor.endEditBlock();
    ui->textEdit->setFocus();
}

void MainWindow::acceptColor(QColor color)
{
    QTextCharFormat format;
    if (color.isValid()) {
        format.setForeground(color);
        currentFGColor = color;
    }
    mergeFormatOnSelection(format);
}

void MainWindow::changeCurrentColorButton(QColor color)
{
    foreach (QLabel *label, ui->currentTextColorButton->findChildren<QLabel*>()) {
        if (!label->pixmap().isNull()) {
            if (color.isValid()) {
                QPixmap pm(16, 3);
                pm.fill(Qt::transparent);

                QPainter painter(&pm);
                painter.setPen(color);

                QPainterPath path;

                path.moveTo(1, 0);
                path.lineTo(14, 0);
                path.moveTo(0, 1);
                path.lineTo(15, 1);
                path.moveTo(1, 2);
                path.lineTo(14, 2);

                painter.drawPath(path);

                label->setPixmap(pm);
            }
        }
    }
}

void MainWindow::acceptBGColor(QColor color)
{
    QTextCharFormat format;
    if (color.isValid()) {
        format.setBackground(color);
        currentBGColor = color;
    }
    mergeFormatOnSelection(format);
}

void MainWindow::changeCurrentBGColorButton(QColor color)
{
    foreach (QLabel *label, ui->currentTextBGColorButton->findChildren<QLabel*>()) {
        if (!label->pixmap().isNull() && label->pixmap().height() == 3) {
            if (color.isValid()) {
                QPixmap pm(16, 3);
                pm.fill(Qt::transparent);

                QPainter painter(&pm);

                if (color != Qt::transparent) {
                    painter.setPen(color);

                    QPainterPath path;

                    path.moveTo(1, 0);
                    path.lineTo(14, 0);
                    path.moveTo(0, 1);
                    path.lineTo(15, 1);
                    path.moveTo(1, 2);
                    path.lineTo(14, 2);

                    painter.drawPath(path);
                } else {
                    painter.setPen(Qt::black);

                    QPainterPath path;

                    path.moveTo(1, 0);
                    path.lineTo(14, 0);
                    path.lineTo(15, 1);
                    path.lineTo(14, 2);
                    path.lineTo(1, 2);
                    path.lineTo(0, 1);
                    path.lineTo(1, 0);

                    painter.drawPath(path);
                }

                label->setPixmap(pm);
            }
        }
    }
}

void MainWindow::mergeFormatOnSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.mergeCharFormat(format);
    ui->textEdit->mergeCurrentCharFormat(format);
    ui->textEdit->setFocus();
}

