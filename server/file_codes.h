#ifndef FILE_CODES_H
#define FILE_CODES_H
#include <QFile>
#define CODES_FILE "codes.json"
class File_Codes
{
public:
    static QString read(int status);
    static QVariantMap readmap(QString status);
    static int readint(QString status);
};

#endif // FILE_CODES_H
