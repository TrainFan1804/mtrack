#include <QWidget>

#include "gui/wrapper/AddTopLevelWrapper.h"

AddTopLevelWrapper::AddTopLevelWrapper(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::AddTopLevel)
{
    ui->setupUi(this);
}

AddTopLevelWrapper::~AddTopLevelWrapper()
{
    delete ui;
}
