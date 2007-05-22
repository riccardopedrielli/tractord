#ifndef TSERVER_H
#define TSERVER_H

#include <QtNetwork>
#include "TDatabase.h"

class TServer : public QTcpServer
{
	Q_OBJECT

private:
	void incomingConnection(int socketid);

public:
	TServer(QObject *parent = 0);
	TDatabase *db;
};

#endif //TSERVER_H
