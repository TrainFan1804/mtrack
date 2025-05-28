#ifndef ADDTOPLEVELWRAPPER_H
#define ADDTOPLEVELWRAPPER_H

#include <QWidget>

#include "gui/forms/add_top_level.h"
#include "gui/wrapper/MediaViewModel.h"

class AddTopLevelWrapper : public QWidget
{
    Q_OBJECT

public:
    AddTopLevelWrapper(QWidget *parent = nullptr);
    ~AddTopLevelWrapper();

private:
    Ui::AddTopLevel *_ui;

signals:
    /**
     * Send a media object. Is used as a signal in {@link addBtnClicked} to send
     * the data from this wrapper window to {@link MainWindowWrapper}.
     */
    void submitAddContent(const QMedia &Media);

private slots:
    void addBtnClicked();
};

#endif
