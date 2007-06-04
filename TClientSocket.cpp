#include "TClientSocket.h"
#include "TServer.h"
#include "TParser.h"
#include "info_structs.h"
#include <QDateTime>

TClientSocket::TClientSocket(QObject *parent, int socketid)
{
	qDebug("New connection.");
	server = (TServer *)parent;	
	socket.setSocketDescriptor(socketid);
	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));	
	connect(&socket, SIGNAL(readyRead()), this, SLOT(onRead()));
	connect(&socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
	uid = server->db->addUser(socket.peerAddress());
	QDateTime time(QDateTime::currentDateTime());
	socket.write(QString("[" + time.toString("hh:mm:ss.zzz") + "] Connection established. Your ip is " + socket.peerAddress().toString()).toAscii());
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
				QDateTime startTime(QDateTime::currentDateTime());
				socket.write(QString("[" + startTime.toString("hh:mm:ss.zzz") + "] Executing ADDFILE with the following args: fid = " + fid + " | name = " + name + " | dim = " + dim + " | complete = " + complete).toAscii());
				server->db->addFile(uid, fid, name, dim, complete);
				QDateTime endTime(QDateTime::currentDateTime());
				socket.write(QString("[" + endTime.toString("hh:mm:ss.zzz") + "] ADDFILE executed successfully.").toAscii());
			}
			else
			{
				QDateTime time(QDateTime::currentDateTime());
				socket.write(QString("[" + time.toString("hh:mm:ss.zzz") + "] Cannot execute ADDFILE: wrong number of arguments.").toAscii());
			}	
		}
		else if(cmdname == "DELFILE")
		{
			QString fid;
			if(TParser::splitDelFile(cmdlist[i], fid))
			{
				QDateTime startTime(QDateTime::currentDateTime());
				socket.write(QString("[" + startTime.toString("hh:mm:ss.zzz") + "] Executing DELFILE with the following args: fid=" + fid).toAscii());
				server->db->deleteFile(uid, fid);
				QDateTime endTime(QDateTime::currentDateTime());
				socket.write(QString("[" + endTime.toString("hh:mm:ss.zzz") + "] DELFILE executed successfully.").toAscii());
			}
			else
			{
				QDateTime time(QDateTime::currentDateTime());
				socket.write(QString("[" + time.toString("hh:mm:ss.zzz") + "] Cannot execute DELFILE: wrong number of arguments.").toAscii());
			}
		}
		else if(cmdname == "COMPLETE")
		{
			QString fid;
			if(TParser::splitComplete(cmdlist[i], fid)) 
			{
				QDateTime startTime(QDateTime::currentDateTime());
				socket.write(QString("[" + startTime.toString("hh:mm:ss.zzz") + "] Executing COMPLETE with the following args: fid=" + fid).toAscii());
				server->db->completeFile(uid, fid);
				QDateTime endTime(QDateTime::currentDateTime());
				socket.write(QString("[" + endTime.toString("hh:mm:ss.zzz") + "] COMPLETE executed successfully.").toAscii());
			}
			else
			{
				QDateTime time(QDateTime::currentDateTime());
				socket.write(QString("[" + time.toString("hh:mm:ss.zzz") + "] Cannot execute COMPLETE: wrong number of arguments.").toAscii());
			}
		}
		else if(cmdname == "PORT")
		{
			QString port;
			if(TParser::splitPort(cmdlist[i], port))
			{
				QDateTime startTime(QDateTime::currentDateTime());
				socket.write(QString("[" + startTime.toString("hh:mm:ss.zzz") + "] Executing PORT with the following args: port=" + port).toAscii());
				server->db->setPort(uid, port);
				QDateTime endTime(QDateTime::currentDateTime());
				socket.write(QString("[" + endTime.toString("hh:mm:ss.zzz") + "] PORT executed successfully.").toAscii());
			}
			else
			{
				QDateTime time(QDateTime::currentDateTime());
				socket.write(QString("[" + time.toString("hh:mm:ss.zzz") + "] Cannot execute PORT: wrong number of arguments.").toAscii());
			}
		}
		else if(cmdname == "FIND")
		{
			QList<FileInfo> files;
			QString name, sid;
			if(TParser::splitFind(cmdlist[i], name, sid))
			{
				QDateTime startTime(QDateTime::currentDateTime());
				socket.write(QString("[" + startTime.toString("hh:mm:ss.zzz") + "] Executing FIND with the following args: name=" + name + " | sid=" + sid).toAscii());
				files = server->db->searchFile(name);
				QDateTime endTime(QDateTime::currentDateTime());
				socket.write(QString("[" + endTime.toString("hh:mm:ss.zzz") + "] FIND executed successfully.").toAscii());
				int n = files.count();
				for(int p=0; p<n; p++)
				{
					socket.write(TParser::sendFile(sid, files.at(p).name, files.at(p).dim, files.at(p).sources, files.at(p).completes, files.at(p).fid).toAscii());
					socket.waitForBytesWritten();
				}
			}
			else
			{
				QDateTime time(QDateTime::currentDateTime());
				socket.write(QString("[" + time.toString("hh:mm:ss.zzz") + "] Cannot execute FIND: wrong number of arguments.").toAscii());
			}
		}
		else if(cmdname == "GETIP")
		{
			QList<FileSource> sources;
			QString fid;
			if(TParser::splitGetIp(cmdlist[i], fid))
			{
				QDateTime startTime(QDateTime::currentDateTime());
				socket.write(QString("[" + startTime.toString("hh:mm:ss.zzz") + "] Executing GETIP with the following args: fid=" + fid).toAscii());
				sources = server->db->getSources(uid, fid);
				QDateTime endTime(QDateTime::currentDateTime());
				socket.write(QString("[" + endTime.toString("hh:mm:ss.zzz") + "] GETIP executed successfully.").toAscii());
				int n = sources.count();
				for(int p=0; p<n; p++)
				{
					socket.write(TParser::sendIp(fid, sources.at(p).ip, sources.at(p).port).toAscii());
					socket.waitForBytesWritten();
				}
			}
			else
			{
				QDateTime time(QDateTime::currentDateTime());
				socket.write(QString("[" + time.toString("hh:mm:ss.zzz") + "] Cannot execute GETIP: wrong number of arguments.").toAscii());
			}
		}
		else
		{
			QDateTime time(QDateTime::currentDateTime());
			socket.write(QString("[" + time.toString("hh:mm:ss.zzz") + "] Unknown command: " + cmdname).toAscii());
		}	
	}
}

void TClientSocket::onDisconnect()
{
	server->db->deleteUser(uid);
	quit();
}
