
//relayer
#ifndef __LINUX_DRIVER_H_
#define __LINUX_DRIVER_H_
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include <sys/ioctl.h>
#endif

#define RELAYER_IOC_MAGIC 'b'
#define RELAYER_DEVICENAME "/dev/relay"
int relayer_fd=-1;

typedef enum{
    CH1=1,
    CH2=2,
    CH3=3,
    CH4=4
}RELAYER_CHANNEL;

int OpenRelayer();    //打开继电器，在界面加载时调用
void CloseRelayer(); //关闭继电器，在界面关闭时调用
void Single_On(RELAYER_CHANNEL ch); //打开单个通道，单击打开通道按钮时调用
void Single_Off(RELAYER_CHANNEL ch); //关闭单个通道，单击关闭通道按钮时调用

void All_On();  //打开所有通道，单击打开所有通道按钮时调用
void All_Off(); //关闭所有通道，单击关闭所有通道按钮时调用