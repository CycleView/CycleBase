#include <QCoreApplication>
#include "server.h"
#include "ruler.h"
#include "light.h"

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);

    Light light;
    Ruler ruler;

    Server server(&light, &ruler);
    server.start();

    return a.exec();
}
