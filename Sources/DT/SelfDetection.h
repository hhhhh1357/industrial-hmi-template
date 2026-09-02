#ifndef SELFDETECTION_H
#define SELFDETECTION_H

#include "uart.h"

#include <QString>

class SelfDetection
{
public:
    SelfDetection();

    static bool getSelfDetectedResultADC(ADS7844_CHANNEL channel, float & result );
};

#endif // SELFDETECTION_H
