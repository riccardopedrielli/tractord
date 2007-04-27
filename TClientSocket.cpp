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
	QString command = readLine();

	/* Da mettere nel costruttore. */
	if(command[0] == 'u')
	{
		if(uid.isEmpty())
		{
			uid = thread->server->db->addUser(peerAddress());
			write(QString("\r\nYour ip is: " + peerAddress().toString() + "\r\nYou uid is: " + uid + "\r\n").toAscii());
		}
		else
		{
			write("\r\nYou already have a uid.\r\n");
		}
	}
	else if(command[0] == 'a')
	{
		thread->server->db->addFile(uid, "abc560", "Hyttulo.avi", "38976501943672", "0");
		write("\r\nFile added.\r\n");
	}
	else if(command[0] == 'f')
	{
		thread->server->db->addFile(uid, "a3d101", "t3x.jpg", "45679", "1");
		write("\r\nFile added.\r\n");
	}
	else if(command[0] == 'd')
	{
		thread->server->db->deleteFile(uid, "abc560");
		write("\r\nFile deleted.\r\n");
	}
	else if(command[0] == 'c')
	{
		thread->server->db->completeFile(uid, "abc560");
		write("\r\nFile completed..\r\n");
	}
	else if(command[0] == 's')
	{
		thread->server->db->searchFile("%ttul%");
		write("\r\nNot implemented yet.\r\n");
	}
	else if(command[0] == 'l')
	{
		thread->server->db->getSources(uid, "abc560");
		write("\r\nNot implemented yet.\r\n");
	}
	else
	{
		write("\r\nInvalid command.\r\n");
	}
}

void TClientSocket::onDisconnect()
{
	thread->server->db->deleteUser(uid);
	thread->quit();
}
