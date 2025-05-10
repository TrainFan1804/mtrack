#include <QWidget>

#include "external/json.hpp"

#include "gui/wrapper/AddTopLevelWrapper.h"

AddTopLevelWrapper::AddTopLevelWrapper(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::AddTopLevel)
{
    ui->setupUi(this);

    connect(
        ui->add_btn, 
        &QPushButton::clicked, 
        this, 
        &AddTopLevelWrapper::addBtnClicked
    );
}

AddTopLevelWrapper::~AddTopLevelWrapper()
{
    delete ui;
}

void AddTopLevelWrapper::addBtnClicked()
{
    // create new media from entry data
    nlohmann::json json_media;

    json_media["id"] = -1;
    json_media["rating"] = ui->rating_box->value();
    json_media["name"] = ui->name_edit->text().toStdString();
    json_media["state"] = ui->state_box->currentText().toStdString();
    json_media["type"] = ui->type_edit->text().toStdString();
    
    QMedia new_media(json_media);
    emit submitAddContent(new_media);
    this->close();
}
