#include "md5.h"
#include "TDatabase.h"

TDatabase::TDatabase()
{
	/* Check the settings.
	   If don't exist they are added.  */
	QSettings settings(QSettings::IniFormat, QSettings::SystemScope, "tractord", "settings");
	if(!settings.contains("server/name"))
		settings.setValue("server/name", "A Tractor Server");
	if(!settings.contains("server/description"))
		settings.setValue("server/description", "This server rocks!");
	if(!settings.contains("database/driver"))
		settings.setValue("database/driver", "QMYSQL");
	if(!settings.contains("database/host"))
		settings.setValue("database/host", "localhost");
	if(!settings.contains("database/port"))
		settings.setValue("database/port", 3306);
	if(!settings.contains("database/user"))
		settings.setValue("database/user", "user");
	if(!settings.contains("database/password"))
		settings.setValue("database/password", "password");
	if(!settings.contains("database/dbname"))
		settings.setValue("database/dbname", "my_tractor");

	/* Settings loading. */
	sqldb = QSqlDatabase::addDatabase(settings.value("database/driver").toString());
	sqldb.setHostName(settings.value("database/host").toString());
	sqldb.setPort(settings.value("database/port").toInt());
	sqldb.setUserName(settings.value("database/user").toString());
	sqldb.setPassword(settings.value("database/password").toString());
	sqldb.setDatabaseName(settings.value("database/dbname").toString());

	/* Try to open the database. */
	if(!sqldb.open())
	{
		/* If it fails the application terminates. */
		QCoreApplication::instance()->exit(1);
	}
	
	/* Wipe the database in case the server was previously crashed. */
	QSqlQuery query;
	query.exec("DELETE FROM files");
	query.exec("DELETE FROM names");
	query.exec("DELETE FROM sources");
	query.exec("DELETE FROM users");
}

QString TDatabase::addUser(QHostAddress ip)
{
	QString uid;

	/* Create a random uid until one free is found. */
	do
	{
		uid = qtMD5(QString(ip.toString() + QTime::currentTime().toString("z")).toAscii());
		QSqlQuery checkUid("SELECT COUNT(*) FROM users WHERE uid='" + uid + "'");
		if(checkUid.value(0).toInt() != 0)
			uid.clear();
	} while(uid.isEmpty());

	/* Insert the uid in the database. */
	QSqlQuery addUid("INSERT INTO users(uid, ip) VALUES ('" + uid + "', '" + ip.toString() + "')");
	return uid;
}

void TDatabase::deleteUser(QString uid)
{
	/* Delete the user. */
	QSqlQuery deleteUid("DELETE FROM users WHERE uid='" + uid + "'");

	/* Get the list of files that belong to the user. */
	QSqlQuery getFiles("SELECT fid FROM sources WHERE uid='" + uid + "'");

	/* Delete the files. */
	while(getFiles.next())
	{
		deleteFile(uid, getFiles.value(0).toString());
	}
}

void TDatabase::addFile(QString uid, QString fid, QString name, QString dim, QString complete)
{
	/* Che the file existance. */
	QSqlQuery checkFile("SELECT COUNT(*) FROM files WHERE fid='" + fid + "'");
	if(checkFile.value(0).toInt() == 0)
	{
		/* If not exist is added. */
		QSqlQuery addFid("INSERT INTO files VALUES ('" + fid + "', '" + dim + "')");
	}

	/* Check the name existance. */
	/* ATTENTION: This is a workaround because the original query didn't work. */
	QSqlQuery checkName("SELECT * FROM names WHERE fid='" + fid + "' AND name='" + name + "' LIMIT 0, 1");
	if(checkName.size() == 0)
	{
		/* If not exist is added. */
		QSqlQuery addName("INSERT INTO names VALUES ('" + fid + "', '" + name + "')");
	}

	/* Add the source. */
	QSqlQuery addSource("INSERT INTO sources VALUES ('" + uid + "', '" + fid + "', '" + complete + "')");
}

void TDatabase::deleteFile(QString uid, QString fid)
{
	/* Delete the source. */
	QSqlQuery deleteSource("DELETE FROM sources WHERE uid='" + uid + "' AND fid='" + fid + "'");

	/* Check the existance of other sources. */
	/* If there are no more sources, the file and all its names are deleted. */
	QSqlQuery deleteFile("DELETE FROM files WHERE fid='" + fid +"' AND (SELECT COUNT(*) AS n FROM sources WHERE fid='" + fid +"')=0");
	QSqlQuery deleteName("DELETE FROM names WHERE fid='" + fid +"' AND (SELECT COUNT(*) AS n FROM sources WHERE fid='" + fid +"')=0");
}

void TDatabase::completeFile(QString uid, QString fid)
{
	/* Set to 1 (true) the 'complete' property of the source. */
	QSqlQuery setComplete("UPDATE sources SET complete=1 WHERE uid='" + uid + "' AND fid='" + fid + "'");
}

void TDatabase::setPort(QString uid, QString port)
{
	/* Set the client port. */
	QSqlQuery setPort("UPDATE users SET port='" + port + "' WHERE uid='" + uid + "'");
}

QList<FileInfo> TDatabase::searchFile(QString name)
{
	/* Get the informations of files that correspond the searched string. */
	QSqlQuery getList("SELECT names.fid AS fileid, names.name, files.dim, (SELECT count(*) FROM sources WHERE fid=fileid) AS sources, (SELECT count(*) FROM sources WHERE fid=fileid AND complete=1) AS completes FROM names, files WHERE names.name LIKE '" + name + "' AND names.fid = files.fid");
	QList<FileInfo> list;
	FileInfo info;
	while(getList.next())
	{
		info.name=getList.value(1).toString();
		info.dim=getList.value(2).toString();
		info.sources=getList.value(3).toString();
		info.completes=getList.value(4).toString();
		info.fid=getList.value(0).toString();
		list.append(info);
	}
	return list;
}

QList<FileSource> TDatabase::getSources(QString uid, QString fid)
{
	/* Get the file sources list. */
	QSqlQuery getList("SELECT users.ip, users.port FROM users, sources WHERE sources.fid='" + fid + "' AND sources.uid=users.uid AND sources.uid<>'" + uid + "'");
	QList<FileSource> list;
	FileSource source;
	while(getList.next())
	{
		source.ip=getList.value(0).toString();
		source.port=getList.value(1).toString();
		list.append(source);
	}
	return list;
}
