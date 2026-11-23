#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    qDebug() << "MAIN STARTED\n";
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}