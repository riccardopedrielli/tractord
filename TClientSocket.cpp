#include "TClientSocket.h"
#include "TServer.h"
#include "TParser.h"
#include "info_structs.h"

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
				
		if(cmdname == "ADDFILE")
		{
			
			QString fid, name, dim, complete;			
			if(TParser::splitAddFile(cmdlist[i], fid, name, dim, complete))
			{
				server->db->addFile(uid, fid, name, dim, complete);
			}
		}
		else if(cmdname == "DELFILE")
		{
			QString fid;
			if(TParser::splitDelFile(cmdlist[i], fid))
			{
				server->db->deleteFile(uid, fid);
			}
		}
		else if(cmdname == "COMPLETE")
		{
			QString fid;
			if(TParser::splitComplete(cmdlist[i], fid)) 
			{
				server->db->completeFile(uid, fid);
			}
		}
		else if(cmdname == "PORT")
		{
			QString port;
			if(TParser::splitPort(cmdlist[i], port))
			{
				server->db->setPort(uid, port);
			}
		}
		else if(cmdname == "FIND")
		{
			QList<FileInfo> files;
			QString name, sid;
			if(TParser::splitFind(cmdlist[i], name, sid))
			{
				files = server->db->searchFile(name);
				int n = files.count();
				for(int p=0; p<n; p++)
				{
					socket.write(TParser::sendFile(sid, files.at(p).name, files.at(p).dim, files.at(p).sources, files.at(p).completes, files.at(p).fid).toAscii());
					socket.waitForBytesWritten();
				}
			}
		}
		else if(cmdname == "GETIP")
		{
			QList<FileSource> sources;
			QString fid;
			if(TParser::splitGetIp(cmdlist[i], fid))
			{
				sources = server->db->getSources(uid, fid);
				int n = sources.count();
				for(int p=0; p<n; p++)
				{
					socket.write(TParser::sendIp(fid, sources.at(p).ip, sources.at(p).port).toAscii());
					socket.waitForBytesWritten();
				}
			}
		}
	}
}

void TClientSocket::onDisconnect()
{
	server->db->deleteUser(uid);
	quit();
}
