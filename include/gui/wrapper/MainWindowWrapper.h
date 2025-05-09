#ifndef MAINWINDOWWRAPPER_H
#define MAINWINDOWWRAPPER_H

#include <QMainWindow>

#include "gui/forms/main_widget.h"
#include "gui/wrapper/MediaViewModel.h"

class MainWindowWrapper : public QMainWindow 
{
    Q_OBJECT

public:
    MainWindowWrapper(QWidget *parent = nullptr);
    ~MainWindowWrapper();

private:
    Ui::MainWindow *ui;
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
    void handleSelectionChanged(const QItemSelection &selection);
};

#endif
