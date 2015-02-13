#include "server.h"
#include <iostream>
#include <QCoreApplication>

Server::Server(Light *light, Ruler *ruler, QObject *parent)
    : QTcpServer(parent),
    mLight(light),
    mRuler(ruler),
    mSocket(nullptr)
{
    connect(this, &QTcpServer::newConnection, this, &Server::onNewConnection);
    connect(mRuler, &Ruler::isClose, this, &Server::transmit);
    connect(mRuler, &Ruler::isClose, light, &Light::onClose);
}

Server::~Server()
{
    mSocket->close();
}

void Server::start()
{
    std::cout << "Server started" << std::endl;
    listen(QHostAddress::Any, 5001);
    mLight->start();
    mRuler->start();

    for (;;)
        QCoreApplication::processEvents();
}

void Server::transmit(int distance)
{
    if (mSocket && mSocket->isOpen())
    {
        mSocket->write(QString("Hello World\n").toUtf8());
        mSocket->flush();
        std::cout << "User warned!" << std::endl;
    }
    else
        std::cout << "User not connected!" << std::endl;
}

void Server::onNewConnection()
{
    mSocket = nextPendingConnection();
    std::cout << "New connection: " << qPrintable(mSocket->peerAddress().toString()) << std::endl;
}

void Server::onReadyRead()
{
}
