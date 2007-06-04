#ifndef INFO_STRUCTS_H
#define INFO_STRUCTS_H

#include <QString>

struct FileInfo
{
	QString name;
	QString dim;
	QString sources;
	QString completes;
	QString fid;
};

struct FileSource
{
	QString fid;
	QString ip;
	QString port;
};

#endif //INFO_STRUCTS_H
