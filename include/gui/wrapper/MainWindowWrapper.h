#ifndef MAINWINDOWWRAPPER_H
#define MAINWINDOWWRAPPER_H

#include <QMainWindow>

#include "gui/forms/main_window.h"
#include "gui/wrapper/MediaViewModel.h"

class MainWindowWrapper : public QMainWindow 
{
    Q_OBJECT

public:
    MainWindowWrapper(QWidget *parent = nullptr);
    ~MainWindowWrapper();

private:
    Ui::MainWindow *_ui;
    MediaViewModel *_model;

private slots:
    void openTopLevelWindow();
    /**
     * Is used to fetch the data that is send from {@link AddTopLevelWrapper}
     * to handle process of adding a new media in the database / TreeViewModel.
     */
    void fetchTopLevelContent(const QMedia &media);

    void removeAction();
    void saveAction();
    void handleSelectionClick(const QModelIndex &selected_index);
    
    /**
     * To update the model after a import the media model is replaced
     * by a new media model. This isn't a nice solution because the view
     * will send a new request to the DB when importing a database dump
     */
    void updateMediaModel();
};

#endif
