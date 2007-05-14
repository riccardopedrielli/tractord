#ifndef TTHREAD_H
#define TTHREAD_H

#include <QtCore>
#include <QtNetwork>
#include "TServer.h"

class TClientSocket : public QThread
{
	Q_OBJECT

private:
	int socketDescriptor;

protected:
    void run();

public:
	TServer *server;
	TClientSocket(QObject *parent, int socketId);
};

#endif //TTHREAD_H
