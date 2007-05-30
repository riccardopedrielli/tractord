#include "TServer.h"
#include "TClientSocket.h"

TServer::TServer(QObject *parent) : QTcpServer(parent)
{
	listen(QHostAddress::Any, 2070);
	db = new TDatabase;
}

void TServer::incomingConnection(int socketid)
{	
	TClientSocket *clientSocket = new TClientSocket(this, socketid);	
	clientSocket->start();
}
