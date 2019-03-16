#include "file_codes.h"
#include "jsoncoder.h"

#include <QString>
#include <QVariant>
#include <QVariantMap>
QString File_Codes::read(int status)
{
    QFile F(CODES_FILE);
    F.open(QIODevice::ReadWrite);
    return Jsoncoder::deocde(F.readAll()).value(QString::number(status)).toString();
}
