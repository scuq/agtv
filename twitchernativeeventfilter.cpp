#include "twitchernativeeventfilter.h"
#include <QDebug>
TwitcherNativeEventFilter::TwitcherNativeEventFilter()
{
}

bool TwitcherNativeEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result){

    Q_UNUSED(result);
    Q_UNUSED(eventType);

    MSG* msg = reinterpret_cast<MSG*>(message);


    switch(msg->message)
       {
           case WM_HOTKEY:
           if(msg->wParam == 1)
           {
               //qDebug() << "Hotkey pressed!!!";


               emit hotkeyPressed(  );
               break;
           }
       }
   return false;




}
