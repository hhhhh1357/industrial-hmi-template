#include "SelfDetection.h"

SelfDetection::SelfDetection()
{

}

bool SelfDetection::getSelfDetectedResultADC(ADS7844_CHANNEL channel, float &result)
{
    int err= ADS7844_Single_Read(channel, &result);
    if (err==0)
        return true;
    else
        return false;
}
