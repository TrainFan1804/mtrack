#ifndef MAINWINDOWWRAPPER_H
#define MAINWINDOWWRAPPER_H

#include <QMainWindow>

#include "gui/forms/main_widget.h"

class MainWindowWrapper : public QMainWindow 
{
    Q_OBJECT

public:
    MainWindowWrapper(QWidget *parent = nullptr);
    ~MainWindowWrapper();

private:
    Ui::MainWindow *ui;
};

#endif
