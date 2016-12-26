#include <iostream>
#include <include/Windows/MainWindow.h>
#include <QtWidgets/QApplication>
#include <Tools/Logger.h>
#include <include/Tools/StableController.h>

int main(int argc, char* argv[])
{
    StableController::init(
            []()
            {
                Logger::i().waitForLogToBeWritten();
            }
    );

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}