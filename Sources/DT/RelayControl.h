#ifndef RELAYCONTROL_H
#define RELAYCONTROL_H
#include<QString>
#include<QDateTime>
#include "uart.h"
#include <qthread.h>
#include<QDebug>
#include "uart.h"
class RelayControl
{
public:

    RelayControl();
    static bool RelayCountAndAlarm(QString time,RELAYER_CHANNEL alarm_ch, RELAYER_CHANNEL count_ch);
public:
    static QString historytime;

};

#endif // RELAYCONTROL_H
