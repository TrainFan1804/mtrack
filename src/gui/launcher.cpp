#include <QApplication>

#include "gui/launcher.h"
#include "gui/wrapper/MainWindowWrapper.h"
#include "gui/wrapper/MediaViewModel.h"

int launch(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindowWrapper w;

    w.setModel(createLatestMediaViewModel());
    w.show();
    return app.exec();
}
