#include "logger.h"

#include <QFile>

#include "generichelper.h"

Logger::Logger()
{

}

void Logger::info(QString logstring)
{
    QFile file(genericHelper::getAppDir() + QDir::separator() + genericHelper::getAppName()+".log");
    if (!file.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text))
    return;

    QTextStream out(&file);
    out << QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss") << ": " << logstring << "\n";
    out.flush();

    file.close();
}
