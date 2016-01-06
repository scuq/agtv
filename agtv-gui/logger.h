#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>

class Logger
{
public:
    Logger();
    void info(QString logstring);
};

#endif // LOGGER_H
