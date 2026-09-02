#include "SamplerState.h"


SamplerState bSamplerState;

SamplerState w;
//bSamplerState = w;

QString SamplerState::DataFormatAnalysis(DataAlign dataformat, unsigned char *data)//大端小端模式解析
{
    //qDebug()<<data[0]<<","<<data[1]<<","<<data[2]<<","<<data[3]<<","<<endl;
    QString result;
    union chartofloat{
            unsigned char a[4];
            float b;
        };
    union chartofloat value1;
    switch(dataformat){
    case DA4321:
        value1.a[0] = *data;
        value1.a[1] = *(data+1);
        value1.a[2] = *(data+2);
        value1.a[3] = *(data+3);
        break;
    case DA1234:
        value1.a[0] = *(data+3);
        value1.a[1] = *(data+2);
        value1.a[2] = *(data+1);
        value1.a[3] = *data;
        break;
    case DA3412:
        value1.a[0] = *(data+1);
        value1.a[1] = *data;
        value1.a[2] = *(data+3);
        value1.a[3] = *(data+2);
         //qDebug()<<__FILE__<<__func__<<"DA3412"<<endl;
        break;
    case DA2143:
        value1.a[0] = *(data+2);
        value1.a[1] = *(data+3);
        value1.a[2] = *data;
        value1.a[3] = *(data+1);
        break;
    }
    //qDebug()<<__FILE__<<__func__<<QString("%1").arg(value1.b)<<endl;
    return QString("%1").arg(value1.b);
}

void SamplerState::DataFormatAnalysis(DataAlign dataformat, unsigned char *data, float floatdata)
{

    QString result;
    union chartofloat{
            unsigned char a[4];
            float b;
        };
    union chartofloat value1;
    value1.b = floatdata;
    switch(dataformat){
    case DA4321:
        /*value1.a[0] = *data;
        value1.a[1] = *(data+1);
        value1.a[2] = *(data+2);
        value1.a[3] = *(data+3);*/
        *data = value1.a[0];
        *(data+1) = value1.a[1];
        *(data+2) = value1.a[2];
        *(data+3) = value1.a[3];
        break;
    case DA1234:
        /*value1.a[0] = *(data+3);
        value1.a[1] = *(data+2);
        value1.a[2] = *(data+1);
        value1.a[3] = *data;*/
        *data = value1.a[3];
        *(data+1) = value1.a[2];
        *(data+2) = value1.a[1];
        *(data+3) = value1.a[0];
        break;
    case DA3412:
        /*value1.a[0] = *(data+1);
        value1.a[1] = *data;
        value1.a[2] = *(data+3);
        value1.a[3] = *(data+2);*/
        *data = value1.a[1];
        *(data+1) = value1.a[0];
        *(data+2) = value1.a[3];
        *(data+3) = value1.a[2];
        break;
    case DA2143:
        /*value1.a[0] = *(data+2);
        value1.a[1] = *(data+3);
        value1.a[2] = *data;
        value1.a[3] = *(data+1);*/
        *data = value1.a[2];
        *(data+1) = value1.a[3];
        *(data+2) = value1.a[0];
        *(data+3) = value1.a[1];
        break;
    }
}

SamplerState::SamplerState()
{

   /* sample_info.insert("12","44");
    bool s = sample_info.contains("121");
    if(s==true){
        //qDebug()<<sample_info.value("121")<<endl;
    }
    else{
        //qDebug()<<"s="<<s<<sample_info.value("121")<<endl;
    }*/
}

SamplerState::~SamplerState()
{

}

bool SamplerState::MonitorIndex(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config, DataAlign dataformat, QMap<QString, QString> &resultMap)
{
    bool r = MonitorIndexCheck(port, slave_address, reg_address, config);
    if(r)
        MonitorIndexCheck_data(dataformat, resultMap);

    return r;

}

bool SamplerState::readWQSDataInfo(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config, QMap<QString, QString> &resultMap)
{
    bool r = getWQSData(port, slave_address, reg_address, config);
    if(r){
        //qDebug()<<"####after getWQData"<<__FILE__<<__func__<<endl;
        WQSDataInfo(resultMap);
    }

    return r;
}

bool SamplerState::readWQSDataInfo_new(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config, QMap<QString, QString> &resultMap)
{
    bool r = getWQSData_new(port, slave_address, reg_address, config);
    if(r){
        //qDebug()<<"####after getWQData"<<__FILE__<<__func__<<endl;
        WQSDataInfo_new(resultMap);
    }

    return r;
}


bool SamplerState::readAnalysisMeterInfo(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config,QMap<QString, QString> &resultMap, DataAlign dataformat)
{
    StateExtract(port, slave_address, reg_address, config);
    Get_StateExtract_Info(resultMap, dataformat);
    return true;
}

bool SamplerState::dev(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config, DataAlign dataformat, QMap<QString, QString> &resultMap)
{
    bool r = DevFlag(port, slave_address, reg_address, config);
    if(r){
        DevFlag_Date(dataformat, resultMap);

    }
    return r;
}

QString SamplerState::YSENVIR(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config,DataAlign dataformat)
{
    unsigned char send[8];
    unsigned short crc;
    /*-----构建发送指令-------------*/
    send[0] = slave_address;
    send[1] = 0x03;
    send[2] = (unsigned char)(reg_address>>8);
    send[3] = (unsigned char)(reg_address&0x00ff);
    /*----------------*/
    if(reg_address==527||reg_address==549){//温度湿度浮点类型
        unsigned char recv[9];
        send[4] = 0x00;
        send[5] = 0x02;
        crc = GetCrcData(send,6);
        send[6] = (unsigned char)(crc>>8);
        send[7] = (unsigned char)(crc&0x00ff);
        Single_485_Write(port,  send, sizeof (send),  config);//发送指令
        /*-------------------读取数据-----------------*/
        int count = 3;
        QThread::msleep(30);
        while(count--){
            if(Single_485_Read(port, recv, 9)>0)
                break;
            QThread::msleep(50);
        }
    /*-----------------------------------------------*/
        QString data = DataFormatAnalysis(dataformat, recv+3);
        return data;
    }
    else{//烟感，门禁，水浸为整数类型 
        unsigned char recv[7];
        send[4] = 0x00;
        send[5] = 0x01;
        crc = GetCrcData(send,6);
        send[6] = (unsigned char)(crc>>8);
        send[7] = (unsigned char)(crc&0x00ff);
        Single_485_Write(port,  send, sizeof (send),  config);//发送指令
        while(Single_485_Read(port, recv, 7)==0);
        qint16 temple = ((qint16)recv[3])<<8;
        temple = temple + (qint16)recv[4];
        QString data;
        switch (reg_address) {
        case 103:
            if(temple==0){
                data = "正常";
            }
            else{
                data = "报警中";
            }
            break;
        case 101:
            if(temple==0){
                data = "关门";
            }
            else{
                data = "开门中";
            }
            break;
        case 51:
            if(temple==0){
                data = "正常";
            }
            else{
                data = "浸水中";
            }
            break;
        }
        return data;
    }

}

