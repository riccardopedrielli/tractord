#ifndef TCLIENTSOCKET_H
#define TCLIENTSOCKET_H

#include <QtNetwork>
#include "TServer.h"

class TClientSocket : public QThread
{
	Q_OBJECT

private:	
	QTcpSocket socket;
	QString uid;

protected:
    void run();

public:
	TServer *server;
	TClientSocket(QObject *parent, int socketid);

private slots:
	void onRead();
	void onDisconnect();
};

#endif //TCLIENTSOCKET_H
