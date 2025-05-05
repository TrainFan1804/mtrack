#include <QApplication>
#include <QWidget>

#include "gui/wrapper/MainWindowWrapper.h"

MainWindowWrapper::MainWindowWrapper(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindowWrapper::~MainWindowWrapper()
{
    delete ui;
}
