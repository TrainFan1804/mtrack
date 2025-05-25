#include <QMessageBox>
#include <QFileDialog>

#include "gui/handler/MenubarHandler.h"
#include "debug/debprint.h"
#include "db/database_service.h"
#include "db/extractor/IDatabaseExtractor.h"
#include "buildenv.h"

MenubarHandler::MenubarHandler(QObject *parent)
    : QObject(parent)
{

}

void MenubarHandler::connectAction(
    QAction *status_action,
    QAction *create_backup_action,
    QAction *import_action
)
{
    connect(
        status_action,
        &QAction::toggled,
        this,
        &MenubarHandler::changeLogStatusAction
    );

    connect(
        create_backup_action,
        &QAction::triggered,
        this,
        &MenubarHandler::createDatabaseBackup
    );

    connect(
        import_action,
        &QAction::triggered,
        this,
        &MenubarHandler::importDatabaseBackup
    );
}

void MenubarHandler::changeLogStatusAction(bool status)
{
    debug::setState(status);
}

void MenubarHandler::createDatabaseBackup()
{
    auto e = createExtractor("sql");
    dumpDatabase(e.get());

    QMessageBox msg;
    msg.setWindowTitle("Backup created");
    msg.setIcon(QMessageBox::Icon::Information);
    msg.setText("Backup created successfully!");
    QString inf_text = "The backup is located in: " 
        + QString::fromUtf8(BACKUP_DIR_PATH);
    msg.setInformativeText(inf_text);
    msg.setStandardButtons(QMessageBox::StandardButton::Ok);
    msg.exec();
}

void MenubarHandler::importDatabaseBackup()
{
    // is this cast ugly? Yes. Do I care? No.
    auto file_name = QFileDialog::getOpenFileName(
        qobject_cast<QWidget*>(parent()),
        "Open Document", 
        BACKUP_DIR_PATH,
        "Backup files (*.dump *.sql);;All files (*.*)", 
        0, 
        QFileDialog::DontUseNativeDialog
    ).toStdString();

    if (!file_name.empty())
    {
        auto i = createImporter("sql");
        importDatabase(i.get(), file_name);
        
        emit backupFinished();

        QMessageBox msg;
        msg.setWindowTitle("Backup imported");
        msg.setIcon(QMessageBox::Icon::Information);
        msg.setText("Backup successfully imported!");
        msg.setStandardButtons(QMessageBox::StandardButton::Ok);
        msg.exec();
    }
}
