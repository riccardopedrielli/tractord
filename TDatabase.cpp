#include "md5.h"
#include "TDatabase.h"

TDatabase::TDatabase()
{
	QSettings settings(QSettings::IniFormat, QSettings::SystemScope, "MyProSoft", "Tractor Server");
	if(!settings.contains("database/driver"))
		settings.setValue("database/driver", "QMYSQL");
	if(!settings.contains("database/host"))
		settings.setValue("database/host", "lollerlandia.no-ip.org");
	if(!settings.contains("database/port"))
		settings.setValue("database/port", 3306);
	if(!settings.contains("database/user"))
		settings.setValue("database/user", "my_tractor");
	if(!settings.contains("database/password"))
		settings.setValue("database/password", "password");
	if(!settings.contains("database/dbname"))
		settings.setValue("database/dbname", "my_tractor");
	sqldb = QSqlDatabase::addDatabase(settings.value("database/driver").toString());
	sqldb.setHostName(settings.value("database/host").toString());
	sqldb.setPort(settings.value("database/port").toInt());
	sqldb.setUserName(settings.value("database/user").toString());
	sqldb.setPassword(settings.value("database/password").toString());
	sqldb.setDatabaseName(settings.value("database/dbname").toString());
	if(!sqldb.open())
	{
		qFatal(QString("Cannot open DB: " + sqldb.lastError().text()).toAscii());
		QCoreApplication::instance()->exit(1);
	}
	
	connect(this, SIGNAL(uidAssigned(uid)), this, SLOT(sendUid(uid)));
}

QString TDatabase::addUser(QHostAddress ip)
{
	QString uid;
	QSqlQuery query;
	do
	{
		uid = qtMD5(QString(ip.toString() + QTime::currentTime().toString("z")).toAscii());
		query.exec(QString("SELECT uid FROM users WHERE uid='" + uid + "'"));
		if(query.size() != 0)
			uid.clear();
	} while(uid.isEmpty());
	query.exec(QString("INSERT INTO users(uid,ip) VALUES ('" + uid + "','" + ip.toString() + "')"));
	return uid;
}

void TDatabase::deleteUser(QString uid)
{
	QSqlQuery query(QString("DELETE FROM users WHERE uid='" + uid + "'"));
}

void TDatabase::addFile(QString uid, QString fid, QString name, QString dim, QString complete)
{
	QSqlQuery query1(QString("INSERT INTO sources(uid,fid,complete) VALUES ('" + uid + "','" + fid + "','" + complete + "')"));
	//QSqlQuery query2(QString("INSERT INTO files(fid,dim,sources) VALUES ('" + fid + "','" + dim + "','" + "70" + "')"));
	//QSqlQuery query3(QString("INSERT INTO names(fid,name) VALUES ('" + fid + "','" + name + "')"));
}

void TDatabase::deleteFile(QString fid, QString uid)
{
	//QSqlQuery query1(QString("DELETE FROM names USING names,sources WHERE names.fid=sources.fid AND sources.uid='" + uid +"'"));
	//QSqlQuery query2(QString("DELETE FROM files USING files,sources WHERE files.fid=sources.fid AND sources.uid='" + uid +"'"));
	QSqlQuery query3(QString("DELETE FROM sources WHERE uid='" + uid + "'"));
}
