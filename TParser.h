#ifndef TPARSER_H
#define TPARSER_H

#include <QtCore>

class TParser
{
public:
	/* Common methods */
	static QStringList splitCommands(QString commands);
	static QString getCommand(QString command);

	/* Server methods */
	static bool splitAddFile(QString command, QString &fid, QString &name, QString &dim, QString &complete);
	static bool splitDelFile(QString command, QString &fid);
	static bool splitComplete(QString command, QString &fid);
	static bool splitPort(QString command, QString &port);
	static QString sendMsg(QString message);
	static bool splitFind(QString command, QString &name, QString &sid);
	static QString sendFile(QString fid, QString name, QString dim, QString sources, QString completes, QString sid);
	static bool splitGetIp(QString command, QString &fid);
	static QString sendIp(QString fid, QString ip, QString port);

	/* Client methods */
	static QString addFile(QString fid, QString name, quint64 dim, int complete);
	static QString delFile(QString fid);
	static QString complete(QString fid);
	static QString port(quint16 port);
	static bool splitSendMsg(QString command, QString message);
	static QString find(QString name, quint64 sid);
	static bool splitSendFile(QString command, QString &fid, QString &name, QString &dim, QString &sources, QString &completes, QString &sid);
	static QString getIp(QString fid);
	static bool splitSendIp(QString command, QString &fid, QString &ip, QString &port);
	static QString getFile(QString fid, quint64 byte);
	static bool splitGetFile(QString command, QString &fid, QString &byte);
};

#endif //TPARSER_H
