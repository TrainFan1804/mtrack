#include <QWidget>
#include <QMessageBox>
#include <QScreen>

#include "external/json.hpp"

#include "utils/str_manipulation.h"
#include "gui/wrapper/AddTopLevelWrapper.h"

AddTopLevelWrapper::AddTopLevelWrapper(QWidget *parent)
    : QWidget(parent),
    _ui(new Ui::AddTopLevel)
{
    _ui->setupUi(this);
    move(screen()->availableGeometry().center() - frameGeometry().center());

    connect(
        _ui->add_btn, 
        &QPushButton::clicked, 
        this, 
        &AddTopLevelWrapper::addBtnClicked
    );
}

AddTopLevelWrapper::~AddTopLevelWrapper()
{
    delete _ui;
}

void AddTopLevelWrapper::addBtnClicked()
{
    /*
        I needed literally almost 3 HOURS to insert the varable
        names into the json correctly...
    */
    auto name = mtrack::trim(_ui->name_edit->text().toStdString());
    auto type = mtrack::trim(_ui->type_edit->text().toStdString());
    if (name.empty() || type.empty())
    {
        QMessageBox msg;
        msg.setWindowTitle("Add warning");
        msg.setIcon(QMessageBox::Icon::Warning);
        msg.setText("Warning while adding a new media");
        msg.setInformativeText("You need to fill out all entries to add a new media!");
        msg.setStandardButtons(QMessageBox::StandardButton::Ok);
        msg.exec();
        return;
    }
    // create new media from entry data
    nlohmann::json json_media;

    json_media["id"] = -1;
    json_media["rating"] = _ui->rating_box->value();
    json_media["name"] = name;
    json_media["state"] = _ui->state_box->currentText().toStdString();
    json_media["type"] = type;
    
    QMedia new_media(json_media);
    emit submitAddContent(new_media);
    this->close();
}
