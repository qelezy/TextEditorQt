#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractItemView>
#include <QColorDialog>
#include <QWidgetAction>
#include <QPainter>
#include <QPainterPath>
#include <QTextCharFormat>
#include <QButtonGroup>
#include <QTextList>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow* getUI();
    bool getMaximizedFlag();
    void setMaximizedFlag(bool flag);

private slots:
    void onButtonMaximize();
    void onCursorPositionChange();
    void onTextChanged();
    void textBold();
    void textItalic();
    void textUnderline();
    void textStrikethrough();
    void textSubscript();
    void textSuperscript();
    void textFontFamily(const QFont &font);
    void textFontSize(int index);
    void onColorButton();
    void onColorBGButton();
    void onFGColorButton();
    void onBGColorButton();
    void onBulletedList(bool checked);
    void onNumberedList(bool checked);

    void onCreate();
    void onOpen();
    void onSave();
    void onSaveAs();

private:
    Ui::MainWindow *ui;
    bool isMaximized;
    QTextListFormat::Style currentBulletStyle, currentNumberedStyle;
    QColor currentFGColor, currentBGColor;
    QRegularExpression re;

    void connectButtons();
    void connectMenuActions();
    void setupMenuBar();
    void setupToolBar();
    void createTextColorWidget();
    void createTextBGColorWidget();
    void createBulletedListWidget();
    void createNumberedListWidget();
    void setupAlignButtons();

    void list(bool checked, QTextListFormat::Style style);
    void acceptColor(QColor color);
    void changeCurrentColorButton(QColor color);
    void acceptBGColor(QColor color);
    void changeCurrentBGColorButton(QColor color);
    void mergeFormatOnSelection(const QTextCharFormat &format);
};
#endif // MAINWINDOW_H
