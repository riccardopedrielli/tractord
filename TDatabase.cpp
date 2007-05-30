#include "md5.h"
#include "TDatabase.h"

TDatabase::TDatabase()
{
	/* Check the settings.
	   If don't exist they are added.  */
	QSettings settings(QSettings::IniFormat, QSettings::SystemScope, "MyProSoft/TractorServer", "config");
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
	
	/* Wipe the database in case it was previously crashed. */
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

	/* Ottengo l'elenco dei files appartenenti all'utente. */
	QSqlQuery getFiles("SELECT fid FROM sources WHERE uid='" + uid + "'");

	/* Elimino i files. */
	while(getFiles.next())
	{
		deleteFile(uid, getFiles.value(0).toString());
	}
}

void TDatabase::addFile(QString uid, QString fid, QString name, QString dim, QString complete)
{
	/* Controllo se il file esiste. */
	QSqlQuery checkFile("SELECT COUNT(*) FROM files WHERE fid='" + fid + "'");
	if(checkFile.value(0).toInt() == 0)
	{
		/* Se non esiste lo aggiungo. */
		QSqlQuery addFid("INSERT INTO files VALUES ('" + fid + "', '" + dim + "')");
	}

	/* Controllo se il nome del file esiste. */
	QSqlQuery checkName("SELECT COUNT(*) FROM names WHERE fid='" + fid + "' AND name='" + name + "'");
	if(checkName.value(0).toInt() == 0)
	{
		/* Se non esiste lo aggiungo. */
		QSqlQuery addName("INSERT INTO names VALUES ('" + fid + "', '" + name + "')");
	}

	/* Aggiungo la fonte del file. */
	QSqlQuery addSource("INSERT INTO sources VALUES ('" + uid + "', '" + fid + "', '" + complete + "')");
}

void TDatabase::deleteFile(QString uid, QString fid)
{
	/* Elimino la fonte. */
	QSqlQuery deleteSource("DELETE FROM sources WHERE uid='" + uid + "' AND fid='" + fid + "'");

	/* Verifico che ci siano altre fonti. */
	QSqlQuery checkSources("SELECT COUNT(*) FROM sources WHERE fid='" + fid + "'");
	if(checkSources.value(0).toInt() == 0)
	{
		/* Se non ci sono altre fonti elimino il file e tutti i suoi nomi. */
		QSqlQuery deleteFid("DELETE FROM files WHERE fid='" + fid +"'");
		QSqlQuery deleteName("DELETE FROM names WHERE fid='" + fid +"'");
	}
}

void TDatabase::completeFile(QString uid, QString fid)
{
	/* Imposto a 1 (true) la propriet… 'complete' della fonte. */
	QSqlQuery setComplete("UPDATE sources SET complete=1 WHERE uid='" + uid + "' AND fid='" + fid + "'");
}

void TDatabase::setPort(QString uid, QString port)
{
	/* Setto la porta del client. */
	QSqlQuery setPort("UPDATE users SET port='" + port + "' WHERE uid='" + uid + "'");
}

QStringList TDatabase::searchFile(QString name)
{
	/* Ottengo tutte le informazioni relative ai files corrispondenti alla stringa cercata. */
	QSqlQuery getList("SELECT names.name, names.fid, files.dim, sources.complete FROM names, files, sources WHERE name LIKE '" + name + "' AND names.fid=files.fid AND names.fid=sources.fid");
	QStringList list; //TODO: fill the list with query result.
	return list;
}

QStringList TDatabase::getSources(QString uid, QString fid)
{
	/* Ottengo la lista delle fonti del file. */
	QSqlQuery getList("SELECT ip, port FROM users, sources WHERE fid='" + fid + "' AND sources.uid<>'" + uid + "'");
	QStringList list; //TODO: fill the list with query result.
	return list;
}
