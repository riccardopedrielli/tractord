#include "md5.h"
#include "TDatabase.h"

TDatabase::TDatabase()
{
	/* Controllo esistenza impostazioni.
	   Se mancano vengono aggiunte. */
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
		
	/* Caricamento impostazioni. */
	sqldb = QSqlDatabase::addDatabase(settings.value("database/driver").toString());
	sqldb.setHostName(settings.value("database/host").toString());
	sqldb.setPort(settings.value("database/port").toInt());
	sqldb.setUserName(settings.value("database/user").toString());
	sqldb.setPassword(settings.value("database/password").toString());
	sqldb.setDatabaseName(settings.value("database/dbname").toString());
	
	/* Tenta di aprire il database. */
	if(!sqldb.open())
	{
		/* In caso di fallimento l'applicazione termina. */
		qFatal(QString("Cannot open DB: " + sqldb.lastError().text()).toAscii());
		QCoreApplication::instance()->exit(1);
	}
}

QString TDatabase::addUser(QHostAddress ip)
{
	QString uid;
	
	/* Creo uid random fino a quando non ne trovo uno libero. */
	do
	{
		uid = qtMD5(QString(ip.toString() + QTime::currentTime().toString("z")).toAscii());
		QSqlQuery checkUid("SELECT uid FROM users WHERE uid='" + uid + "'");
		if(checkUid.size() != 0)
			uid.clear();
	} while(uid.isEmpty());
	
	/* Inserisco l'uid nel database. */
	QSqlQuery addUid("INSERT INTO users(uid,ip) VALUES ('" + uid + "','" + ip.toString() + "')");
	return uid;
}

void TDatabase::deleteUser(QString uid)
{
	/* Elimino l'utente. */
	QSqlQuery deleteUid("DELETE FROM users WHERE uid='" + uid + "'");
	
	/* Ottengo l'elenco dei files appartenenti all'utente. */
	QSqlQuery getFiles("SELECT fid FROM files WHERE uid='" + uid + "'");
	
	/* Elimino i files. */
	while(getFiles.next())
	{
		deleteFile(uid, getFiles.value(0).toString());
	}
}

void TDatabase::addFile(QString uid, QString fid, QString name, QString dim, QString complete)
{
	/* Controllo se il file esiste. */
	QSqlQuery checkFile("SELECT fid FROM files WHERE fid='" + fid + "'");
	if(checkFile.size() == 0)
	{
		/* Se non esiste lo aggiungo. */
		QSqlQuery addFid("INSERT INTO files(fid,dim) VALUES ('" + fid + "','" + dim + "')");
	}
	
	/* Controllo se il nome del file esiste. */
	QSqlQuery checkName("SELECT name FROM names WHERE name='" + name + "' AND fid='" + fid + "'");
	if(checkName.size() == 0)
	{
		/* Se non esiste lo aggiungo. */
		QSqlQuery addName("INSERT INTO names(fid,name) VALUES ('" + fid + "','" + name + "')");
	}
	
	/* Aggiungo la fonte del file. */
	QSqlQuery addSource("INSERT INTO sources(uid,fid,complete) VALUES ('" + uid + "','" + fid + "','" + complete + "')");
}

void TDatabase::deleteFile(QString uid, QString fid)
{
	/* Elimino la fonte. */
	QSqlQuery deleteSource("DELETE FROM sources WHERE uid='" + uid + "'");
	
	/* Verifico che ci siano altre fonti. */
	QSqlQuery checkSources("SELECT uid FROM sources WHERE fid='" + fid + "'");
	if(checkSources.size() == 0)
	{
		/* Se non ci sono altre fonti elimino il file e tutti i suoi nomi. */
		QSqlQuery deleteFid("DELETE FROM files WHERE fid='" + fid +"'");
		QSqlQuery deleteName("DELETE FROM names WHERE fid='" + fid +"'");
	}
}

void TDatabase::completeFile(QString uid, QString fid)
{
	/* Imposto a 1 (true) la proprietů 'complete' della fonte. */
	QSqlQuery setComplete("UPDATE sources SET complete=1 WHERE uid='" + uid + "' AND fid='" + fid + "'");
}

QStringList TDatabase::searchFile(QString name)
{
	/* Ottengo tutte le informazioni relative ai files corrispondenti alla stringa cercata. */
	QSqlQuery getList("SELECT names.name,names.fid,files,dim,sources.complete FROM names,files,sources WHERE name LIKE '" + name + "' AND names.fid=files.fid AND names.fid=sources.fid");
	QStringList list; //TODO: fill the list with query result.
	return list;
}

QStringList TDatabase::getSources(QString uid, QString fid)
{
	/* Ottengo la lista delle fonti del file. */
	QSqlQuery getList("SELECT ip,port FROM users,sources WHERE fid='" + fid + "' AND sources.uid<>'" + uid + "'");
	QStringList list; //TODO: fill the list with query result.
	return list;
}
