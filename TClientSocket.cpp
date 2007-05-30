#include "TClientSocket.h"
#include "TServer.h"
#include "TParser.h"

TClientSocket::TClientSocket(QObject *parent, int socketid)
{
	qDebug("New connection.");
	server = (TServer *)parent;	
	socket.setSocketDescriptor(socketid);
	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));	
	connect(&socket, SIGNAL(readyRead()), this, SLOT(onRead()));
	connect(&socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
	uid = server->db->addUser(socket.peerAddress());
	socket.write(QString("Connection established. Your ip is " + socket.peerAddress().toString()).toAscii());
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
				
		if(cmdname == "ADDFILE")
		{
			
			QString fid, name, dim, complete;			
			if(TParser::splitAddFile(cmdlist[i], fid, name, dim, complete))
			{
				socket.write(QString("File added. fid=" + fid + " name=" + name + " dim=" + dim + "complete=" + complete).toAscii());
				server->db->addFile(uid, fid, name, dim, complete);	
			}
			else
			{
				socket.write("Cannot execute ADDFILE: syntax error.");
			}
		}
		else if(cmdname == "DELFILE")
		{
			QString fid;
			if(TParser::splitDelFile(cmdlist[i], fid))
				server->db->deleteFile(uid, fid);
		}
		else if(cmdname == "COMPLETE")
		{
			QString fid;
			if(TParser::splitComplete(cmdlist[i], fid))
				server->db->completeFile(uid, fid);
		}
		else if(cmdname == "PORT")
		{
			QString port;
			if(TParser::splitPort(cmdlist[i], port))
				server->db->setPort(uid, port);
		}
		else if(cmdname == "FIND")
		{
			QStringList files;
			QString name;
			if(TParser::splitFind(cmdlist[i], name))
				files = server->db->searchFile(name);
		}
		else if(cmdname == "GETIP")
		{
			QStringList sources;
			QString fid;
			if(TParser::splitGetIp(cmdlist[i], fid))
				sources = server->db->getSources(uid, fid);
		}		
	}
}

void TClientSocket::onDisconnect()
{
	server->db->deleteUser(uid);
	quit();
}