//水质采样器
bool SamplerState::getWQSData(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config)
{
    unsigned char cmd[8];//构建取值指令
    unsigned short crc;//crc校验值
    int datalen = 0;
    cmd[0] = slave_address;
    cmd[1] = 0x03;
    cmd[2] = 0x0b;
    cmd[3] = 0xb8;
    cmd[4] = 0x00;
    cmd[5] = 0x03;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);//发送指令

    /*-------------------读取数据-----------------*/
    memset(this->buf,0,89);
    int count = 3;
    while(count--){
        QThread::msleep(200);
        if((datalen = Single_485_Read(port, this->buf, 11))>0)
            break;

    }
    if(datalen==0){
        return false;
    }
    /*-----------------------------------------------*/



    /*-----------------构建第二次取值指令-----------*/
    cmd[2] = 0x0c;
    cmd[3] = 0x1c;
    cmd[4] = 0x00;
    cmd[5] = 0x1a;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);
    datalen = 0;
    /*-------------------读取数据-----------------*/
    count = 3;
    while(count--){
        QThread::msleep(200);
        if((datalen = Single_485_Read(port, this->buf+11, 57))>0)
            break;

    }
    if(datalen == 0){
        return false;
    }
    /*-----------------------------------------------*/


    /*-----------------构建第三次取值指‵令-----------*/
    cmd[2] = 0x0c;
    cmd[3] = 0x44;
    cmd[4] = 0x00;
    cmd[5] = 0x08;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);

    datalen = 0;
    /*-------------------读取数据-----------------*/
    count = 3;
    while(count--){
        QThread::msleep(200);
        if((datalen = Single_485_Read(port, this->buf+68, 21))>0)
            break;

    }

    if(datalen == 0){
        return false;
    }
    /*-----------------------------------------------*/
    return true;
}

//新水质采样器
bool SamplerState::getWQSData_new(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config)
{
    unsigned char cmd[8];//构建取值指令
    unsigned short crc;//crc校验值
    int datalen = 0;
    /*---------------读取采样器状态-------------*/
    cmd[0] = slave_address;
    cmd[1] = 0x03;
    cmd[2] = 0x10;
    cmd[3] = 0x45;
    cmd[4] = 0x00;
    cmd[5] = 0x01;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);

    for(int i=0; i<8; i++){
        printf("%x ", cmd[i]);
    }
    printf("\n");
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);//发送指令

    /*-------------------读取数据-----------------*/
    memset(this->buf,0,90);
    int count = 3;
    while(count--){
        QThread::msleep(200);
        if((datalen = Single_485_Read(port, this->buf, 7))>0)
            break;

    }
    if(datalen==0){
        return false;
    }
    /*-----------------读取采样器报警状态和采样模式-----------*/
    cmd[2] = 0x12;
    cmd[3] = 0x71;
    cmd[4] = 0x00;
    cmd[5] = 0x02;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);

    for(int i=0; i<8; i++){
        printf("%x ", cmd[i]);
    }
    printf("\n");
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);
    datalen = 0;
    /*-------------------读取数据-----------------*/
    count = 3;
    while(count--){
        QThread::msleep(200);
        if((datalen = Single_485_Read(port, this->buf+7, 9))>0)
            break;

    }
    if(datalen == 0){
        return false;
    }
    /*-----------------------------------------------*/

    /*-----------------读取采样系统门禁状态和采样系统门禁报警状态-----------*/
    cmd[2] = 0x10;
    cmd[3] = 0x42;
    cmd[4] = 0x00;
    cmd[5] = 0x01;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);

    for(int i=0; i<8; i++){
        printf("%x ", cmd[i]);
    }
    printf("\n");
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);
    datalen = 0;
    /*-------------------读取数据-----------------*/
    count = 3;
    while(count--){
        QThread::msleep(200);
        if((datalen = Single_485_Read(port, this->buf+16, 7))>0)
            break;

    }
    if(datalen == 0){
        return false;
    }
    /*-----------------------------------------------*/
    /*-----------------读取采样系统固定密码和动态密码-----------*/
    cmd[2] = 0x12;
    cmd[3] = 0x63;
    cmd[4] = 0x00;
    cmd[5] = 0x06;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);

    for(int i=0; i<8; i++){
        printf("%x ", cmd[i]);
    }
    printf("\n");
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);
    datalen = 0;
    /*-------------------读取数据-----------------*/
    count = 3;
    while(count--){
        QThread::msleep(200);
        if((datalen = Single_485_Read(port, this->buf+23, 17))>0)
            break;

    }
    if(datalen == 0){
        return false;
    }
    /*-----------------------------------------------*/
    /*-----------------读取门禁开关时间-----------*/
    cmd[2] = 0x11;
    cmd[3] = 0x98;
    cmd[4] = 0x00;
    cmd[5] = 0x0A;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);

    for(int i=0; i<8; i++){
        printf("%x ", cmd[i]);
    }
    printf("\n");
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);
    datalen = 0;
    /*-------------------读取数据-----------------*/
    count = 3;
    while(count--){
        QThread::msleep(200);
        if((datalen = Single_485_Read(port, this->buf+40, 25))>0)
            break;

    }
    if(datalen == 0){
        return false;
    }
    /*-----------------------------------------------*/

    /*-----------------读取超标留样时间-----------*/
    cmd[2] = 0x13;
    cmd[3] = 0xA6;
    cmd[4] = 0x00;
    cmd[5] = 0x0F;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);

    for(int i=0; i<8; i++){
        printf("%x ", cmd[i]);
    }
    printf("\n");
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);
    datalen = 0;
    /*-------------------读取数据-----------------*/
    count = 3;
    while(count--){
        QThread::msleep(200);
        if((datalen = Single_485_Read(port, this->buf+65, 35))>0)
            break;

    }
    if(datalen == 0){
        return false;
    }
    /*-----------------------------------------------*/
    /*-----------------读取最后一次门禁操作时间-----------*/
    cmd[2] = 0x12;
    cmd[3] = 0x6E;
    cmd[4] = 0x00;
    cmd[5] = 0x03;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);

    for(int i=0; i<8; i++){
        printf("%x ", cmd[i]);
    }
    printf("\n");
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);
    datalen = 0;
    /*-------------------读取数据-----------------*/
    count = 3;
    while(count--){
        QThread::msleep(200);
        if((datalen = Single_485_Read(port, this->buf+100, 11))>0)
            break;

    }
    if(datalen == 0){
        return false;
    }
    /*-----------------------------------------------*/

    /*-----------------读取留样瓶号-----------
    cmd[2] = 0x13;
    cmd[3] = 0xA6;
    cmd[4] = 0x00;
    cmd[5] = 0x06;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);
    datalen = 0;
    /*-------------------读取数据-----------------
    count = 3;
    while(count--){
        QThread::msleep(200);
        if((datalen = Single_485_Read(port, this->buf+74, 9))>0)
            break;

    }
    if(datalen == 0){
        return false;
    }
    /*-----------------------------------------------

    /*-----------------读取超标留样水量-----------
    cmd[2] = 0x13;
    cmd[3] = 0xB1;
    cmd[4] = 0x00;
    cmd[5] = 0x0F;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);
    datalen = 0;
    /*-------------------读取数据-----------------
    count = 3;
    while(count--){
        QThread::msleep(200);
        if((datalen = Single_485_Read(port, this->buf+83, 7))>0)
            break;

    }
    if(datalen == 0){
        return false;
    }
    /*-----------------------------------------------*/

