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
    Ui::AddTopLevel *ui;

signals:
    void submitAddContent(const QMedia &Media);

private slots:
    void addBtnClicked();
};

#endif
