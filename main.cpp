#include "qt/mainwindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Create main event loop handler and parse command line arguments
    QApplication app(argc, argv);

    // Create application main window & enter main event loop
    MainWindow window;
    window.show();
    return app.exec();
}