/*
    //-----------------构建第二次取值指令-----------
    cmd[2] = 0x0c;
    cmd[3] = 0x1c;
    cmd[4] = 0x00;
    cmd[5] = 0x1a;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);
    datalen = 0;
    //-------------------读取数据-----------------
    count = 3;
    while(count--){
        QThread::msleep(200);
        if((datalen = Single_485_Read(port, this->buf+11, 57))>0)
            break;

    }
    if(datalen == 0){
        return false;
    }
    /*-----------------------------------------------


    //-----------------构建第三次取值指‵令-----------
    cmd[2] = 0x0c;
    cmd[3] = 0x44;
    cmd[4] = 0x00;
    cmd[5] = 0x08;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);

    datalen = 0;
    //-------------------读取数据-----------------
    count = 3;
    while(count--){
        QThread::msleep(200);
        if((datalen = Single_485_Read(port, this->buf+68, 21))>0)
            break;

    }

    if(datalen == 0){
        return false;
    }
    -----------------------------------------------*/
    return true;
}

void SamplerState::WQSDataInfo(QMap<QString, QString> &sample_info)
{
    bSamplerState.UpCYQ = "";

    qint16 temple = ((qint16)this->buf[3])<<8;
    temple = temple + (qint16)this->buf[4];
    if(temple>=0&&temple < 2)
    {
        bSamplerState.UpCYQ += "i42001-Info="+QString::number(temple)+";";
        sample_info.insert("i42001", this->status.i42001[temple]);
    }

    temple = ((qint16)this->buf[5])<<8;
    temple = temple + (qint16)this->buf[6];//采样器报警状态
    if(temple>=0&&temple < 5)
    {
        bSamplerState.UpCYQ += "i42002-Info="+QString::number(temple)+";";
        sample_info.insert("i42002", this->status.i42002[temple]);
    }

    temple = ((qint16)this->buf[7])<<8;
    temple = temple + (qint16)this->buf[8];//采样模式
    if(temple>=0&&temple < 5)
    {
        bSamplerState.UpCYQ += "i42003-Info="+QString::number(temple)+";";
        sample_info.insert("i42003", this->status.i42003[temple]);
    }

    temple = ((qint16)this->buf[14])<<8;
    temple = temple + (qint16)this->buf[15];//采样系统门禁状态
    if(temple>=0&&temple < 2)
    {
        bSamplerState.UpCYQ += "i42101-Info="+QString::number(temple)+";";
        sample_info.insert("i42101", this->status.i42101[temple]);
    }

    temple = ((qint16)this->buf[16])<<8;
    temple = temple + (qint16)this->buf[17];//采样系统门禁报警状态
    if(temple>=0&&temple < 3)
    {
        bSamplerState.UpCYQ += "i42102-Info="+QString::number(temple)+";";
        sample_info.insert("i42102", this->status.i42102[temple]);
    }

    QString str = "";
    for(int i = 18; i < 30; i = i+2){//系统采样动态密码
        temple = ((qint16)this->buf[i])<<8;
        temple = temple + (qint16)this->buf[i+1];
        str = str + QString::number(temple, 10);
    }
    sample_info.insert("i42103", str);
    bSamplerState.UpCYQ += "i42103-Info="+str+";";

    str = "";
    for(int i = 30; i < 42; i = i+2){//固定密码
        temple = ((qint16)this->buf[i])<<8;
        temple = temple + (qint16)this->buf[i+1];
        str = str + QString::number(temple, 10);
    }
    sample_info.insert("i42104", str);
    bSamplerState.UpCYQ += "i42104-Info="+str+";";

    str = "";
    for(int i = 42; i < 54; i = i+2){//门禁开关时间
        temple = ((qint16)this->buf[i])<<8;
        temple = temple + (qint16)this->buf[i+1];
        if(temple < 10){
            str = str + "0" + QString::number(temple, 10);
        }
        else{
            str = str + QString::number(temple, 10);
        }
    }
    sample_info.insert("i42105", str);
    bSamplerState.UpCYQ += "i42105-Info="+str+";";

    str = "";
    for(int i = 54; i < 66; i = i+2){//门禁登录信息
        temple = ((qint16)this->buf[i])<<8;
        temple = temple + (qint16)this->buf[i+1];
        str = str + QString::number(temple, 10);
    }
    sample_info.insert("i42106", str);
    bSamplerState.UpCYQ += "i42106-Info="+str+";";

    str = "";
    for(int i = 71; i < 83; i = i+2){//超标留样时间
        temple = ((qint16)this->buf[i])<<8;
        temple = temple + (qint16)this->buf[i+1];
        if(temple < 10){
            str = str + "0" + QString::number(temple, 10);
        }
        else{
            str = str + QString::number(temple, 10);
        }
    }
    sample_info.insert("i43002", str);
    bSamplerState.UpCYQ += "i43002-Info="+str+";";

    str = "";
    temple = ((qint16)this->buf[83])<<8;
    temple = temple + (qint16)this->buf[84];//留样瓶号
    if(temple < 10){
        str = str + "0" + QString::number(temple, 10);
    }
    else{
        str = str + QString::number(temple, 10);
    }
    sample_info.insert("i43001", QString::number(temple, 10));
    bSamplerState.UpCYQ += "i43001-Info="+str+";";

    str = "";
    temple = ((qint16)this->buf[85])<<8;
    temple = temple + (qint16)this->buf[86];//超标留样水样量
    sample_info.insert("i43003", QString::number(temple, 10));
    bSamplerState.UpCYQ += "i43003-Info="+QString::number(temple);
}

