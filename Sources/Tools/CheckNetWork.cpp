#include "CheckNetWork.h"

bool CheckNetWork(char * hostOrIp){
    bool ret = true;
    PingResult pingResult;
    Ping ping = Ping();
    for (int count = 1; count <= 4; count++) {
        ret = ret && ping.ping(hostOrIp, 1, pingResult);
    }
    return ret;
}
