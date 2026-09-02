#include "uart.h"
#define _ADS7844FACTOR


char *UART_PORT_DEVICENAME[]={"/dev/ttymxc0","/dev/ttysWK0","/dev/ttysWK1","/dev/ttysWK2","/dev/ttysWK3","/dev/ttymxc2","/dev/ttymxc3"};

int uart_fd[7]={-1, -1,-1,-1,-1,-1, -1};
int relayer_fd=-1;//继电器relaystatus
int ads7844_fd = -1;   //ADS
int relaystatus[4]={0};

#ifdef _ADS7844FACTOR
float chFactorTable[8][7]={
    {6.51626264466281e-07,	-4.44913168675556e-05,	0.00120144924643856,	-0.0163239565089746,	0.117272282004958,	0.596607280228507,	0.619610018299306},
    {1.42911209270481e-07,	-9.57087501962936e-06,	0.000251882177974092,	-0.00333312144704970,	0.0236367337033549,	0.948315599881242,	0.130343314729920},
    {-4.62687515070381e-08,	2.29375134273707e-06,	-4.90917128870152e-05,	0.000707354346709794,	-0.00735732878932692,	1.09028797607968,	-0.122176323773473},
    {-3.04527628916453e-07,	2.31231570303145e-05,	-0.000699509347067712,	0.0106904223120660,	-0.0859664858500986,	1.36906379099803,	-0.504345985244920},
    {4.15697124031250e-07,-2.67533215685637e-05,0.000663420431340948,-0.00794324410880276,	0.0472635873553718,	0.899566437941808,	0.112036561648686},
    {1.00096130781528e-07,	-8.50414445297472e-06,	0.000280728303297303,	-0.00458054815326364,	0.0386698318674756,	0.879832267853327,	0.276099299059506},
    {7.09193817652926e-07,	-4.69755106459494e-05,	0.00123741750152267,	-0.0165511941190889,	0.118559858202989,	0.599219218796235,	0.635658586887564},
    {-5.54429763109226e-07,	3.83564929660619e-05,	-0.00106486826346328,	0.0151142739835666,	-0.114713489708895,	1.45789565211489,	-0.654567006175793}
};
#endif

