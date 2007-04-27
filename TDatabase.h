#ifndef TDATABASE_H
#define TDATABASE_H

#include <QtSql>
#include <QtNetwork>

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
	void deleteFile(QString fid, QString uid);
	void completeFile(QString uid, QString fid);
	QStringList searchFile(QString name);
	QStringList getSources(QString uid, QString fid); 
};

#endif //TDATABASE_H
