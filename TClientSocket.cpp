#include "TClientSocket.h"
#include "TServer.h"

TClientSocket::TClientSocket(QObject *parent, int socketId)
{
	server = (TServer*)parent;	
	client.setSocketDescriptor(socketId);
	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
	
	connect(&client, SIGNAL(readyRead()), this, SLOT(onRead()));
	connect(&client, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
}

void TClientSocket::run()
{	
	exec();
}

void TClientSocket::onRead()
{
	QString command = client.readLine();

	/* Da mettere nel costruttore. */
	if(command[0] == 'u')
	{
		if(uid.isEmpty())
		{
			uid = server->db->addUser(client.peerAddress());
			client.write(QString("\r\nYour ip is: " + client.peerAddress().toString() + "\r\nYou uid is: " + uid + "\r\n").toAscii());
		}
		else
		{
			client.write("\r\nYou already have a uid.\r\n");
		}
	}
	else if(command[0] == 'a')
	{
		server->db->addFile(uid, "abc560", "Hyttulo.avi", "38976501943672", "0");
		client.write("\r\nFile added.\r\n");
	}
	else if(command[0] == 'f')
	{
		server->db->addFile(uid, "a3d101", "t3x.jpg", "45679", "1");
		client.write("\r\nFile added.\r\n");
	}
	else if(command[0] == 'd')
	{
		server->db->deleteFile(uid, "abc560");
		client.write("\r\nFile deleted.\r\n");
	}
	else if(command[0] == 'c')
	{
		server->db->completeFile(uid, "abc560");
		client.write("\r\nFile completed..\r\n");
	}
	else if(command[0] == 's')
	{
		server->db->searchFile("%ttul%");
		client.write("\r\nNot implemented yet.\r\n");
	}
	else if(command[0] == 'l')
	{
		server->db->getSources(uid, "abc560");
		client.write("\r\nNot implemented yet.\r\n");
	}
	else
	{
		client.write("\r\nInvalid command.\r\n");
	}
}

void TClientSocket::onDisconnect()
{
	server->db->deleteUser(uid);
	quit();
}
