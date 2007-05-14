#include "TServer.h"
#include "TClientSocket.h"
#include <QFile>

TServer::TServer(QObject *parent) : QTcpServer(parent)
{
	listen(QHostAddress::Any, 2070);
	db = new TDatabase;
}

void TServer::incomingConnection(int socketId)
{	
	TClientSocket *clientsocket = new TClientSocket(this, socketId);	
	clientsocket->start();
}
