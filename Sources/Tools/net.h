#ifndef __NET_H_
#define __NET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

typedef enum {
    ETH0 = 0, 
    ETH1 = 1, 
    PPPD = 2
}Interface;

typedef enum{
    China_Mobile = 0, 
    China_Unicom = 1, 
    China_Telecom = 2
}APN;

//设置网口
int Set_Ethernet(Interface ch , char *ip, char *mask, char *gateway);
//设置4G
int Set_Wlan_Net(APN ch);
//关闭所有网口
void Close_Network(Interface ch);
//检查usb是否挂载成功
int USBCheck();
//检查SDcard是否挂载成功
int SDcardCheck();
//息屏
int BackLight_Dark();
//点亮屏幕
int BackLight_Light();

//判断以太网状态
int EthernetCheck(Interface ch);
//判断4G状态
int Dtu4GCheck();


#ifdef __cplusplus
}
#endif

#endif
