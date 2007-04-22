#include "TServer.h"
#include "TClientSocket.h"
#include "TThread.h"
#include <QFile>

TServer::TServer(QObject *parent) : QTcpServer(parent)
{
	listen(QHostAddress::Any, 2070);
	db = new TDatabase;
}

void TServer::incomingConnection(int socketId)
{	
	TThread *thread = new TThread(this, socketId);	
	thread->start();
}