void SamplerState::WQSDataInfo_new(QMap<QString, QString> &sample_info)
{
    bSamplerState.UpCYQ = "";
    qint16 temple = ((qint16)this->buf[3])<<8;//读取采样器状态
    temple = temple + (qint16)this->buf[4];

    if(temple>=0&&temple < 2)
    {
        bSamplerState.UpCYQ += "i42001-Info="+QString::number(temple)+";";
        sample_info.insert("i42001", this->status.i42001[temple]);
    }

    temple = ((qint16)this->buf[10])<<8;
    temple = temple + (qint16)this->buf[11];//采样器报警状态
    if(temple>=0&&temple < 5)
    {
        bSamplerState.UpCYQ += "i42002-Info="+QString::number(temple)+";";
        sample_info.insert("i42002", this->status.i42002[temple]);
    }

    temple = ((qint16)this->buf[12])<<8;
    temple = temple + (qint16)this->buf[13];//采样模式
    if(temple>=0&&temple < 5)
    {
        bSamplerState.UpCYQ += "i42003-Info="+QString::number(temple)+";";
        sample_info.insert("i42003", this->status.i42002[temple]);
    }

    temple = ((qint16)this->buf[19]);//采样系统门禁报警状态
    if(temple>=0&&temple < 2)
    {
        bSamplerState.UpCYQ += "i42102-Info="+QString::number(temple)+";";
        sample_info.insert("i42102", this->status.i43102[temple]);
    }

    temple = ((qint16)this->buf[20]);//采样系统门禁状态
    if(temple>=0&&temple < 3)
    {
        bSamplerState.UpCYQ += "i42101-Info="+QString::number(temple)+";";
        sample_info.insert("i42101", this->status.i43101[temple]);
    }

    QString str = "";
    for(int i = 32; i < 38; i++){//系统采样动态密码
        temple = (qint16)this->buf[i];
        temple = temple - 0x30;
        str = str + QString::number(temple, 10);
    }
    sample_info.insert("i42103", str);
    bSamplerState.UpCYQ += "i42103-Info="+str+";";

    str = "";
    for(int i = 26; i < 32; i++){//固定密码
        temple = (qint16)this->buf[i];
        temple = temple - 0x30;
        str = str + QString::number(temple, 10);
    }
    sample_info.insert("i42104", str);
    bSamplerState.UpCYQ += "i42104-Info="+str+";";

    str = "";
    for(int i = 43; i < 49; i++){//门禁开关时间
        temple = ((qint16)this->buf[i]);
        if(temple < 16){
            str = str + "0" + QString::number(temple, 16);
        }
        else{
            str = str + QString::number(temple, 16);
        }
    }
    sample_info.insert("i42105", str);
    bSamplerState.UpCYQ += "i42105-Info="+str+";";

    str = "";
    for(int i = 26; i < 32; i++){//门禁登录信息
        temple = (qint16)this->buf[i];
        temple = temple - 0x30;
        str = str + QString::number(temple, 10);
    }
    sample_info.insert("i42106", str);
    bSamplerState.UpCYQ += "i42106-Info="+str+";";

    str = "";
    for(int i = 68; i < 74; i++){//超标留样时间
        temple = ((qint16)this->buf[i]);
        if(temple < 16){
            str = str + "0" + QString::number(temple, 16);
        }
        else{
            str = str + QString::number(temple, 16);
        }
    }
    sample_info.insert("i43002", str);
    bSamplerState.UpCYQ += "i43002-Info="+str+";";

    str = "";
    temple = (qint16)this->buf[89];//留样瓶号
    temple = temple + (qint16)this->buf[84];//留样瓶号
    if(temple < 10){
        str = str + "0" + QString::number(temple, 10);
    }
    else{
        str = str + QString::number(temple, 10);
    }
    sample_info.insert("i43001", QString::number(temple, 10));
    bSamplerState.UpCYQ += "i43001-Info="+str+";";

    str = "";
    temple = ((qint16)this->buf[90])<<8;
    temple = temple + (qint16)this->buf[91];//超标留样水样量
    sample_info.insert("i43003", QString::number(temple, 10));
    bSamplerState.UpCYQ += "i43003-Info="+QString::number(temple);
}

//分析仪
void SamplerState::StateExtract(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config){
    unsigned char cmd[8];//构建取值指令
    unsigned short crc;//crc校验值
    cmd[0] = slave_address;
    cmd[1] = 0x03;
    cmd[2] = (unsigned char)(reg_address>>8);
    cmd[3] = (unsigned char)(reg_address&0x00ff);
    cmd[4] = 0x00;
    cmd[5] = 0x36;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    clearbuff(port);
    Single_485_Write(port, cmd, sizeof (cmd), config);

    /*-------------------读取数据-----------------*/
    int count = 3;
    while(count--){
        QThread::msleep(200);
        if(Single_485_Read(port, this->buff, 113)>0)
            break;

    }

    /*-----------------------------------------------*/

}

