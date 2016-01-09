#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "syntaxhighlighter.h"
#include <QDialog>
#include <QLabel>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_Quit_triggered();
    void on_pushButton_released();

private:
    Ui::MainWindow *ui;
    SyntaxHighlighter* highlighter;
    QDialog *err;
    QLabel *text;
    QHBoxLayout *layout;
};

#endif // MAINWINDOW_H
