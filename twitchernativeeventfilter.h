#ifndef TWITCHERNATIVEEVENTFILTER_H
#define TWITCHERNATIVEEVENTFILTER_H

#include <QObject>
#include <QAbstractNativeEventFilter>
#include <windows.h>
#include <QVector>


class TwitcherNativeEventFilter : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    TwitcherNativeEventFilter();
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);





signals:
void hotkeyPressed();

};

#endif // TWITCHERNATIVEEVENTFILTER_H
