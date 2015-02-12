#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include "light.h"
#include "ruler.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(Light *light, Ruler *ruler, QObject *parent = 0);
    ~Server();
    void start();

private slots:
    void onNewConnection();
    void transmit(int distance);
    void onReadyRead();

private:
    QTcpSocket *mSocket;
    Light *mLight;
    Ruler *mRuler;
};

#endif
