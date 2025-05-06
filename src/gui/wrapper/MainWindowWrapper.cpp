#include <iostream>

#include <QApplication>
#include <QWidget>

#include "gui/wrapper/MainWindowWrapper.h"
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
    ui->mediaView->setModel(_model);

    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addAction()));
    connect(ui->rmButton, SIGNAL(clicked()), this, SLOT(removeAction()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveAction()));

    connect(
        ui->mediaView->selectionModel(),
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
    std::cout << "Need to be implemented\n";
}

void MainWindowWrapper::removeAction()
{
    std::cout << "Need to be implemented\n";
}

void MainWindowWrapper::saveAction()
{
    auto edit_name = ui->nameEdit->text();
    auto edit_state = ui->stateBox->currentText();
    auto edit_type = ui->typeEdit->text();
    auto edit_rating = ui->ratingBox->value();

    std::cout << "name: " << edit_name.toStdString() << ", state: " << edit_state.toStdString() 
        << ", type: " << edit_type.toStdString() << ", rating: " << edit_rating << "\n";
}

void MainWindowWrapper::handleSelectionChanged(const QItemSelection &selection)
{
    QModelIndex selected_index = ui->mediaView->currentIndex();
    if (!selected_index.isValid()) return;
    auto list = _model->getMediaList();

    auto name = list.at(selected_index.row())._name;
    auto state = list.at(selected_index.row())._state;
    auto type = list.at(selected_index.row())._type;
    auto rating = list.at(selected_index.row())._rating;

    ui->nameEdit->setText(name);
    /*
        Replace findText with replaceData but for this there need to be real
        data be added to the combobox and that is not possible through
        the designer. The combobox need to be filled at runtime to acomplish
        this.
    */
    int index = ui->stateBox->findText(state);
    ui->stateBox->setCurrentIndex(index);
    ui->typeEdit->setText(type);
    ui->ratingBox->setValue(rating);

    ui->rmButton->setEnabled(true);
    ui->saveButton->setEnabled(true);
    ui->nameEdit->setEnabled(true);
    ui->stateBox->setEnabled(true);
    ui->typeEdit->setEnabled(true);
    ui->ratingBox->setEnabled(true);
}