int Single_PortConfig(URT_PORT port, SERIAL_PORT_CONFIG config){
    struct termios newtio;
    if(tcgetattr(uart_fd[port], &newtio)!=0){    //获取原来的串口配置
        perror("Setup Serial 1");
        return -1;
    }
    bzero(&newtio, sizeof(newtio));   //或者使用memset(&newtio, 0, sizeof(newtio))
    newtio.c_cflag != CLOCAL | CREAD;  //开启串行数据接受，并打开本地连接
    newtio.c_cflag &= ~CSIZE;    //先使用CSIZE作位屏蔽
    switch(config.dataBits){
        case 7:
            newtio.c_cflag |= CS7; break;  //设置7位数据位
        case 8:
            newtio.c_cflag |= CS8; break;   //设置8位数据位
    }
    switch(config.checkBits){
        case ODD:
            newtio.c_cflag |= PARENB;    //校验位
            newtio.c_cflag |= PARODD;    //奇校验
            newtio.c_iflag |= INPCK;
            break;
        case EVEN:
            newtio.c_iflag |= INPCK;
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case NONE:
            newtio.c_cflag &= ~PARENB;
            newtio.c_iflag &= ~INPCK;
            break;
    }
    switch(config.baud){
        case BD2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case BD4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case BD9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case BD19200:
            cfsetispeed(&newtio, B19200);
            cfsetospeed(&newtio, B19200);
            break;
        case BD115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
    }
    if( config.stopBits == 1 )
    {
        newtio.c_cflag &=  ~CSTOPB;   //一位停止位
    }
    else if ( config.stopBits == 2 )
    {
        newtio.c_cflag |=  CSTOPB;
    }
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(uart_fd[port],TCIFLUSH);     //清空末端未完成的请求及数据
    if((tcsetattr(uart_fd[port],TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return -1;

    }
    printf("set done!\n");
    return 0;
}

int open_port(URT_PORT port)
{
    long  vdisable;
    if(port>sizeof(UART_PORT_DEVICENAME)/sizeof(char*)){
        perror("Serial Port Not Exist !");
        return -1;
    }
    printf("%s\n",UART_PORT_DEVICENAME[port]);
    uart_fd[port] = open(UART_PORT_DEVICENAME[port], O_RDWR|O_NOCTTY|O_NDELAY);
    if(-1==uart_fd[port]){
        perror("Can't Open Serial Port");
        return(-1);
    }
    else{
        printf("open %s .....\n", UART_PORT_DEVICENAME[port]);
    }

    if(fcntl(uart_fd[port], F_SETFL, 0)<0)
    {
        printf("fcntl failed!\n");
    }
    else
    {
        printf("fcntl=%d\n",fcntl(uart_fd[port], F_SETFL,0));
    }
    if(isatty(STDIN_FILENO)==0)
    {
        printf("standard input is not a terminal device\n");
    }
    else
    {
        printf("isatty success!\n");
    }
    printf("fd-open=%d\n",uart_fd[port]);
    return uart_fd[port];
}
int Single_Port_OpenAndConfig(URT_PORT port,SERIAL_PORT_CONFIG config){  //界面加载时调用一次
    int i;
    if((uart_fd[port]=open_port(port))<0)
    {
        perror("open_port error");
        return -1;
    }
    if((i=Single_PortConfig(port, config)))
    {
        perror("set_opt error");
        return -1;
    }
  //  printf("fd=%d\n",uart_fd[port]);
    return uart_fd[port];
}
void All_Port_OpenAndConfig(SERIAL_PORT_CONFIG configs[]){
    URT_PORT port;
    int i=0;
    for( i=0; i<sizeof(UART_PORT_DEVICENAME)/sizeof(char*); i++){
        port = i;
        Single_Port_OpenAndConfig(port, configs[port]);
    }
}
int Single_Port_Read(URT_PORT port,  char buf[], int len){
    int nread=0;
    if(port!=5 && port != 6)
        nread=read(uart_fd[port],buf,len);

    return nread;
}
void All_Port_Read(char **buf, int *len){
    int i=0, num=sizeof(UART_PORT_DEVICENAME)/sizeof(char*);
    for(i=0; i<num; i++){
        read(uart_fd[i], buf[i], len[i]);
    }
}

int Single_Port_Write(URT_PORT port, int len,unsigned char buf[]){
    int nwrite = 0;
    nwrite = write(uart_fd[port], buf, len);
   /*
    * for(int i=0; i<8; i++){
        printf(" buf.data[%d]=%c ", i, buf.data[i]);
    }
    printf("\n");
    printf("Single_Port_Write()===============>nwite: %d\n" ,nwrite);
    */
    return nwrite;
}

u_int16_t GetCrcData(u_int8_t *data, u_int32_t len) {
    u_int32_t i;
    u_int8_t j;
    u_int16_t crc = 0xffff; // 16位crc寄存器预置
    for ( i = 0 ; i < len ; i++) { // 循环计算每个数据
        crc ^= data[i]; // 将八位数据与crc寄存器亦或，然后存入crc寄存器
        for (j = 0; j < 8; j++) { // 循环计算数据的
            if (crc & 0x0001) { // 判断右移出的是不是1，如果是1则与多项式进行异或。
                crc >>= 1; // 将数据右移一位
                crc ^= 0xa001; // 与上面的多项式进行异或
            } else { // 如果不是1，则直接移出
                crc >>= 1; // 将数据右移一位
            }
        }
    }
            return (crc << 8) | (crc >> 8);
}


void Single_Port_Close(URT_PORT port){

    close(uart_fd[port]);
    uart_fd[port] = -1;
}
void All_Port_Close(){
    int ch;
    int length = sizeof(uart_fd)/sizeof(int);
    for(ch=0; ch<length; ch++){
        Single_Port_Close(ch);
    }
}

int Single_485_Read(URT_PORT port,  unsigned char buf[], int len){//ch为通道号
    int nread=0;

    if(port==5 || port ==6){
        int ch;
        if(relayer_fd==-1)
            OpenRelayer();
        ch =port==PORT5?1:2;
        int ffd[2]={relayer_fd, uart_fd[port]};
        nread = rs485_rx(ffd, ch, buf,  len);
  printf("485 port = %d #### read: data length = %d\n",port,nread);
    }
    else{
        nread = Single_Port_Read(port, buf, len);
  printf("232 port = %d #### read: data length = %d\n",port,nread);
    }

    for(int i=0; i<nread; i++){
        printf("%x ", buf[i]);
    }
    printf("\n");
    return nread;
}
int Single_485_Write(URT_PORT port, unsigned char sbuf[], int len, SERIAL_PORT_CONFIG config){  //ch为通道号
    if(port==5 || port ==6){
        int ch;
        ch = port==PORT5?1:2;
        if(relayer_fd==-1)
            OpenRelayer();
        int ffd[2]={relayer_fd, uart_fd[port]};
  printf("485 write:  port = %d , BAUD=%d\n",port,config.baud);
//  config.baud=9600;
//  config.dataBits=8;
//  config.stopBits=1;
//  config.checkBits=0;
//  config.startBits=1;
  printf("++++++++++++++++++++++++++++++++\n");
            for(int i=0; i<8; i++){
                printf("%x ", sbuf[i]);
            }
            printf("\n");
          int nwrite = rs485_tx(ffd, ch, sbuf, len, config);
          return nwrite;
    }
    else{

        printf("++++++++++++++++++++++++++++++++\n");
                  for(int i=0; i<8; i++){
                      printf("%x ", sbuf[i]);
                  }
                  printf("\n");
        int nwrite = Single_Port_Write(port , len, sbuf);
        return nwrite;
    }

}
int rs485_rx(int *fd, int ch,  unsigned char buf[], int len){
    int nread = 0;
    RS485_RX_TX rs485;

    if(ch == 1){
        rs485 = RS485_RX1;
    }
    else {
        rs485 = RS485_RX2;
    }

    //ioctl(fd[0], _IO(RELAYER_IOC_MAGIC, rs485));
    nread = read(fd[1], buf, len);

    //ioctl(fd[0], _IO(RELAYER_IOC_MAGIC, rs485+1));

    return nread;

}

int rs485_tx(int *fd, int ch,  unsigned char buf[], int len, SERIAL_PORT_CONFIG config){
    int i;
    int nwrite;
    int time,check;
    RS485_RX_TX rs485;
    if(ch == 1){
        rs485 = RS485_TX1;
    }
    else {
        rs485 = RS485_TX2;
    }
   switch(config.baud){
        case BD2400:
            time = 417*len;
        break;
        case BD4800:
            time = 209*len;
        break;
        case BD9600:
            time = 105*len;
            break;
    case BD19200:
            time = 53*len;
        break;
    case BD115200:
            time = 9*len;
        break;
    }
    if(config.checkBits == EVEN|| config.checkBits == ODD)
    {
        check = 1;
    }
    else{
        check = 0;
    }
    switch(config.stopBits+check){
        case 1:
        time = 10*time;
        break;
        case 2:
        time = 11*time;
        break;
        case 3:
        time = 12*time;
        break;
    }


    //sleep(1);
    printf("//////////////////////////\n");
    for(i=0;i<8;i++){
        printf("%x ",buf[i]);
    }
    printf("\n");

    ioctl(fd[0],_IO(RELAYER_IOC_MAGIC, rs485));
    nwrite = write(fd[1], buf, len);
    usleep(time+200);

    //printf("write data length: %d\n",nwrite);

    ioctl(fd[0],_IO(RELAYER_IOC_MAGIC, rs485-1));
    return nwrite;

    //sleep(1);

}
//*******************************************************************//
//Relayer Functions
int OpenRelayer(){   //打开继电器，在界面加载时调用

    relayer_fd = open(RELAYER_DEVICENAME, O_RDWR);
    if(relayer_fd<0)
        printf("open failed\n");
    else
        printf("open success\n");
    return relayer_fd;
}
void CloseRelayer(){//关闭继电器，在界面关闭时调用
    close(relayer_fd);
    relayer_fd = -1;
}
void Single_On(RELAYER_CHANNEL ch){//打开单个通道，单击打开通道按钮时调用
    int err  = ioctl(relayer_fd, _IO(RELAYER_IOC_MAGIC, ch));
    relaystatus[ch-1]=1;
    printf("Relayer Single_On, %d\n", err);
}
void Single_Off(RELAYER_CHANNEL ch){//关闭单个通道，单击关闭通道按钮时调用
    int err  = ioctl(relayer_fd, _IO(RELAYER_IOC_MAGIC, ch+4));
    relaystatus[ch-1]=0;
    printf("Relayer Single_Off, %d\n", err);
}


void All_On(){ //打开所有通道，单击打开所有通道按钮时调用
    RELAYER_CHANNEL ch;
    for(ch=CH1; ch<=CH4;ch++){
        Single_On(ch);
        printf("Channel %d Switch On Completed !\n",ch);
    }
    printf("All Channel Switch On Completed !");
}
void All_Off(){//关闭所有通道，单击关闭所有通道按钮时调用
    RELAYER_CHANNEL ch;
    for(ch=CH1; ch<=CH4;ch++){
        Single_Off(ch+4);
        printf("Channel %d Switch Off Completed !\n",ch);
    }
    printf("All Channel Switch Off Completed !");
}
//===========================ADS转换区开始========================================
//打开ADS的操作一般是在界面初始化的时候执行ADS7844_Open()
int ADS7844_Open(){
    ads7844_fd = open(ADS7844_DEVICENAME, O_RDWR);
    if(ads7844_fd<0)
        printf("open failed\n");
    else
        printf("open success\n");
    return ads7844_fd;
}
//关闭（界面退出或程序结束时记得关闭)
void ADS7844_Close(){
    close(ads7844_fd);
    ads7844_fd = -1;
}
//读8通道数据（该读取通道数据的操作一般就在按钮点击事件中执行，以便刷新单个通道的数据）
int ADS7844_Single_Read(ADS7844_CHANNEL ch, float *result){   //ch=0,1,2,....,7
    int err;
    int16_t data;

#ifdef _ADS7844FACTOR
    double r;
    int i,j;
    double x,y=1,sum = 0;
#endif

    int cmd = _IOR(ADS_IOC_MAGIC, ch, int);
    err = ioctl(ads7844_fd,cmd,&data);
    printf("%d\n",err);
    *result = data*ADS_FACTOR;

    #ifdef _ADS7844FACTOR
    r = (*result)/200;
    r = r*1000;
    if(r>=4&&r<=20)
    {
    printf("-------------1\n");
    for(i=1; i<=7; i++){

        for(j=1; j<=7-i; j++){
            y = y*r;
        }
        sum = sum + y*chFactorTable[ch][i-1];
        y = 1;
    }
    *result = (float)sum * 200/1000;
    }


#endif

    return err;
}
//读8通道数据（读取通道数据的操作一般就在定时线程中执行）
void ADS7844_All_Read(float result[], int err[]){

    int ch=0;
    for(  ch=0; ch<8;ch++){
        err[ch] = ADS7844_Single_Read(ch, &result[ch]);
        printf("Read Channel %d Completed !\n", ch);
    }
    printf("All Channel Data Read Completed !");

}

/*==============================================================================================================================*/
void PLC_Time_Set(URT_PORT port, unsigned char slave_address, unsigned short reg_address, unsigned char time[],SERIAL_PORT_CONFIG config){//设置检测仪表时间 
	//传入参数为端口 从站地址 寄存器地址 设置时间(20200801080101)
	unsigned char cmd[21];
    unsigned char *year;
    unsigned char *month;
    unsigned char *day;
    unsigned char *hour;
    unsigned char *min;
    unsigned char *second;
    unsigned short crc;
    unsigned char confirm[9];
    year = (char*)malloc(4);
    month = (char*)malloc(2);
    day = (char*)malloc(2);
    hour = (char*)malloc(2);
    min = (char*)malloc(2);
    second = (char*)malloc(2);
    int i;
    for(i=0;i<15;i++){
        if(i < 4)
            *(year+i) = time[i];
        if(i>=4&&i<6)
        {
            *(month+i-4) = time[i];
            //printf("month=%x\n",month[i-4]);
        }

        if(i>=6&&i<8)
            *(day+i-6) = time[i];
        if(i>=8&&i<10)
            *(hour+i-8) = time[i];
        if(i>=10&&i<12)
            *(min+i-10) = time[i];
        if(i>=12&&i<14)
            *(second+i-12) = time[i];
    }


	/*拼接发送指令*/
    cmd[0] = slave_address;
    cmd[1] = 0x10;
    cmd[2] = (unsigned char)(reg_address>>8);
    cmd[3] = (unsigned char)(reg_address&0x00ff);
    cmd[4] = 0x00;
    cmd[5] = 0x06;
    cmd[6] = 0x0c;
    cmd[7] = (unsigned char)(atol(second)>>8);
    cmd[8] = (unsigned char)(atol(second)&0x00ff);
    cmd[9] = (unsigned char)(atol(min)>>8);
    cmd[10] = (unsigned char)(atol(min)&0x00ff);
    cmd[11] = (unsigned char)(atol(hour)>>8);
    cmd[12] = (unsigned char)(atol(hour)&0x00ff);
    cmd[13] = (unsigned char)(atol(day)>>8);
    cmd[14] = (unsigned char)(atol(day)&0x00ff);
    cmd[15] = (unsigned char)(atol(month)>>8);
    cmd[16] = (unsigned char)(atol(month)&0x00ff);
    cmd[17] = (unsigned char)(atol(year)>>8);
    cmd[18] = (unsigned char)(atol(year)&0x00ff);
    crc = GetCrcData(cmd,19);
   // printf("%x\n",crc);
    cmd[19] = (unsigned char)(crc>>8);
    cmd[20] = (unsigned char)(crc&0x00ff);

      // printf("%x\n",cmd[19]); 
       //printf("%x\n",cmd[20]); 
      // printf("%x\n",cmd[4]); 
    clearbuff(port);
    Single_485_Write(port, cmd, sizeof(cmd),config);//发送指令
 
    free(year);
    free(month);
    free(day);
    free(hour);
    free(min);
    free(second);
    confirm[0] = slave_address;
    confirm[1] = 0x06;
    confirm[2] = 0x00;
    confirm[3] = 0x34;
    confirm[4] = 0x00;
    confirm[5] = 0x01;
    crc = GetCrcData(confirm,6);
    confirm[7] = (unsigned char)(crc>>8);
    confirm[8] = (unsigned char)(crc&0x00ff);
    //msleep(200);
    clearbuff(port);
    Single_485_Write(port, confirm, sizeof(confirm),config);//发送确认指令
}

void clearbuff(URT_PORT port){
    int sign = tcflush(uart_fd[port], TCIOFLUSH);
    printf("sign = %d\n",sign);
}
/*==============================================================================================================================*/
/*void SamplerState(URT_PORT port, unsigned char slave_address, unsigned short reg_address, struct Sample_Info info, SERIAL_PORT_CONFIG config){
    char cmd[7];
    char recvbuf1[11];
    unsigned short crc;char i42001_reg;
    cmd[0] = slave_address;
    cmd[1] = 0x03;
    cmd[2] = (unsigned char)(reg_address>>8);
    cmd[3] = (unsigned char)(reg_address&0x00ff);
    cmd[4] = 0x00;
    cmd[5] = 0x03;
    crc = GetCrcData(cmd,6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    Single_485_Write(port, cmd, 8, config);
    Single_485_Read(port,recvbuf1,11);
    //info.i42001_info.i42001_reg = "i42001";

    
}
*/
/*void main(){
    URT_PORT PORT5;
    SERIAL_PORT_CONFIG config[7];
    config[0].baud=BD115200;
    config[0].startBits=START_ONE;
    config[0].dataBits =DATA_EIGHT;
    config[0].checkBits=NONE;
    config[0].stopBits=STOP_ONE;
    config[6] = config[5] = config[4] = config[3] = config[2] = config[1] = config[0];
    unsigned char slave_address = 1;
    unsigned short reg_address  = 86;
    unsigned char time[] = "20200801080101";
    
    All_Port_OpenAndConfig(config);
    PLC_Time_Set(PORT5, slave_address, reg_address, time, config[5]);
}*/


//========================ADS转换区结束=========================
/*
int main(void)
{
    int nread[4];
    //char buff0[]="Test Data\n", buff1[]="Test Data\n",buff2[]="Test Data\n",buff3[]="Test Data\n";
    char *buff[]={"Test Data\n","Test Data\n","Test Data\n","Test Data\n"};
    //实际应用中，请确保下面的代码只执行一次，一般在程序启动时或界面加载时执行，
     //实际应用中，将以下一些代码作为全局变量，这些代码只针对一个端口的擦作

    SERIAL_PORT_CONFIG config[4];
    config[0].baud=BD115200;
    config[0].startBits=START_ONE;
    config[0].dataBits =DATA_EIGHT;
    config[0].checkBits=NONE;
    config[0].stopBits=STOP_ONE;
    if((uart_fd[0]=Single_Port_OpenAndConfig(0,config[0]))<0){   //端口0
        perror("open_port error");
        return -1;
    }

   //实际应用中，每当刷新数据时，执行下面代码；
    nread[0]=Single_Port_Read(0,buff[0], 16);
    printf("nread=%d,%s\n",nread[0],buff[0]);

    //实际应用中，当程序或界面退出时，执行关闭文件操作
    close(uart_fd[0]);
    return -1;
}
*/
int get_RelayStatus(RELAYER_CHANNEL ch){
    return relaystatus[ch-1];
}
