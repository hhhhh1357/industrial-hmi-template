#ifndef __LINUX_TIME_H_
#define __LINUX_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>

void Set_Sys_Time(char * time);

#ifdef __cplusplus
}
#endif

#endif

