#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>

#include "gui/launcher.h"

int launch(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Meine Qt-GUI");

    QVBoxLayout *layout = new QVBoxLayout();

    QPushButton *button = new QPushButton("Klick mich!");
    layout->addWidget(button);

    QObject::connect(button, &QPushButton::clicked, []() {
        std::cout << "Button wurde geklickt!" << std::endl;
    });

    window.setLayout(layout);
    window.resize(300, 100);
    window.show();

    return app.exec();
}
