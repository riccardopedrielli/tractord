#include "TParser.h"

/*** BEGIN COMMON METHODS ***/

/* Split the commands whenever they arrive concatenated. */
QStringList TParser::splitCommands(QString commands)
{
	QStringList list;
	list=commands.split("\";", QString::SkipEmptyParts);
	for(int i=0; i<list.count(); i++)
		list[i].append("\"");
	return list;
}

/* Read the command keyword by extracting the first word from the string. */
QString TParser::getCommand(QString command)
{
	QString keyword;
	for(int i=0; command.at(i)!=' '; i++)
		keyword+=command.at(i);
	return keyword;
}

/*** END COMMON METHODS ***/


/*** BEGIN SERVER METHODS ***/

/* Split the arguments of the ADDFILE command received from the client. */
bool TParser::splitAddFile(QString command, QString &fid, QString &name, QString &dim, QString &complete)
{
	QStringList list;
	list=command.split(" \"");
	if(list.count()!=5)
		return false;
	fid=list[1];
	fid.chop(1);
	name=list[2];
	name.replace("/\"/","\"");
	name.chop(1);
	dim=list[3];
	dim.chop(1);
	complete=list[4];	
	complete.chop(1);
	return true;
}

/* Split the arguments of the DELFILE command received from the client. */
bool TParser::splitDelFile(QString command, QString &fid)
{
	QStringList list;
	list=command.split(" \"");
	if(list.count()!=2)
		return false;
	fid=list[1];
	fid.chop(1);
	return true;
}

/* Split the arguments of the COMPLETE command received from the client. */
bool TParser::splitComplete(QString command, QString &fid)
{
	QStringList list;
	list=command.split(" \"");
	if(list.count()!=2)
		return false;
	fid=list[1];
	fid.chop(1);
	return true;
}

/* Split the arguments of the PORT command received from the client. */
bool TParser::splitPort(QString command, QString &port)
{
	QStringList list;
	list=command.split(" \"");
	if(list.count()!=2)
		return false;
	port=list[1];
	port.chop(1);
	return true;
}

/* Compose the MSG command for sending it to the client. */
QString TParser::sendMsg(QString message)
{
	return "MSG \"" + message + "\";";
}

/* Split the arguments of the FIND command received from the client. */
bool TParser::splitFind(QString command, QString &name)
{
	QStringList list;
	list=command.split(" \"");
	if(list.count()!=2)
		return false;
	name=list[1];
	name.replace("/\"/","\""); //Reaplace /"/ with "
	name.chop(1);
	return true;
}

/* Compose the FILE command for sending it to the client. */
QString TParser::sendFile(QString fid, QString name, QString dim, QString sources, QString completes, QString sid)
{
	name.replace(QString("\""), QString("/\"/")); //Reaplace " with /"/
 	return "FILE \"" + fid + "\" \"" + name + "\" \"" + dim + "\" \"" + sources + "\" \"" + completes + "\" \"" + sid + "\";";
}

/* Split the arguments of the GETIP command received from the client. */
bool TParser::splitGetIp(QString command, QString &fid)
{
	QStringList list;
	list=command.split(" \"");
	if(list.count()!=2)
		return false;
	fid=list[1];
	fid.chop(1);
	return true;
}

/* Compose the IP command for sending it to the client. */
QString TParser::sendIp(QString fid, QString ip, QString port)
{	
	return "IP \"" + fid + "\" \"" + ip + "\" \"" + port + "\";";
}

/*** END SERVER METHODS ***/


/*** BEGIN CLIENT METHODS ***/

/* Compose the ADDFILE command for sending it to the server. */
QString TParser::addFile(QString fid, QString name, quint64 dim, int complete)
{
	name.replace(QString("\""), QString("/\"/")); //Reaplace " with /"/
	return "ADDFILE \"" + fid + "\" \"" + name + "\" \"" + QString::number(dim) + "\" \"" + QString::number(complete) + "\";";
}

/* Compose the DELFILE command for sending it to the server. */
QString TParser::delFile(QString fid)
{
	return "DELFILE \"" + fid + "\";";
}

/* Compose the COMPLETE command for sending it to the server. */
QString TParser::complete(QString fid)
{
	return "COMPLETE \"" + fid + "\";";
}

/* Compose the PORT command for sending it to the server. */
QString TParser::port(quint16 port)
{
	return "PORT \"" + QString::number(port) + "\";";
}

/* Split the arguments of the MSG command received from the server. */
bool TParser::splitSendMsg(QString command, QString message)
{
	QStringList list;
	list=command.split(" \"");
	if(list.count()!=2)
		return false;
	message=list[1];
	message.chop(1);
	return true;
}

/* Compose the FIND command for sending it to the server. */
QString TParser::find(QString name, quint64 sid)
{
	name.replace(QString("\""), QString("/\"/")); //Reaplace " with /"/
	return "FIND \"" + name + "\" \"" + QString::number(sid) + "\";";
}

/* Split the arguments of the FILE command received from the server. */
bool TParser::splitSendFile(QString command, QString &fid, QString &name, QString &dim, QString &sources, QString &completes, QString &sid)
{
	QStringList list;
	list=command.split(" \"");
	if(list.count()!=7)
		return false;
	fid=list[1];
	fid.chop(1);
	name=list[2];
	name.replace("/\"/","\""); //Reaplace /"/ with "
	name.chop(1);
	dim=list[3];
	dim.chop(1);
	sources=list[4];
	sources.chop(1);
	completes=list[5];	
	completes.chop(1);
	sid=list[6];
	sid.chop(1);
	return true;
}

/* Compose the GETIP command for sending it to the server. */
QString TParser::getIp(QString fid)
{	
	return "GETIP \"" + fid + "\";";
}

/* Split the arguments of the IP command received from the server. */
bool TParser::splitSendIp(QString command, QString &fid, QString &ip, QString &port)
{
	QStringList list;
	list=command.split(" \"");
	if(list.count()!=4)
		return false;
	fid=list[1];
	fid.chop(1);
	ip=list[2];
	ip.chop(1);
	port=list[3];
	port.chop(1);
	return true;
}

/* Compose the GETFILE command for sending it to another client. */
QString TParser::getFile(QString fid, quint64 byte)
{
	return "GETFILE \"" + fid + "\" \"" + QString::number(byte) + "\";";
}

/* Split the arguments of the GETFILE command received from another client. */
bool TParser::splitGetFile(QString command, QString &fid, QString &byte)
{
	QStringList list;
	list=command.split(" \"");
	if(list.count()!=3)
		return false;
	fid=list[1];
	fid.chop(1);
	byte=list[2];
	byte.chop(1);
	return true;
}

/*** END CLIENT METHODS ***/
