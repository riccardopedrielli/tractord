#include <QString>
#include <QList>
#include <QStringList>


class TParser
{	
public:
	//FUNZIONI
	static QString addFile(QString name, quint64 dim, QString hash, quint64 complete); 
	static QString sendFile(QString name, QString dim, QString hash, QString complete,QString sources); 
	static QString find(QString file);	
	static QString getIp(QString hash);	//Restituisce hash
	static QString sendIp(QString hash,QString ip,QString port);	//Restituisce hash+ip
	static QStringList splitCommands(QString File);
	static QString getFile(QString hash,QString byte);
	static void splitAddFile(QString file, QString &name,QString &dim,QString &hash,QString &complete);
	static void splitSendFile(QString file, QString &name,QString &dim,QString &hash,QString &complete,QString &sources);
	static void splitFind(QString file, QString &name);
	static void splitGetIp(QString file, QString &hash);
	static void splitSendIp(QString file, QString &hash,QString &ip,QString &port);
	static void spiltGetFile(QString file,QString &hash,QString &byte); 
	static QString getCommand(QString command);
};