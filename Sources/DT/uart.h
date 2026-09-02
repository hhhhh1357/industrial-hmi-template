#ifndef UART
#define UART

#ifdef __cplusplus
extern "C" {
#endif

//uart
#include <errno.h>
#include <termios.h>

#ifndef __LINUX_DRIVER_H_
#define __LINUX_DRIVER_H_
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
//#include "log.h"


#define RELAYER_IOC_MAGIC 'b'
#define RELAYER_DEVICENAME "/dev/relay"

#define ADS_IOC_MAGIC 'a'

#define ADS7844_DEVICENAME "/dev/ads7844"
#define ADS_FACTOR 5.0/4096.0      //矫正因子


typedef struct
{
   unsigned char data[8];
} CMD_PACKET;

 typedef enum{
    BD2400=2400,
    BD4800=4800,
    BD9600=9600,
    BD19200=19200,
    BD115200=115200
}BAUD_RATE;
typedef enum {
    START_ONE=1,
    START_TWO = 2,
}START_BITS;
typedef enum{
    DATA_SEVEN=7,
    DATA_EIGHT=8
}DATA_BITS;
typedef enum{
    STOP_ONE=1,
    STOP_TWO=2
}STOP_BITS;
typedef enum{
    NONE,
    ODD,
    EVEN
}PARITY_CHECK;

typedef enum{
    PORT0=0,
    PORT1=1,
    PORT2=2,
    PORT3=3,
    PORT4=4,
    PORT5=5,
    PORT6=6,
    //AI
    PORT7=7,
    PORT8=8,
    PORT9=9,
    PORT10=10,
    PORT11=11,
    PORT12=12,
    PORT13=13,
    PORT14=14,
    //继电器
    PORT15=15
}URT_PORT;
typedef struct {
    BAUD_RATE baud;
    START_BITS startBits;
    DATA_BITS dataBits;
    STOP_BITS stopBits;
    PARITY_CHECK checkBits;
}SERIAL_PORT_CONFIG;

typedef enum{
    RS485_RX1 = 9,
    RS485_TX1 = 10,
    RS485_RX2 = 11,
    RS485_TX2 = 12,
}RS485_RX_TX;

typedef enum{
    CH1=1,
    CH2=2,
    CH3=3,
    CH4=4
}RELAYER_CHANNEL;

typedef enum{
    ADS_CH0=0,
    ADS_CH1=1,
    ADS_CH2=2,
    ADS_CH3=3,
    ADS_CH4=4,
    ADS_CH5=5,
    ADS_CH6=6,
    ADS_CH7=7
}ADS7844_CHANNEL;

int Single_Port_OpenAndConfig(URT_PORT port,SERIAL_PORT_CONFIG config);
int Single_PortConfig(URT_PORT port, SERIAL_PORT_CONFIG config);
void All_Port_OpenAndConfig(SERIAL_PORT_CONFIG configs[]);
int Single_Port_Read(URT_PORT port, char buf[], int len);
void clearbuff(URT_PORT port);

int Single_Port_Write(URT_PORT port, int len,unsigned char buf[]);
void All_Port_Read(char **buf, int *len);

void Single_Port_Close(URT_PORT port);
void All_Port_Close();
int Single_485_Read(URT_PORT port,  unsigned char buf[], int len);
int Single_485_Write(URT_PORT port, unsigned char buf[], int len, SERIAL_PORT_CONFIG config);

int rs485_tx(int *fd, int ch,  unsigned char buf[], int len, SERIAL_PORT_CONFIG config);
int rs485_rx(int *fd, int ch, unsigned char buf[], int len);

int OpenRelayer();    //打开继电器，在界面加载时调用
void CloseRelayer(); //关闭继电器，在界面关闭时调用
void Single_On(RELAYER_CHANNEL ch); //打开单个通道，单击打开通道按钮时调用
void Single_Off(RELAYER_CHANNEL ch); //关闭单个通道，单击关闭通道按钮时调用

void PLC_Time_Set(URT_PORT port, unsigned char slave_address, unsigned short reg_address, unsigned char time[],SERIAL_PORT_CONFIG config);

void All_On();  //打开所有通道，单击打开所有通道按钮时调用
void All_Off(); //关闭所有通道，单击关闭所有通道按钮时调用

u_int16_t GetCrcData(u_int8_t *data, u_int32_t len);
//打开ADS的操作一般是在界面初始化的时候执行ADS7844_Open();
int ADS7844_Open();
//关闭（界面退出或程序结束时记得关闭)
void ADS7844_Close();

//读8通道数据（该读取通道数据的操作一般就在按钮点击事件中执行，以便刷新单个通道的数据）
int ADS7844_Single_Read(ADS7844_CHANNEL ch, float *result);

//读8通道数据（读取通道数据的操作一般就在定时线程中执行）
void ADS7844_All_Read(float result[], int err[]);

//获取继电器状态
int get_RelayStatus(RELAYER_CHANNEL ch);

//

#endif

#ifdef __cplusplus
}
#endif
 
#endif
