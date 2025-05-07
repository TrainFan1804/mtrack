#include <iostream>

#include <QApplication>
#include <QWidget>

#include "gui/wrapper/MainWindowWrapper.h"
#include "gui/wrapper/AddTopLevelWrapper.h"
#include "databasemanager.h"

MainWindowWrapper::MainWindowWrapper(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // fetch data from database
    auto data_json = selectAllJsonQuery();
    QList<QMedia> view_data;
    for (auto json_obj : data_json)
    {
        QMedia media(json_obj);
        view_data << media;
    }
    // put data in MediaViewModel
    _model = new MediaViewModel(this);
    _model->setDatalist(view_data);
    // put MediaViewModel into ui->mediaView
    ui->media_view->setModel(_model);

    // setup custom slots
    connect(ui->add_button, SIGNAL(clicked()), this, SLOT(addAction()));
    connect(ui->rm_button, SIGNAL(clicked()), this, SLOT(removeAction()));
    connect(ui->save_button, SIGNAL(clicked()), this, SLOT(saveAction()));

    connect(
        ui->media_view->selectionModel(),
        SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
        this,
        SLOT(handleSelectionChanged(QItemSelection))
    );
}

MainWindowWrapper::~MainWindowWrapper()
{
    delete ui;
    delete _model;
}

void MainWindowWrapper::addAction()
{
    AddTopLevelWrapper *top_level = new AddTopLevelWrapper();
    top_level->setAttribute(Qt::WA_DeleteOnClose);
    top_level->show();
}

void MainWindowWrapper::removeAction()
{
    QModelIndex rm_index = ui->media_view->currentIndex();
    int del_id = _model->removeRow(rm_index.row(), rm_index);
    // this is just temp to avoid access the database when none is selected
    if (del_id <= -1) return;
    rmMedia(del_id);
}

void MainWindowWrapper::saveAction()
{
    auto edit_name = ui->name_edit->text();
    auto edit_state = ui->state_box->currentText();
    auto edit_type = ui->type_edit->text();
    auto edit_rating = ui->rating_box->value();

    std::cout << "name: " << edit_name.toStdString() << ", state: " << edit_state.toStdString() 
        << ", type: " << edit_type.toStdString() << ", rating: " << edit_rating << "\n";
}

void MainWindowWrapper::handleSelectionChanged(const QItemSelection &selection)
{
    QModelIndex selected_index = ui->media_view->currentIndex();
    if (!selected_index.isValid()) return;
    auto list = _model->getMediaList();

    auto name = list.at(selected_index.row())._name;
    auto state = list.at(selected_index.row())._state;
    auto type = list.at(selected_index.row())._type;
    auto rating = list.at(selected_index.row())._rating;

    ui->name_edit->setText(name);
    /*
        Replace findText with replaceData but for this there need to be real
        data be added to the combobox and that is not possible through
        the designer. The combobox need to be filled at runtime to acomplish
        this.
    */
    int index = ui->state_box->findText(state);
    ui->state_box->setCurrentIndex(index);
    ui->type_edit->setText(type);
    ui->rating_box->setValue(rating);

    ui->rm_button->setEnabled(true);
    ui->save_button->setEnabled(true);
    ui->name_edit->setEnabled(true);
    ui->state_box->setEnabled(true);
    ui->type_edit->setEnabled(true);
    ui->rating_box->setEnabled(true);
}
