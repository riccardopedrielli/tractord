#include "TThread.h"
#include "TClientSocket.h"

TThread::TThread(QObject *parent, int socketId)
{
	server = (TServer*)parent;
	socketDescriptor = socketId;
	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void TThread::run()
{
	TClientSocket client(this);
	client.setSocketDescriptor(socketDescriptor);
	exec();
}
