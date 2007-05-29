#include "TClientSocket.h"
#include "TServer.h"
#include "TParser.h"

TClientSocket::TClientSocket(QObject *parent, int socketid)
{
	server = (TServer *)parent;	
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
			QString fid, name, dim, complete;			
			TParser::splitAddFile(cmdlist[i], fid, name, dim, complete);
			server->db->addFile(uid, fid, name, dim, complete);			
		}
		else if (cmdname == "FIND")
		{
			QString name;
			TParser::splitFind(cmdlist[i], name);
			server->db->searchFile(name);
		}
		else
		{
			
		}
	}
}

void TClientSocket::onDisconnect()
{
	server->db->deleteUser(uid);
	quit();
}