void SamplerState::Get_StateExtract_Info(QMap<QString, QString> &sample_info,DataAlign dataformat){


    //qDebug()<<"1: "<<__FILE__<<__func__<<endl;
    QString data = DataFormatAnalysis(dataformat, this->buff+3);

    /*union chartofloat{
            unsigned char a[4];
            float b;
        };
    union chartofloat value1;   
    value1.a[0] = this->buff[4];
    value1.a[1] = this->buff[3];
    value1.a[2] = this->buff[6];
    value1.a[3] = this->buff[5];
    QString str = QString("%1").arg(value1.b);//检测值*/
    //sample_info.insert("检测值", str);
    sample_info.insert("2061", data);

    qint16 temple = ((qint16)this->buff[7])<<8;
    temple = temple + (qint16)this->buff[8];
    //sample_info.insert("检测值数据标识", this->status.DataFlag[temple-1]);//检测值数据标识
    sample_info.insert("2061tag", this->status.DataFlag[temple-1]);//检测值数据标识
    //qDebug()<<"2: "<<__FILE__<<__func__<<endl;
    temple = ((qint16)this->buff[9])<<8;
    temple = temple + (qint16)this->buff[10];
    sample_info.insert("i11001", this->status.RunLog[temple]);//工作日志
    //qDebug()<<"3: "<<__FILE__<<__func__<<endl;
    temple = ((qint16)this->buff[11])<<8;
    temple = temple + (qint16)this->buff[12];
    sample_info.insert("i12001",this->status.DeviceStatus[temple]);//工作状态
    //qDebug()<<"4: "<<__FILE__<<__func__<<endl;
     data = DataFormatAnalysis(dataformat, this->buff+13);
    /*value1.a[0] = this->buff[14];
    value1.a[1] = this->buff[13];
    value1.a[2] = this->buff[16];
    value1.a[3] = this->buff[15];
    str = QString("%1").arg(value1.b);*/
    sample_info.insert("i12003", data);
    // qDebug()<<"5: "<<__FILE__<<__func__<<endl;
    temple = ((qint16)this->buff[17])<<8;
    temple = temple + (qint16)this->buff[18];
    sample_info.insert("i12031",this->status.WarningStatus[temple]);
   // qDebug()<<"6: "<<__FILE__<<__func__<<endl;
    QString str = "";
    for(int i = 19; i < 31; i = i+2){//超标留样时间
        temple = ((qint16)this->buff[i])<<8;
        temple = temple + (qint16)this->buff[i+1];
        if(temple < 10){
            str = str + "0" + QString::number(temple, 10);
        }
        else{
            str = str + QString::number(temple, 10);
        }
    }
    sample_info.insert("提取监测仪表时间", str);//提取监测仪表时间
    //qDebug()<<"7: "<<__FILE__<<__func__<<endl;


    temple = ((qint16)this->buff[43])<<8;
    temple = temple + (qint16)this->buff[44];
    sample_info.insert("i13001", QString::number(temple, 10));//测量量程
    //qDebug()<<"8: "<<__FILE__<<__func__<<endl;
    temple = ((qint16)this->buff[45])<<8;
    temple = temple + (qint16)this->buff[46];
    sample_info.insert("i13002", QString::number(temple, 10));//测量精度
    //qDebug()<<"9: "<<__FILE__<<__func__<<endl;
    temple = ((qint16)this->buff[47])<<8;
    temple = temple + (qint16)this->buff[48];
    sample_info.insert("i13003", QString::number(temple, 10));//测量间隔
   // qDebug()<<"10: "<<__FILE__<<__func__<<endl;
    temple = ((qint16)this->buff[49])<<8;
    temple = temple + (qint16)this->buff[50];
    sample_info.insert("i13004", QString::number(temple, 10));//消解温度
   // qDebug()<<"11: "<<__FILE__<<__func__<<endl;
    temple = ((qint16)this->buff[51])<<8;
    temple = temple + (qint16)this->buff[52];
    sample_info.insert("i13005", QString::number(temple, 10));//消解时长

    str = "";
    for(int i = 53; i < 65; i = i+2){
        temple = ((qint16)this->buff[i])<<8;
        temple = temple + (qint16)this->buff[i+1];
        if(temple < 10){
            str = str + "0" + QString::number(temple, 10);
        }
        else{
            str = str + QString::number(temple, 10);
        }

    }
    sample_info.insert("空白校准时间", str);//空白校准时间
   // qDebug()<<"12: "<<__FILE__<<__func__<<endl;
    data = DataFormatAnalysis(dataformat, this->buff+65);

    /*value1.a[0] = this->buff[66];
    value1.a[1] = this->buff[65];
    value1.a[2] = this->buff[68];
    value1.a[3] = this->buff[67];
    str = QString("%1").arg(value1.b);*/
    sample_info.insert("i13007", data);//曲线斜率
    //qDebug()<<"13: "<<__FILE__<<__func__<<endl;
    data = DataFormatAnalysis(dataformat, this->buff+69);
    /*value1.a[0] = this->buff[70];
    value1.a[1] = this->buff[69];
    value1.a[2] = this->buff[72];
    value1.a[3] = this->buff[71];
    str = QString("%1").arg(value1.b);*/
    sample_info.insert("i13008", data);//曲线截矩
    //qDebug()<<"14: "<<__FILE__<<__func__<<endl;
    data = DataFormatAnalysis(dataformat, this->buff+73);
    /*value1.a[0] = this->buff[74];
    value1.a[1] = this->buff[73];
    value1.a[2] = this->buff[76];
    value1.a[3] = this->buff[75];
    str = QString("%1").arg(value1.b);*/
    sample_info.insert("i13009", data);//测量检出限
    //qDebug()<<"15: "<<__FILE__<<__func__<<endl;
    data = DataFormatAnalysis(dataformat, this->buff+77);
   /* value1.a[0] = this->buff[78];
    value1.a[1] = this->buff[77];
    value1.a[2] = this->buff[80];
    value1.a[3] = this->buff[79];
    str = QString("%1").arg(value1.b);*/
    sample_info.insert("i13010", data);//测量信号值
    //qDebug()<<"16: "<<__FILE__<<__func__<<endl;
    data = DataFormatAnalysis(dataformat, this->buff+81);
    /*value1.a[0] = this->buff[82];
    value1.a[1] = this->buff[81];
    value1.a[2] = this->buff[84];
    value1.a[3] = this->buff[83];
    str = QString("%1").arg(value1.b);*/
    sample_info.insert("i13011", data);//线性相关系数
    //qDebug()<<"17: "<<__FILE__<<__func__<<endl;
    data = DataFormatAnalysis(dataformat, this->buff+85);
   /* value1.a[0] = this->buff[86];
    value1.a[1] = this->buff[85];
    value1.a[2] = this->buff[88];
    value1.a[3] = this->buff[87];
    str = QString("%1").arg(value1.b);*/
    sample_info.insert("i13012", data);//二次多项式系数
    //qDebug()<<"18: "<<__FILE__<<__func__<<endl;
    str = "";
    for(int i = 89; i < 101; i = i+2){
        temple = ((qint16)this->buff[i])<<8;
        temple = temple + (qint16)this->buff[i+1];
        if(temple < 10){
            str = str + "0" + QString::number(temple, 10);
        }
        else{
            str = str + QString::number(temple, 10);
        }
    }
    sample_info.insert("i13013", str);//标样校准时间
     //qDebug()<<"19: "<<__FILE__<<__func__<<endl;
    data = DataFormatAnalysis(dataformat, this->buff+101);
    /*value1.a[0] = this->buff[102];
    value1.a[1] = this->buff[101];
    value1.a[2] = this->buff[104];
    value1.a[3] = this->buff[103];
    str = QString("%1").arg(value1.b);*/
    sample_info.insert("核查数据", data);//核查数据
    //qDebug()<<"20: "<<__FILE__<<__func__<<endl;
    data = DataFormatAnalysis(dataformat, this->buff+105);
    /*value1.a[0] = this->buff[106];
    value1.a[1] = this->buff[105];
    value1.a[2] = this->buff[108];
    value1.a[3] = this->buff[107];
    str = QString("%1").arg(value1.b);*/
    sample_info.insert("标样标准浓度", data);
    //qDebug()<<"21: "<<__FILE__<<__func__<<endl;
    temple = ((qint16)this->buff[109])<<8;
    temple = temple + (qint16)this->buff[110];
    sample_info.insert("核查数据标识", this->status.DataFlag[temple-1]);
    //qDebug()<<"22: "<<__FILE__<<__func__<<endl;

}
//启动分析仪
void SamplerState::StartStruc(URT_PORT port, unsigned short reg_address,SERIAL_PORT_CONFIG config){
    unsigned char cmd[8];//构建取值指令
    unsigned short crc;//crc校验值
    unsigned char recvbuf[8];
    //发送第一条启动指令 COD
    cmd[0] = 0x11;
    cmd[1] = 0x06;
    cmd[2] = (unsigned char)(reg_address>>8);
    cmd[3] = (unsigned char)(reg_address&0x00ff);
    cmd[4] = 0x00;
    cmd[5] = 0x01;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);//发送指令

    /*-------------------读取数据-----------------*/
    int count = 3;
    while(count--){
        QThread::msleep(200);
        if(Single_485_Read(port, recvbuf, 8)>0)
            break;

    }
    /*-----------------------------------------------*/


    //发送第二条启动指令  总磷
    cmd[0] = 0x99;
    cmd[1] = 0x06;
    cmd[2] = (unsigned char)(reg_address>>8);
    cmd[3] = (unsigned char)(reg_address&0x00ff);
    cmd[4] = 0x00;
    cmd[5] = 0x01;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);//发送指令


    /*-------------------读取数据-----------------*/
    count = 3;
    while(count--){
        if(Single_485_Read(port, recvbuf, 8)>0)
            break;
        QThread::msleep(18);
    }
    /*-----------------------------------------------*/

    //发送第三条启动指令  总氮
    cmd[0] = 0x65;
    cmd[1] = 0x06;
    cmd[2] = (unsigned char)(reg_address>>8);
    cmd[3] = (unsigned char)(reg_address&0x00ff);
    cmd[4] = 0x00;
    cmd[5] = 0x01;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);//发送指令

    /*-------------------读取数据-----------------*/
    count = 3;
    while(count--){
        QThread::msleep(200);
        if(Single_485_Read(port, recvbuf, 8)>0)
            break;

    }
    /*-----------------------------------------------*/

    //发送第四条启动指令 氨氮
    cmd[0] = 0x60;
    cmd[1] = 0x06;
    cmd[2] = (unsigned char)(reg_address>>8);
    cmd[3] = (unsigned char)(reg_address&0x00ff);
    cmd[4] = 0x00;
    cmd[5] = 0x01;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);//发送指令

    /*-------------------读取数据-----------------*/
    count = 3;
    while(count--){
        QThread::msleep(200);
        if(Single_485_Read(port, recvbuf, 8)>0)
            break;
        //QThread::msleep(18);
    }
    /*-----------------------------------------------*/
}
//采样器启动、停止
void SamplerState::SamplerStartStop(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config,QString cmdd){
    unsigned char cmd[8];//构建取值指令
    unsigned short crc;//crc校验值
    unsigned char recvbuf[7];
    //cmdd = " ";
    cmd[0] = slave_address;
    cmd[1] = 0x06;
    cmd[2] = 0x0c;
    cmd[3] = 0x59;
    cmd[4] = 0x00;

    if(cmdd.toInt() == 1)//01为启动 02停止
    cmd[5] = 0x01;
    else
    cmd[5] = 0x02;

    //cmd[5] = 0x01;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);//发送指令

    /*-------------------读取数据-----------------*/
    /*int count = 3;
    while(count--){
        QThread::msleep(200);
        if(Single_485_Read(port, recvbuf, 7)>0)
            break;

    }*/
    /*-----------------------------------------------*/
    //qint16 temple = ((qint16)recvbuf[3])<<8;
    //qint16 temple = (qint16)recvbuf[4];
    //sample_info.insert("i42001", QString::number(temple));

}
//数采仪发给水质采样器的留样指令
void SamplerState::OverReSample(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config,QString cmdd){
    unsigned char cmd[8];//构建取值指令
    unsigned short crc;//crc校验值
    unsigned char recvbuf[8];
    //cmdd = " ";
    cmd[0] = slave_address;
    cmd[1] = 0x06;
    cmd[2] = 0x0c;
    cmd[3] = 0x58;
    cmd[4] = 0x00;
    if(cmdd.toInt()==0){
        cmd[5] = 0x00;
    }
    //if(cmdd == "无超标")//0：无超标 1：有超标


    else
        cmd[5] = 0x01;

    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);//发送指令
    int nread=0;


    int count = 3;
    while(count--){
        if((nread=Single_485_Read(port, recvbuf, 9))>0)
            break;
        QThread::msleep(200);
    }

