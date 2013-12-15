#include <QCoreApplication>
#include "teststate.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TestState t;
    t.start();

    return a.exec();
}
