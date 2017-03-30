#include <iostream>
#include <Windows/MainWindow.h>
#include <QtWidgets/QApplication>
#include <Tools/Logger.h>
#include <Tools/StableController.h>
#include <Implementation/DefaultProtocol.h>
#include <Implementation/TCPInterface.h>

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

//    FRDriver driver;
//    driver.setProtocol(std::make_shared<DefaultProtocol>());
//
//    auto tcp = std::make_shared<TCPInterface>();
//    driver.setInterface(tcp);
//
//    tcp->setAddress(IPv4Address::fromString("192.168.137.111"), 7778);
//
//    if (!driver.physicalInterface()->openConnection())
//    {
//        std::cout << "Can't connect." << std::endl;
//        return -1;
//    }
//
//    std::cout << "Tag send result: " << driver.sendTag(
//            30,
//            15000,
//            "\xef\xf0\xe8\xec\xe5\xf0"
//    );

    return 0;
}