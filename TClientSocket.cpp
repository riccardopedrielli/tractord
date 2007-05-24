#include "TClientSocket.h"
#include "TServer.h"
#include "TParser.h"

TClientSocket::TClientSocket(QObject *parent, int socketid)
{
	server = (TServer*)parent;	
	socket.setSocketDescriptor(socketid);
	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));	
	connect(&socket, SIGNAL(readyRead()), this, SLOT(onRead()));
	connect(&socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
	uid = server->db->addUser(socket.peerAddress());
}

void TClientSocket::run()
{	
	exec();
}

void TClientSocket::onRead()
{	
	QStringList cmdlist = TParser::splitCommands(socket.readLine());	

	for(int i=0; i<cmdlist.count(); i++)
	{
		QString cmdname = TParser::getCommand(cmdlist[i]);
				
		if (cmdname == "ADDFILE")
		{
			QString name,dim,hash,complete;			
			TParser::splitAddFile(cmdlist[i], name, dim, hash, complete);
			server->db->addFile(uid, hash, name, dim, complete);			
		}
		if (cmdname == "FIND")
			server->db->searchFile(TParser::splitFind(cmdlist[i]));
	}

	/* Da mettere nel costruttore. 
	if(command[0] == 'u')
	{
		if(uid.isEmpty())
		{
			uid = server->db->addUser(socket.peerAddress());
			socket.write(QString("\r\nYour ip is: " + socket.peerAddress().toString() + "\r\nYou uid is: " + uid + "\r\n").toAscii());
		}
		else
		{
			socket.write("\r\nYou already have a uid.\r\n");
		}
	}
	else if(command[0] == 'a')
	{
		server->db->addFile(uid, "abc560", "Hyttulo.avi", "38976501943672", "0");
		socket.write("\r\nFile added.\r\n");
	}
	else if(command[0] == 'f')
	{
		server->db->addFile(uid, "a3d101", "t3x.jpg", "45679", "1");
		socket.write("\r\nFile added.\r\n");
	}
	else if(command[0] == 'd')
	{
		server->db->deleteFile(uid, "abc560");
		socket.write("\r\nFile deleted.\r\n");
	}
	else if(command[0] == 'c')
	{
		server->db->completeFile(uid, "abc560");
		socket.write("\r\nFile completed..\r\n");
	}
	else if(command[0] == 's')
	{
		server->db->searchFile("%ttul%");
		socket.write("\r\nNot implemented yet.\r\n");
	}
	else if(command[0] == 'l')
	{
		server->db->getSources(uid, "abc560");
		socket.write("\r\nNot implemented yet.\r\n");
	}
	else
	{
		socket.write("\r\nInvalid command.\r\n");
	}*/
}

void TClientSocket::onDisconnect()
{
	server->db->deleteUser(uid);
	quit();
}
