#include <QtCore>
#include <QString>
#include <TParser.h>
#include <QList>
#include <QStringList>

//Funzione addFile (Client)
QString TParser::addFile(QString name, quint64 dim, QString hash, quint64 complete)
{
	QString str;
	name.replace(QString("\""), QString("/\"/"));	//Controllo Per Le "" dove ci sono sostitusce /"/
	str = "ADDFILE \""+name+"\" \""+QString::number(dim, 10)+"\" \""+hash+"\" \""+QString::number(complete, 10)+"\";";	//Crea la Stringa
	return str;
}
//Funzione find	(Client)
QString TParser::find(QString file)
{
	QString str;
	file.replace(QString("\""), QString("/\"/"));	//Controllo Per Le "" dove ci sono sostitusce /"/
	str="FIND \""+file+"\";";	//Crea la Stringa
	return str;
}
//Funzioni Send	(Server)
QString TParser::sendFile(QString name, QString dim, QString hash, QString complete,QString sources)
{
	name.replace(QString("\""), QString("/\"/"));	//Controllo Per Le "" dove ci sono sostitusce /"/ 	
 	return "FILE \""+name+"\" \""+dim+"\" \""+hash+"\" \""+complete+"\" \""+sources+"\";";
}

//Funzione GETIP	(Client)
QString TParser::getIp(QString hash)
{	
	return "GETIP \""+hash+"\";";
}

//Funzione sendIp	(Server)
QString TParser::sendIp(QString hash,QString ip,QString port)
{	
	return "IP \""+hash+"\" \""+ip+"\" \""+port+"\";";
}

//Funzione getFile (Client)
QString TParser::getFile(QString hash,QString byte)
{
	return "GETFILE \""+hash+"\" \""+byte+"\";";
}

//Funzione splitCommands 
QStringList TParser::splitCommands(QString file)
{
	QStringList lista;
	lista=file.split("\";", QString::SkipEmptyParts);
	for(int i=0;i<lista.count();i++)
		lista[i].append("\"");
	return lista;
}

//Funzione slpitAddFile 
void TParser::splitAddFile (QString file, QString &name,QString &dim,QString &hash,QString &complete)
{
	QStringList temp;
	temp=file.split(" \"");
	name=temp[1];
	name.replace("/\"/","\"");
	name.chop(1);
	dim=temp[2];
	dim.chop(1);
	hash=temp[3];
	hash.chop(1);
	complete=temp[4];	
	complete.chop(2);
}

//Funzione splitSendFile 
void TParser::splitSendFile (QString file, QString &name,QString &dim,QString &hash,QString &complete,QString &sources)
{
	QStringList temp;
	temp=file.split(" \"");
	name=temp[1];
	name.replace("/\"/","\"");
	name.chop(1);
	dim=temp[2];
	dim.chop(1);
	hash=temp[3];
	hash.chop(1);
	complete=temp[4];	
	complete.chop(1);
	sources=temp[5];
	sources.chop(2);
}

//Funzione splitFind 
void TParser::splitFind (QString file, QString &name)
{
	QStringList temp;
	temp=file.split(" \"");
	name=temp[1];
	name.replace("/\"/","\"");
	name.chop(2);
}

//Funzione splitGetIp 
void TParser::splitGetIp (QString file, QString &hash)
{
	QStringList temp;
	temp=file.split(" \"");
	hash=temp[1];
	hash.chop(2);
}

//Funzione splitSendIp
void TParser::splitSendIp(QString file, QString &hash,QString &ip,QString &port)
{
	QStringList temp;
	temp=file.split(" \"");
	hash=temp[1];
	hash.chop(1);
	ip=temp[2];
	ip.chop(1);
	port=temp[3];
	port.chop(2);
}

//Funzione spiltGetFile
void TParser::spiltGetFile(QString file,QString &hash,QString &byte)
{
	QStringList temp;
	temp=file.split(" \"");
	hash=temp[1];
	hash.chop(1);
	byte=temp[2];
	byte.chop(2);
}

//Funzione getCommand
QString TParser::getCommand(QString command)
{
	QString str;
	for(int i=0;command.at(i)!=' ';i++)
		str+=command.at(i);
	return str;
}