//SamplerState::SamplerState(){

//}

//SamplerState::~SamplerState(){

//}

}

//供水
QString SamplerState::WaterSupply(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config){
    unsigned char cmd[8];//构建取值指令
    unsigned short crc;//crc校验值
    unsigned char recv[9];
    cmd[0] = slave_address;
    cmd[1] = 0x03;
    cmd[2] = (unsigned char)(reg_address>>8);
    cmd[3] = (unsigned char)(reg_address&0x00ff);

    cmd[4] = 0x00;
    //if(cmdd == "供样水未就绪")//0：供样水未就绪 1：供样水就绪
    cmd[5] = 0x01;

    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);

    clearbuff(port);
    Single_485_Write(port,cmd, sizeof (cmd), config);

    //Single_485_Read(port,cmd,sizeof (cmd));

    /*-------------------读取数据-----------------*/
    int count = 3;
    while(count--){
        QThread::msleep(200);
        if(Single_485_Read(port, recv, sizeof (recv))>=8){
            //qDebug()<<"samplerstate count= "<<count<<endl;
        break;
        }

        //QThread::sleep(2);
    }
   /* qint16 data = (qint16)cmd[4];

    sample_info.insert("供样水就绪标志",QString::number(data));
    //qDebug()<<sample_info.value("供样水就绪标志")<<endl;*/
    /*-----------------------------------------------*/
    return QString::number(cmd[4]);

/*    if(cmd[5] == 1){
        StartStruc(port, reg_address,config);//启动分析仪，连发四条
    }
    else{
         WaterSupply(port,slave_address,reg_address,config,cmdd);
    }
*/
    //return cmd[5];
}
//分析仪启动、停止指令
void SamplerState::AnalyzerStartStop(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config,QString cmdd){
    unsigned char cmd[8];//构建取值指令
    unsigned short crc;//crc校验值
    unsigned char recvbuf[9];
    //cmdd = " ";
    cmd[0] = slave_address;
    cmd[1] = 0x06;
    cmd[2] = 0x12;
    cmd[3] = 0x00;
    cmd[4] = 0x00;
/*
    if(cmdd == "启动测量")//01为启动测量 02为标样核查 10为初始化 11为停止
    cmd[5] = 0x01;
    else if(cmdd == "标样核查")
    cmd[5] = 0x02;
    else if(cmdd == "初始化")
    cmd[5] = 0x0A;
    else
    cmd[5] = 0x0B;
*/
    int intCmd = cmdd.toInt();
    switch(intCmd){ //01为启动测量 02为标样核查 03零点校准量程校准 10为初始化 11为停止
            //case "启动测量":
            case 1:
                cmd[5] = 0x01;
                break;
                //case "标样核查":
            case 2:
                cmd[5] = 0x02;
                break;
                //case "零点校准量程校准":
            case 3:
                cmd[5] = 0x03;
                break;
                //case "初始化":
            case 10:
                cmd[5] = 0x0a;
                break;
                //case "停止":
            case 11:
                cmd[5] = 0x0b;
                break;
    }
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    clearbuff(port);
    SETAWAIT(port,slave_address, config);
    clearbuff(port);
    if(cmd[5] != 0x0b)
    Single_485_Write(port,  cmd, sizeof (cmd),  config);//发送指令

    int count = 3;
    while(count--){
        if(Single_485_Read(port, recvbuf, 9)>0)
            break;
        QThread::msleep(18);
    }

}

// 2062 上传监测指标核查
bool SamplerState::MonitorIndexCheck(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config){
    unsigned char cmd[8];//构建取值指令
    unsigned short crc;//crc校验值
    cmd[0] = slave_address;
    cmd[1] = 0x03;

    cmd[2] = 0x06;
    cmd[3] = 0x0d;//寄存器地址

    cmd[4] = 0x00;
    cmd[5] = 0x05;

    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);

    Single_485_Write(port,cmd, sizeof (cmd), config);
    int nread = 0;
    int count = 3;
    while(count--){
        if((nread = Single_485_Read(port, this->buf, 16))>0){
            break;
        }

        QThread::msleep(18);


    /*int mm = Single_485_Read(port, this->buf, 16);
    while(mm > 0){
        break;*/
    }
    if(nread == 0){
        return false;
    }
    else
        return true;
}
// 2062 监测指标核查解析
void SamplerState::MonitorIndexCheck_data(DataAlign dataformat, QMap<QString, QString> &sample_info){

     QString data = DataFormatAnalysis(dataformat, this->buff+4);
    /*union chartofloat{
            unsigned char a[4];
            float b;
        };
    union chartofloat value1;
    value1.a[0] = this->buff[6];
    value1.a[1] = this->buff[7];
    value1.a[2] = this->buff[4];
    value1.a[3] = this->buff[5];
    QString str = QString("%1").arg(value1.b);*/
    this->sample_info.insert("Check", data);//核查数据

    data = DataFormatAnalysis(dataformat, this->buff+8);

    /*value1.a[0] = this->buff[10];
    value1.a[1] = this->buff[11];
    value1.a[2] = this->buff[8];
    value1.a[3] = this->buff[9];
    str = QString("%1").arg(value1.b);*/
    this->sample_info.insert("StandardValue", data);//标样标准浓度

    qint16 temple = ((qint16)this->buff[12])<<8;
    temple = temple + (qint16)this->buff[13];
    this->sample_info.insert("Flag", this->status.DataFlag[temple-1]);//核查数据标识
}

