#include <QApplication>

#include "debug/debprint.h"
#include "gui/launcher.h"
#include "gui/wrapper/MainWindowWrapper.h"

int launch(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindowWrapper w;
    w.show();
    return app.exec();
}
