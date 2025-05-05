#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>

#include "gui/launcher.h"
#include "gui/wrapper/MainWindowWrapper.h"

int launch(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindowWrapper w;
    w.show();

    // QWidget window;
    // window.setWindowTitle("Meine Qt-GUI");

    // QVBoxLayout *layout = new QVBoxLayout();

    // QPushButton *button = new QPushButton("Close");
    // layout->addWidget(button);
    // QObject::connect(button, &QPushButton::clicked, &window, &QWidget::close);

    // window.setLayout(layout);
    // window.resize(300, 100);
    // window.show();

    return app.exec();
}
