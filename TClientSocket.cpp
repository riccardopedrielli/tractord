#include "TClientSocket.h"
#include "TServer.h"

TClientSocket::TClientSocket(QObject *parent) : QTcpSocket(parent)
{
	thread = (TThread*)parent;
	connect(this, SIGNAL(readyRead()), this, SLOT(onRead()));
	connect(this, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
}

void TClientSocket::onRead()
{
	QStringList commands;
	commands.append(readLine());

	if(commands[0] == "u")
	{
		clientUid = thread->server->db->addUser(peerAddress());
		write(QString("\r\nYour ip is: " + peerAddress().toString() + "\r\nYou uid is: " + clientUid + "\r\n").toAscii());
	}
	else if(commands[0] == "a")
	{
		thread->server->db->addFile(clientUid, "abc560", "Hyttulo.avi", "38976501943672", "1");
	}
	else if(commands[0] == "d")
	{
		thread->server->db->deleteFile("abc560", clientUid);
	}
	else
	{
		write("\r\nInvalid command.\r\n");
	}
}

void TClientSocket::onDisconnect()
{
	thread->server->db->deleteUser(clientUid);
	thread->quit();
}
