#include "random.h"

const QString Random::GetRandomString(const int randomStringLength)
{
    const QString possibleCharacters("abcdefghijklmnopqrstuvwxyz0123456789");

    QString randomString;
    for(int i=0; i<randomStringLength; ++i)
    {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}
