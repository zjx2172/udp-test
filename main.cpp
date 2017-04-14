#include <QCoreApplication>
#include "udp-test.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    UDPTest mainapp;

    return a.exec();
}