//3019 上传设备唯一标识
bool SamplerState::DevFlag(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config){
    unsigned char cmd[8]; //构建取值指令
    unsigned short crc; //crc校验值
    cmd[0] = slave_address; //从站地址
    cmd[1] = 0x03;    //功能码 读指令

    cmd[2] = 0x0d;
    cmd[3] = 0x4e; //寄存器地址

    cmd[4] = 0x00;
    cmd[5] = 0x02;

    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);

    clearbuff(port);
    Single_485_Write(port,cmd, sizeof(cmd), config);

    /*-------------------读取数据-----------------*/
    int count = 3;
    int datalen = 0;
    while(count--){
        QThread::msleep(200);
        if((datalen = Single_485_Read(port, this->buff, 10))>0)
            break;

    }
    if(datalen ==0)
        return false;
    else
        return true;
    /*-----------------------------------------------*/
}
// 3019 3019解析
void SamplerState::DevFlag_Date(DataAlign dataformat, QMap<QString, QString> &sample_info){

     QString data = DataFormatAnalysis(dataformat, this->buff+3);
    /* union chartofloat{
            unsigned char a[4];
            float b;
        };
    union chartofloat value1;
    value1.a[0] = this->buff[5];
    value1.a[1] = this->buff[6];
    value1.a[2] = this->buff[3];
    value1.a[3] = this->buff[4];
    QString str = QString("%1").arg(value1.b);*/
    this->sample_info.insert("devSerialNo", data);  //设备序列号
}

//3018 累计出样时间
bool SamplerState::SamplerTime(URT_PORT port, unsigned char slave_address, SERIAL_PORT_CONFIG config){
    unsigned char cmd[8]; //构建取值指令
    unsigned short crc; //crc校验值
    unsigned char databuff[7];
    cmd[0] = slave_address; //从站地址
    cmd[1] = 0x03;    //功能码 读指令
    cmd[2] = 0x10;
    cmd[3] = 0x8e; //寄存器地址

    cmd[4] = 0x00;
    cmd[5] = 0x03;

    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);

    clearbuff(port);
    Single_485_Write(port, cmd, sizeof (cmd), config);
    /*--------------接受数据-------------------------*/
    int count = 3;
    int nread = 0;
    while(count--){
        QThread::msleep(200);
        if((nread = Single_485_Read(port, databuff, 7))>0)
            //this->sample_info.insert("CTime",QString::number(temple,10));
            break;

    }
    if(nread==0){
        return false;
    }
    else{
        qint16 temple = ((qint16)databuff[3])<<8;
        temple = temple + (qint16)databuff[4];
        this->sample_info.insert("STime",QString::number(temple,10));
        return true;
    }
    /*--------------解析指令-------------------------*/


}
//参数说明：读写端口，来自云端的Info_Id, 来自云端的值，从站地址，端口配置信息
void SamplerState::Set_instrument_Info(URT_PORT port, QString Info_Id, QString value, unsigned char slave_address, SERIAL_PORT_CONFIG config, DataAlign dataformat)
{
    unsigned char recvbuf[10];
    //qDebug()<< InfoMap.contains("i13001")<<endl;
    if(InfoMap.contains(Info_Id)){//查找是否有该键
        unsigned short crc;//crc校验值
        struct Instrument_Info info = InfoMap.value(Info_Id);

            if(info.datatype == INT_DATA){
                unsigned char cmd[8];
                cmd[0] = slave_address;
                cmd[1] = 0x06;
                unsigned short RegAddress = (unsigned short)info.RegAddress.toInt();//将寄存器地址的字符串转化为数值
                cmd[2] = (unsigned char)(RegAddress>>8);
                cmd[3] = (unsigned char)(RegAddress&0x00ff);
                //cmd[4] = 0x00;
                //cmd[5] = 0x01;
                unsigned short int_value = (unsigned short)value.toInt();
                cmd[4] = (unsigned char)(int_value>>8);
                cmd[5] = (unsigned char)(int_value&0x00ff);
                crc = GetCrcData(cmd, 6);
                cmd[6] = (unsigned char)(crc>>8);
                cmd[7] = (unsigned char)(crc&0x00ff);
                Single_485_Write(port, cmd, sizeof (cmd), config);

                /*-------------------读取数据-----------------*/
                int count = 3;
                while(count--){
                    QThread::msleep(200);
                    if(Single_485_Read(port, recvbuf, 10)>0)
                        break;
                    //QThread::msleep(18);
                }
                /*-----------------------------------------------*/
            }
            else if(info.datatype == FLOAT_DATA){
                unsigned char cmd[13];
                cmd[0] = slave_address;
                cmd[1] = 0x10;
                unsigned short RegAddress = (unsigned short)info.RegAddress.toUInt();
                cmd[2] = (unsigned char)(RegAddress>>8);
                cmd[3] = (unsigned char)(RegAddress&0x00ff);
                cmd[4] = 0x00;
                cmd[5] = 0x02;
                cmd[6] = 0x04;
                float float_value = value.toFloat();

                DataFormatAnalysis(dataformat,cmd+7, float_value);
                /*union floattochar{
                        unsigned char a[4];
                        float b;
                    };
                union floattochar value1;//将浮点数转化为modbus格式(小端)
                value1.b = float_value;
                cmd[7] = value1.a[1];
                cmd[8] = value1.a[0];
                cmd[9] = value1.a[3];
                cmd[10] = value1.a[2];*/

                crc = GetCrcData(cmd,11);
                cmd[11] = (unsigned char)(crc>>8);
                cmd[12] = (unsigned char)(crc&0x00ff);

                clearbuff(port);
                Single_485_Write(port, cmd, sizeof (cmd), config);

                /*-------------------读取数据-----------------*/
                int count = 3;
                while(count--){
                    QThread::msleep(200);
                    if(Single_485_Read(port, recvbuf, 10)>0)
                        break;

                }
                /*-----------------------------------------------*/

            }

            else{
                unsigned char cmd[14];
                cmd[0] = slave_address;
                cmd[1] = 0x06;
                unsigned short RegAddress = (unsigned short)info.RegAddress.toInt();
                cmd[2] = (unsigned char)(RegAddress>>8);
                cmd[3] = (unsigned char)(RegAddress&0x00ff);
                cmd[4] = 0x00;
                cmd[5] = 0x06;
                 /*将年月日时分秒切割出来*/
                QString year = value.mid(0,4);
                QString month = value.mid(4,2);
                QString day = value.mid(6,2);
                QString hour = value.mid(8,2);
                QString minute = value.mid(10,2);
                QString second = value.mid(12,2);

                cmd[6] = year.toInt();
                cmd[7] = month.toInt();
                cmd[8] = day.toInt();
                cmd[9] = hour.toInt();
                cmd[10] = minute.toInt();
                cmd[11] = second.toInt();

                crc = GetCrcData(cmd,11);
                cmd[12] = (unsigned char)(crc>>8);
                cmd[13] = (unsigned char)(crc&0x00ff);
                clearbuff(port);
                Single_485_Write(port, cmd, sizeof (cmd), config);



                /*-------------------读取数据-----------------*/
                int count = 3;
                while(count--){
                    QThread::msleep(200);
                    if(Single_485_Read(port, recvbuf, 10)>0)
                        break;
                    //QThread::msleep(18);
                }
                /*-----------------------------------------------*/
            }
    }
    else{

    }
}

