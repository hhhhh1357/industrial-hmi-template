//key
#ifndef __KEY_H_
#define __KEY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include <sys/ioctl.h>

#define KEY_IOC_MAGIC 'c'

#define KEY_DEVICENAME "/dev/key777"

typedef enum {
    KEY1=1,
    KEY2=2,
    KEY3=3,
    KEY4=4
}KEY;

int OpenKey();
int Single_Press(KEY key);
void All_Press();
void CloseKey();

#ifdef __cplusplus
}
#endif

#endif
