#include "PLCTaskHandler.h"

extern int uart_fd[4];

PLCTaskHandler::PLCTaskHandler(){

}
PLCTaskHandler::PLCTaskHandler(SERIAL_PORT_CONFIG config[4])
{
    mIsStop = false;
    this->configs[0] = config[0];
    this->configs[1] = config[1];
    this->configs[2] = config[2];
    this->configs[3] = config[3];

    if((uart_fd[0]=Single_Port_OpenAndConfig(PORT0,config[0]))<0){   //端口0
        perror("open_port error");
        //return -1;
    }
    if((uart_fd[1]=Single_Port_OpenAndConfig(PORT1,config[0]))<0){   //端口1
        perror("open_port error");
        //return -1;
    }
    if((uart_fd[2]=Single_Port_OpenAndConfig(PORT2,config[0]))<0){   //端口2
        perror("open_port error");
        //return -1;
    }
    if((uart_fd[3]=Single_Port_OpenAndConfig(PORT3,config[0]))<0){   //端口3
        perror("open_port error");
        //return -1;
    }
}
PLCTaskHandler::~PLCTaskHandler(){
    QMutexLocker m_lock(&mqIsStop);
    mIsStop = true;
}
//接收PLC数据请求信号，返回PLC数据的槽函数
bool PLCTaskHandler::requestHandler(Task task){
    QMutexLocker m_lock(&mqTaskQueue);
    this->taskQueue.enqueue(task);
    return true;
}
//接收修改端口配置消息的槽函数
void PLCTaskHandler::changePortConfig(SERIAL_PORT_CONFIG newValue, URT_PORT port){
    this->configs[port] = newValue;
}
//CRC
quint16 PLCTaskHandler::CRC16(unsigned char * data, int len){
    quint16 wcrc=0XFFFF;//预置16位crc寄存器，初值全部为1
    quint8 temp;
    int i=0,j=0;
    for(i=0; i<len; i++){
        temp = data[i];
        wcrc ^=temp;
        for(j=0;j<8;j++){
            if(wcrc&0X0001){
                wcrc>>=1;
                wcrc ^=0XA001;
            }
            else
                wcrc >>=1;//直接移出
        }
    }
    temp = wcrc;
    return wcrc;
}
//构建待发送的PLC命令
CMD_PACKET PLCTaskHandler::buildPLCCmd(PLCRequest *req){
    CMD_PACKET buf;
    unsigned char stationNo = req->stationNo;   //站号
    QString cmdCode = req->cmdCode;    //命令码
    quint16 plcAddr = req->plcAddr;    //PLC地址
    DataAlign dataAlign = req->dataAlign;  //大小端
    buf.data[0] = stationNo;
    buf.data[1] = 0x03;    //功能码
    if(dataAlign==BIG){//如果是大端
        buf.data[2]=(unsigned char)plcAddr;  //低8位
        buf.data[3]=(unsigned char)plcAddr>>8;   //高8位
    }
    else{
        buf.data[3]=(unsigned char)plcAddr;
        buf.data[2]=(unsigned char)plcAddr>>8;
    }
    buf.data[4] = 0x00;
    buf.data[5] = 0x02;
    if(dataAlign==DataAlign::BIG){
        quint16 wcrc = CRC16(buf.data,6);
        buf.data[7] =wcrc;
        buf.data[6] = wcrc>>8;
    }
    else{
        quint16 wcrc = CRC16(buf.data,6);
        buf.data[6] =wcrc;
        buf.data[7] = wcrc>>8;
    }

    return buf;
}
//向PLC发送命令的函数
bool PLCTaskHandler::sendPLCCmd(PLCRequest *req){
    URT_PORT port = req->port;
    CMD_PACKET c = buildPLCCmd(req);
    //配置波特率，以便波特率修改能及时生效
    //changePortConfig(configs[port], port);

    Single_Port_Write(port, c);  //向PLC发送命令
    return true;

}
//与PLC交互的函数（向PLC发送命令，读取PLC的响应）

bool PLCTaskHandler::taskResolver(Task *task){
    PLCResponse res = task->res;
    res.value="Task Completed !";
    res.result = true;
    task->res = res;
    //================
    bool result = true;
    sendPLCCmd(&task->req);

    msleep(100);  //等待PLC响应

    //接收PLC的返回
    int c = 0; int count=3;
    while(count--){
        //count++;
        c = Single_Port_Read(task->req.port, task->res.answer, 3);//先接收前三个字节，通过第三个字节获取数据长度
        if(count>0)
            break;
        msleep(100);
    }

    Single_Port_Read(task->req.port, task->res.answer+3, int(task->res.answer[2])+2);//接收数据长度+CRC的两个字节校验码
    task->res.answerLen = int(task->res.answer[2])+2+3;
    task->res.dataLen = int(task->res.answer[2]);
    for(int i=0; i<task->res.dataLen;i++)
        task->res.data[i] = task->res.answer[i+3];

    task->res.crcData[0] = task->res.answer[task->res.answerLen-2];
    task->res.crcData[1] = task->res.answer[task->res.answerLen-1];

    return result;
}
void PLCTaskHandler::run(){
    while(!mIsStop){
        msleep(100);   //休眠 100 毫秒
        QMutexLocker locker1(&(this->mqTaskQueue));
        if (!(this->taskQueue.isEmpty()))
        {
            Task task = this->taskQueue.dequeue();

            //处理请求，向PLC发送命令，将PLC返回信息封装在PLCRespose类型的res指针变量中

            bool rs = taskResolver(&task);

            if(rs){
                //this->responseQueue.enqueue(&res);    //将响应入列
                //发送信号，向相应的调用者的槽函数传递task对象
                if(task.taskSource==TaskDirection::UI_REQUEST)
                    emit UITaskDone(task);
                else if(task.taskSource==TaskDirection::CLOUD_REQUEST){
                    emit CloudTaskDone(task);
                }
                else if(task.taskSource == TaskDirection::SELF_REPORT){
                    emit SelfTaskDone(task);
                }

            }
        }
    }
}
