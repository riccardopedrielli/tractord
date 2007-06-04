#ifndef TDATABASE_H
#define TDATABASE_H

#include <QtSql>
#include <QtNetwork>
#include "info_structs.h"

class TDatabase : public QObject
{
	Q_OBJECT

private:
	QSqlDatabase sqldb;

public:
	TDatabase();
	QString addUser(QHostAddress ip);
	void deleteUser(QString uid);
	void addFile(QString uid, QString fid, QString name, QString dim, QString complete);
	void deleteFile(QString uid, QString fid);
	void completeFile(QString uid, QString fid);
	void setPort(QString uid, QString port);
	QList<FileInfo> searchFile(QString name);
	QList<FileSource> getSources(QString uid, QString fid); 
};

#endif //TDATABASE_H
