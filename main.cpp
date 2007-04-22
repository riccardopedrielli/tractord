#include <QCoreApplication>
#include "TServer.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	TServer server;

	return a.exec();
}
