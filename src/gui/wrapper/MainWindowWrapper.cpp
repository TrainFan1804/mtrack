#include <QApplication>
#include <QWidget>
#include <QAction>
#include <QScreen>
#include <QMessageBox>

#include "external/json.hpp"

#include "gui/wrapper/MainWindowWrapper.h"
#include "gui/wrapper/AddTopLevelWrapper.h"
#include "db/database_service.h"
#include "db/extractor/DumpExtractor.h"
#include "debug/debprint.h"

MainWindowWrapper::MainWindowWrapper(QWidget *parent)
    : QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    move(screen()->availableGeometry().center() - frameGeometry().center());

    // fetch data from database
    auto data_json = selectAllJsonQuery();
    QList<QMedia> view_data;
    for (auto json_obj : data_json)
    {
        QMedia media(json_obj);
        view_data << media;
    }
    // put data in MediaViewModel
    _model = new MediaViewModel(this, view_data);
    // put MediaViewModel into ui->mediaView
    _ui->media_view->setModel(_model);

    // setup custom slots
    connect(
        _ui->add_button, 
        &QPushButton::clicked, 
        this, 
        &MainWindowWrapper::openTopLevelWindow
    );

    connect(
        _ui->rm_button, 
        &QPushButton::clicked, 
        this, 
        &MainWindowWrapper::removeAction
    );
    connect(
        _ui->save_button, 
        &QPushButton::clicked, 
        this, 
        &MainWindowWrapper::saveAction
    );

    connect(
        _ui->media_view,
        &QTreeView::clicked,
        this,
        &MainWindowWrapper::handleSelectionClick
    );

    connect(
        _ui->log_status,
        &QAction::toggled,
        this,
        &MainWindowWrapper::changeLogStatusAction
    );

    connect(
        _ui->as_database_action,
        &QAction::triggered,
        this,
        &MainWindowWrapper::createDatabaseBackup
    );
}

MainWindowWrapper::~MainWindowWrapper()
{
    delete _ui;
    delete _model;
}

void MainWindowWrapper::openTopLevelWindow()
{
    AddTopLevelWrapper *top_level = new AddTopLevelWrapper();
    connect(
        top_level, 
        &AddTopLevelWrapper::submitAddContent, 
        this, 
        &MainWindowWrapper::fetchTopLevelContent
    );
    top_level->setAttribute(Qt::WA_DeleteOnClose);
    top_level->show();
}

void MainWindowWrapper::fetchTopLevelContent(const QMedia &media)
{
    int last_row = _model->rowCount();
    if (last_row < 0) return;

    int last_index_id;
    if (last_row == 0) last_index_id = 0;
    else last_index_id = _model->getMediaAt(last_row - 1).getId();

    QMedia c = media;
    c.setId(last_index_id + 1);

    _model->insertRow(c);
    auto m = c.unwrap();
    addMedia(m);
}

void MainWindowWrapper::removeAction()
{
    int rm_index = _ui->media_view->currentIndex().row();
    int del_id = _model->removeRow(rm_index);
    // this is just temp to avoid access the database when none is selected
    if (del_id <= -1) return;
    rmMedia(del_id);
}

void MainWindowWrapper::saveAction()
{
    int selected_index = _ui->media_view->currentIndex().row();
    int selected_id = _model->getMediaAt(selected_index).getId();

    auto edit_name = _ui->name_edit->text();
    auto edit_state = _ui->state_box->currentText();
    auto edit_type = _ui->type_edit->text();
    auto edit_rating = _ui->rating_box->value();

    nlohmann::json json;
    json["id"] = selected_id;
    json["name"] = edit_name.toStdString();
    json["state"] = edit_state.toStdString();
    json["type"] = edit_type.toStdString();
    json["rating"] = edit_rating;
    QMedia media(json);
    _model->editRow(selected_index, media);
    editMedia(selected_id, media.unwrap());
}

void MainWindowWrapper::handleSelectionClick(const QModelIndex &selected_index)
{
    if (!selected_index.isValid()) return;
    auto media = _model->getMediaAt(selected_index.row());

    auto name = media.name();
    auto state = media.state();
    auto type = media.type();
    auto rating = media.rating();

    _ui->name_edit->setText(name);
    /*
        Replace findText with replaceData but for this there need to be real
        data be added to the combobox and that is not possible through
        the designer. The combobox need to be filled at runtime to acomplish
        this.
    */
    int index = _ui->state_box->findText(state);
    _ui->state_box->setCurrentIndex(index);
    _ui->type_edit->setText(type);
    _ui->rating_box->setValue(rating);

    _ui->rm_button->setEnabled(true);
    _ui->save_button->setEnabled(true);
    _ui->name_edit->setEnabled(true);
    _ui->state_box->setEnabled(true);
    _ui->type_edit->setEnabled(true);
    _ui->rating_box->setEnabled(true);
}

void MainWindowWrapper::changeLogStatusAction(bool status)
{
    debug::setState(status);
}

void MainWindowWrapper::createDatabaseBackup()
{
    IDatabaseExtractor *e = new DumpExtractor();
    dumpDatabase(e);
    delete e;
    QMessageBox msg;
    msg.setWindowTitle("Backup created");
    msg.setIcon(QMessageBox::Icon::Information);
    msg.setText("Backup created successfully!");
    msg.setInformativeText("The backup is located at: <FILL WITH BACKUP_DIR_PATH>");
    msg.setStandardButtons(QMessageBox::StandardButton::Ok);
    msg.exec();
}