//3013 清洗反吹  寄存器地址425 0x01a9
void SamplerState::ClearStart(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config,QString cmdd){
    unsigned char recvbuf[10];
    unsigned char cmd[8];//构建取值指令
    unsigned short crc;//crc校验值
    //cmdd = " ";
    cmd[0] = slave_address;
    cmd[1] = 0x06;
    cmd[2] = (unsigned char)(reg_address>>8);  //0x01
    cmd[3] = (unsigned char)(reg_address&0x00ff);//0xa9
    cmd[4] = 0x00;
    cmd[5] = 0x01;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);//发送指令


    /*-------------------读取数据-----------------*/
    int count = 3;
    while(count--){
        QThread::msleep(200);
        if(Single_485_Read(port, recvbuf, 10)>0)
            break;

    }
    /*-----------------------------------------------*/
}

//2021 取设备运行状态数据
void SamplerState::SBState(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config){
    unsigned char cmd[8];//构建取值指令
    unsigned short crc;//crc校验值

    cmd[0] = slave_address;
    cmd[1] = 0x03;
    cmd[2] = (unsigned char)(reg_address>>8);  //0x05
    cmd[3] = (unsigned char)(reg_address&0x00ff);//0xe0
    cmd[4] = 0x00;
    cmd[5] = 0x01;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);//发送指令

    //while(Single_485_Read(port, this->buff,7)==0);


    /*-------------------读取数据-----------------*/
    int count = 3;
    while(count--){
        QThread::msleep(200);
        if(Single_485_Read(port, this->buff, 7)>0)
            break;

    }
    /*-----------------------------------------------*/

    qint16 temple = ((qint16)this->buff[4])<<8;
    temple = temple + (qint16)this->buff[5];
    this->sample_info.insert("i12001", this->status.DataFlag[temple]); //设备运行状态
}

bool SamplerState::GetSamplePeriodTime(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config)
{
    unsigned char cmd[8];//构建取值指令
    unsigned short crc;//crc校验值
    unsigned char recvbuff[7]={0};

    cmd[0] = slave_address;
    cmd[1] = 0x03;
    cmd[2] = (unsigned char)(reg_address>>8);
    cmd[3] = (unsigned char)(reg_address&0x00ff);
    cmd[4] = 0x00;
    cmd[5] = 0x01;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);//发送指令

    int nread=0;
    int count = 3;
    while (count--) {
        QThread::msleep(200);
        if(Single_485_Read(port, recvbuff, 7)>0)
            break;
    }
    if(nread==0){
        return false;
    }
    else
    {
        qint16 temple = ((qint16)recvbuff[3])<<8;
        temple = temple + (qint16)recvbuff[4];
        sample_info.insert("CTime",QString::number(temple));
        return true;
    }

}

void SamplerState::ControlAnalyzer(URT_PORT port, unsigned char slave_address, SERIAL_PORT_CONFIG config, QString cmdd)
{
    unsigned char cmd[8];
    unsigned short crc;

    cmd[0] = slave_address;
    cmd[1] = 0x06;
    cmd[2] = 0x06;
    cmd[3] = 0x18;
    cmd[4] = 0x00;
    switch (cmdd.toInt()) {
    case 3044:
        cmd[5] = 0x29;
        break;
    case 3085:
        cmd[5] = 0x2a;
        break;
    case 3086:
        cmd[5] = 0x2b;
        break;
    case 3080:
        cmd[5] = 0x2c;
        break;
    }
    crc = GetCrcData(cmd,6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);//发送指令

}

void SamplerState::SetSamplePeriodTime(URT_PORT port, unsigned char slave_addess, unsigned short reg_address, SERIAL_PORT_CONFIG config, QString CTime)
{
    unsigned char cmd[8];
    unsigned short crc;

    cmd[0] = slave_addess;
    cmd[1] = 0x06;
    cmd[2] = (unsigned char)(reg_address>>8);
    cmd[3] = (unsigned char)(reg_address&0x00ff);
    unsigned short ct = (unsigned short)CTime.toInt();
    cmd[4] = (unsigned char)(ct>>8);
    cmd[5] = (unsigned char)(ct&0x00ff);
    crc = GetCrcData(cmd,6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7]= (unsigned char)(crc&0x00ff);
    clearbuff(port);
    Single_485_Write(port, cmd, sizeof (cmd), config);

}

void SamplerState::SETAWAIT(URT_PORT port, unsigned char slave_address,SERIAL_PORT_CONFIG config)
{
    unsigned char cmd[8];//构建取值指令
    unsigned short crc;//crc校验值
    //cmdd = " ";
    cmd[0] = slave_address;
    cmd[1] = 0x06;
    cmd[2] = 0x12;
    cmd[3] = 0x00;
    cmd[4] = 0x00;
    cmd[5] = 0x0b;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);//发送指令
    QThread::msleep(50);
}

bool SamplerState::RS232Check(URT_PORT port, SERIAL_PORT_CONFIG config)
{
    unsigned char cmd[13] = "NITNIUBILITY";
    unsigned char recvbuff[13];
    clearbuff(port);
    Single_485_Write(port, cmd, sizeof (cmd), config);


    int nread=0;
    int count = 3;
    while (count--) {
        QThread::msleep(200);
        if((nread = Single_485_Read(port, recvbuff, sizeof (recvbuff)))>0)
            break;
    }
    if(nread==0){
        return false;
    }
    else if(strcmp((char*)cmd,(char*)recvbuff)==0)
    {
        return true;
    }
    else {
        return false;
    }


}

QString SamplerState::RS485Check(URT_PORT port, unsigned char slave_address, unsigned short reg_address, SERIAL_PORT_CONFIG config)
{
    unsigned char cmd[8];//构建取值指令
    unsigned short crc;//crc校验值
    unsigned char recvbuff[7]={0};
    //cmdd = " ";
    cmd[0] = slave_address;
    cmd[1] = 0x03;
    cmd[2] = (unsigned char)(reg_address>>8);
    cmd[3] = (unsigned char)(reg_address&0x00ff);
    cmd[4] = 0x00;
    cmd[5] = 0x01;
    crc = GetCrcData(cmd, 6);
    cmd[6] = (unsigned char)(crc>>8);
    cmd[7] = (unsigned char)(crc&0x00ff);
    clearbuff(port);
    Single_485_Write(port,  cmd, sizeof (cmd),  config);//发送指令

    int nread=0;
    int count = 3;
    while (count--) {
        QThread::msleep(200);
        if((nread = Single_485_Read(port, recvbuff, sizeof (recvbuff)))>0)
            break;
    }
    QString str="";
    for(int i=0;i<7;i++){
        str = str+QString("0x%1 ").arg(recvbuff[i],2,16,QLatin1Char('0'));
    }
    return str;
}


