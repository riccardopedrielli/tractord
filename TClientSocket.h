#ifndef TCLIENTSOCKET_H
#define TCLIENTSOCKET_H

#include <QtNetwork>
#include "TServer.h"

class TClientSocket : public QThread
{
	Q_OBJECT

private:	
	QTcpSocket client;
	QString uid;

protected:
    void run();

public:
	TServer *server;
	TClientSocket(QObject *parent, int socketId);

private slots:
	void onRead();
	void onDisconnect();
};

#endif //TCLIENTSOCKET_H
