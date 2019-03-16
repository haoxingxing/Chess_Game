#ifndef RANDOM_H
#define RANDOM_H

#include <QString>



class Random
{
public:
    static const QString GetRandomString(const int randomStringLength = 12);
};

#endif // RANDOM_H
