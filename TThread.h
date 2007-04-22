#ifndef TTHREAD_H
#define TTHREAD_H

#include <QtCore>
#include "TServer.h"

class TThread : public QThread
{
	Q_OBJECT

private:
	int socketDescriptor;

protected:
    void run();

public:
	TServer *server;
	TThread(QObject *parent, int socketId);
};

#endif //TTHREAD_H
