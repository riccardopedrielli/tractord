#ifndef TCLIENTSOCKET_H
#define TCLIENTSOCKET_H

#include <QtNetwork>
#include "TThread.h"

class TClientSocket : public QTcpSocket
{
	Q_OBJECT

private:
	TThread *thread;
	QString clientUid;

public:
	TClientSocket(QObject *parent);

private slots:
	void onRead();
	void onDisconnect();
};

#endif //TCLIENTSOCKET_H
