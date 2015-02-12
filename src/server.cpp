#include "server.h"
#include <QCoreApplication>

Server::Server(Light *light, Ruler *ruler, QObject *parent)
    : QTcpServer(parent),
    mLight(light),
    mRuler(ruler)
{
    connect(this, &QTcpServer::newConnection, this, &Server::onNewConnection);
    connect(ruler, &Ruler::isClose, this, &Server::transmit);
    connect(ruler, &Ruler::isClose, light, &Light::onClose);
}

Server::~Server()
{
    mSocket->close();
}

void Server::start()
{
    printf("Server started\n");
    listen(QHostAddress::Any, 5001);
    mLight->start();
    mRuler->start();
}

void Server::transmit(int distance)
{
    if (mSocket->isOpen())
    {
        mSocket->write(QString("Hello World\n").toUtf8());
        mSocket->flush();
        QCoreApplication::processEvents();
    }
    else
        fprintf(stderr, "Socket not open\n");
}

void Server::onNewConnection()
{
    mSocket = nextPendingConnection();
    printf("New connection: %s\n", qPrintable(mSocket->peerAddress().toString()));
}

void Server::onReadyRead()
{
}
