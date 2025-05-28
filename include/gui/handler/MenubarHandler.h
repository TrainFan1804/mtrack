#pragma once

#include <QObject>
#include <QAction>

/**
 * Wrapper class to connect menu bar actions seperate from the MainWindowWrapper
 */
class MenubarHandler : public QObject
{
    Q_OBJECT

public:
    MenubarHandler(QObject *parent = nullptr);

    void connectAction(
        QAction *status_action,
        QAction *create_backup_action,
        QAction *import_action
    );

signals:
    void backupFinished();

private slots:
    void changeLogStatusAction(bool status);
    void createDatabaseBackup();
    void importDatabaseBackup();
};
