#include "PLCTaskHandler2.h"

#include <QTime>
#include <QDebug>
//#include <QFile>

#include "plc.h"
#include "SamplerState.h"

#include <Headers/BLL/BFactor.h>

#include <Sources/Tools/datatype.h>


extern PLC plc;
extern int uart_fd;
extern int ads7844_fd;
PLCTaskHandler2::PLCTaskHandler2(QObject *parent){

}

PLCTaskHandler2::PLCTaskHandler2(URT_PORT p, QObject *parent):targetPort(p)
{    
}

PLCTaskHandler2::PLCTaskHandler2(const PLCTaskHandler2 &handler)
{
    this->taskQueue = handler.taskQueue;
    this->targetPort = handler.targetPort;
}


PLCTaskHandler2::~PLCTaskHandler2(){

}


/*
bool PLCTaskHandler2::requestHandler(Task task){

    this->task = task;
    bool rs = taskResolver(&(this->task));
    emit isDone(this); // 给PLC 对象发信号
    return rs;
}
*/
//以下这个功能被放在run()中完成

void PLCTaskHandler2::doWork(Task task){    
    m_lock_IsStop.lock();
    if(!mIsStop){
        m_lock_taskQueue.lock();
        if(task.req.port == this->targetPort){
//qDebug()<< __FILE__ <<", "<< __FUNCTION__<<" # code:"<< task.req.factorCode<<", port:"<<task.req.port<<endl;
            this->taskQueue.enqueue(task);
        }
//        qDebug()<< __FILE__ <<", "<< __FUNCTION__<<", After enterQueue, taskQueue Length="<<this->taskQueue.length()<<endl;
        m_lock_taskQueue.unlock();
    }
    m_lock_IsStop.unlock();
//    bool rs = taskResolver(&task);
//    if(rs)
//        emit isDone(task);
}

//接收修改端口配置消息的槽函数
void PLCTaskHandler2::changePortConfig(SERIAL_PORT_CONFIG newValue, URT_PORT port){
    this->config = newValue;
    Single_PortConfig(port, newValue);
}
//CRC
quint16 PLCTaskHandler2::CRC16(unsigned char * data, int len){
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
CMD_PACKET PLCTaskHandler2::buildPLCCmd(PLCRequest *req){
    CMD_PACKET buf;
    unsigned char stationNo = req->stationNo;   //站号
    unsigned char cmdCode = req->cmdCode;    //命令码
    quint16 plcAddr = req->plcAddr;    //PLC地址
    DataAlign dataAlign = req->dataAlign;  //大小端
    buf.data[0] = stationNo;
    buf.data[1] = cmdCode;    //功能码

    buf.data[2]=(unsigned char)(plcAddr>>8);
    buf.data[3]=(unsigned char) plcAddr;
    buf.data[4] = 0x00;
    buf.data[5] = 0x02;

    quint16 wcrc = CRC16(buf.data,6);
    buf.data[6] =wcrc;
    buf.data[7] = wcrc>>8;

    //qDebug()<<req->plcAddr<<endl;
    //qDebug()<<buf.data[0]<<endl;
    //qDebug()<<buf.data[1]<<endl;
    //qDebug()<<buf.data[2]<<endl;
    //qDebug()<<buf.data[3]<<endl;
    //qDebug()<<buf.data[4]<<endl;
    //qDebug()<<buf.data[5]<<endl;



    return buf;
}
//向PLC发送命令的函数
bool PLCTaskHandler2::sendPLCCmd(PLCRequest *req){
    URT_PORT port = req->port;
    CMD_PACKET c = buildPLCCmd(req);
    //配置波特率，以便波特率修改能及时生效

//    SERIAL_PORT_CONFIG config = plc.configs[port];
//    Single_485_Write(port, c, config);
    return true;

}

//与PLC交互的函数（向PLC发送命令，读取PLC的响应）

bool PLCTaskHandler2::taskResolver(Task *task){

    //qDebug()<< __FILE__ <<", "<< __FUNCTION__<<" # code:"<< task->req.factorCode<<", port:"<<task->req.port<<"# "<<task->req.action<<endl;
    //qDebug()<<"# "<<task->req.action<<endl;
////模拟测试用
//task->res.resultMap.insert("i11001             if(dataLen)","上电冷却消解液");
//task->res.result = true;
//return true;
//}

//task->res.result=true;
//BFactor bf;
//MFactor mf;
//bf.getFactorInfoByCode(task->req.factorCode,mf);
//task->res.singleData.v = RandomData::getRadomData(mf.warningValueMax,mf.warningValueMin);
//return true;
//以上模拟测试数据

    if(task->req.action==PLCAction::GET_VALUE){
        if (task->req.port>=0 && task->req.port<=6) // RS232/RS485
        {
            //qDebug()<<"Factor Code = "<<task->req.factorCode<<", port="<<task->req.port<<endl;


            int len = 10;
            CMD_PACKET buff = buildPLCCmd(&task->req);
//            qDebug()<<"******************************************"<<endl;
//            for(int i=0; i<8; i++){
//                qDebug()<<QString::number(buff.data[i], 16)<< " ";
//            }
//            qDebug()<<endl;
            clearbuff(task->req.port);

//qDebug()<<__FILE__<<":"<<__FUNCTION__<<":baud="<<this->config.baud<<",dataBits="<<this->config.dataBits<<",stopBits="<<this->config.stopBits<<",checkBits"<<this->config.checkBits<<endl;
            Single_485_Write(task->req.port,buff.data,8, this->config);
//            QThread::msleep(200);
//            Single_485_Read(task->req.port, buf, len);
            int count = 3, dataLen=0;
            unsigned char buf[10]={0};
            while(count--){
                QThread::msleep(200);
                if((dataLen=Single_485_Read(task->req.port, buf, sizeof (buf)))>0){
                    //qDebug()<<"samplerstate count= "<<count<<endl;
                    break;
                }
            }
//            QThread::msleep(1000);
//            dataLen=Single_485_Read(task->req.port, buf, sizeof (buf));
//            qint16 crc = buf[8];
//            crc = ((crc<<8)|buf[7]);
//                qDebug()<<"crc = ------------------------>"<< crc<<endl;
//                qDebug()<<"crc = ------------------------>"<< CRC16(buf,7)<<endl;
//            bool r = true;
//            if(crc==CRC16(buf,7))
//            {
//                qDebug()<<"crc = ------------------------?"<< crc<<endl;
//                r = true;
//            }
//            dataLen = Single_485_Read(task->req.port,buf,sizeof (buf));
//            if(dataLen==0)
//            {
//                while(count--)
//                {
//                    QThread::msleep(200);
//                    dataLen = Single_485_Read(task->req.port, buf, sizeof (buf));
//                    if(dataLen>0)
//                        break;
//                }
//            }
//            if(dataLen!=9)
//            {
//                count =3;
//                int r = dataLen;
//                while(count--)
//                {
//                    QThread::msleep(200);
//                    dataLen = Single_485_Read(task->req.port, buf+r, 9-r);
//                    r = dataLen + r;
//                    if(r==9){
//                        dataLen = r;
//                        break;
//                    }
//                }
//            }



            union dd{
                float v;
                unsigned char c[4];
            }rv;
            task->res.data[0]=buf[3];
            task->res.data[1]=buf[4];
            task->res.data[2]=buf[5];
            task->res.data[3]=buf[6];

            if (dataLen==9)
                task->res.result = true;
            else
                task->res.result = false;

            if (task->req.dataAlign == DataAlign::DA1234)
            {
                //rv.c[0] = task->res.data[0];
                //rv.c[1] = task->res.data[1];
                //rv.c[2] = task->res.data[2];
                //rv.c[3] = task->res.data[3];

                //zl
                rv.c[0] = task->res.data[3];
                rv.c[1] = task->res.data[2];
                rv.c[2] = task->res.data[1];
                rv.c[3] = task->res.data[0];
            }
            else if (task->req.dataAlign == DataAlign::DA4321)
            {
                //rv.c[0] = task->res.data[3];
                //rv.c[1] = task->res.data[2];
                //rv.c[2] = task->res.data[1];
                //rv.c[3] = task->res.data[0];

                //zl
                rv.c[0] = task->res.data[0];
                rv.c[1] = task->res.data[1];
                rv.c[2] = task->res.data[2];
                rv.c[3] = task->res.data[3];
            }
            else
            {
                rv.c[0] = task->res.data[1];
                rv.c[1] = task->res.data[0];
                rv.c[2] = task->res.data[3];
                rv.c[3] = task->res.data[2];
            }
            task->res.singleData.v = rv.v;

  //qDebug()<<task->req.factorCode<<",value="<<task->res.singleData.v<<endl;

        }
        else if(task->req.port>=7 && task->req.port<=14){  //AI
            if(ads7844_fd==-1){
                ADS7844_Open();
            }
            float result;
            int res= ADS7844_Single_Read( ADS7844_CHANNEL(task->req.port-7), &result);
            task->res.singleData.v = result;
            task->res.result = true;
            return true;
        }
    }
    else if(task->req.action==PLCAction::GET_STATUS)//获取水质采样器供水样就绪标志也可以从这里获取
    {
        bool rs;
        if(task->req.factorCode=="wqs"){ //水质采样器状态值
            //qDebug()<<"Water:"<<task->req.factorCode<<endl;
            QMap<QString, QString> resultMap;
            SamplerState samplerState;

            if(plc.StrProtocol == "ESUN")
            {
                rs = samplerState.readWQSDataInfo(task->req.port, task->req.stationNo, task->req.plcAddr, this->config, resultMap);
            }
            else if(plc.StrProtocol == "GRASP")
            {
                rs = samplerState.readWQSDataInfo_new(task->req.port, task->req.stationNo, task->req.plcAddr, this->config, resultMap);
            }
            else
            {
                rs = samplerState.readWQSDataInfo(task->req.port, task->req.stationNo, task->req.plcAddr, this->config, resultMap);
            }
          //qDebug()<<"######## wqs:  after read"<<endl;
            QString result = samplerState.WaterSupply(task->req.port,task->req.stationNo,task->req.plcAddr,this->config);
            resultMap.insert("WaterSupplyFlag",result);
            task->res.resultMap = resultMap;
            task->res.result = rs;
            //qDebug()<<"Water return: resultMap Count="<<resultMap.count()<<", WaterSupplyFlag" << resultMap.value("WaterSupplyFlag")<<endl;
            return rs;
        }
        else if(task->req.factorCode=="w01018" ||
                task->req.factorCode=="w01019" ||
                task->req.factorCode=="w21001" ||
                task->req.factorCode=="w21003" ||
                task->req.factorCode=="w21011" ){  //分析仪 Analysis Meter State
            QMap<QString, QString>resultMap;
            SamplerState samplerState;
            bool rs = samplerState.readAnalysisMeterInfo(task->req.port, task->req.stationNo, task->req.plcAddr, this->config, resultMap, task->req.dataAlign);
            task->res.resultMap = resultMap;
            task->res.result = rs;
            return rs;
        }
        else if(task->req.factorCode=="ys01"||
                task->req.factorCode=="ys02"||
                task->req.factorCode=="ys03"||
                task->req.factorCode=="ys04"||
                task->req.factorCode=="ys05"){
            SamplerState samplerState;
            //QString str = samplerState.YSENVIR(task->req.port, task->req.stationNo, task->req.plcAddr, task->req.config);
            QString str = samplerState.YSENVIR(task->req.port, task->req.stationNo, task->req.plcAddr, this->config, task->req.dataAlign);
            task->res.value = str;
            task->res.result = true;
        }
        else{
            QMap<QString, QString>resultMap;
            SamplerState samplerState;
            bool rs = samplerState.readAnalysisMeterInfo(task->req.port, task->req.stationNo, task->req.plcAddr, this->config, resultMap, task->req.dataAlign);
            task->res.resultMap = resultMap;
            task->res.result = rs;
            return rs;
        }
    }
    else if(task->req.action==PLCAction::CMD_2062){//提取监测指标核查数据 CN=2062
        bool rs = false;
        SamplerState samplerState;
        QMap<QString, QString> resultMap;
        rs = samplerState.MonitorIndex(task->req.port, task->req.stationNo, task->req.plcAddr,this->config, task->req.dataAlign, resultMap);
        task->res.resultMap = resultMap;
        task->res.result = rs;
        //emit isDone(*task);
        return rs;
    }
    else if(task->req.action==PLCAction::CMD_SAMPLING_3015){//超标留样 CN=3015
        bool rs = false;
        SamplerState samplerState;
        QMap<QString, QString> result;
        samplerState.OverReSample(task->req.port,task->req.stationNo, task->req.plcAddr, this->config, "01");  //01表示超标留样

        if(plc.StrProtocol == "ESUN")
        {
            samplerState.readWQSDataInfo(task->req.port, task->req.stationNo, task->req.plcAddr, this->config, result);
        }
        else if(plc.StrProtocol == "GRASP")
        {
            samplerState.readWQSDataInfo_new(task->req.port, task->req.stationNo, task->req.plcAddr, this->config, result);
        }
        else
        {
            samplerState.readWQSDataInfo(task->req.port, task->req.stationNo, task->req.plcAddr, this->config, result);
        }

        task->dataPacket.data.CP.VaseNo = result.value("i430xx_num");
        task->res.result = rs;
        //emit isDone(*task);
        return true;  //true 确保在run方法中向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::CMD_SAMPLING_3015_CONTROL){//超标留样反控 （来自UI）
        bool rs = false;
        SamplerState samplerState;
        QMap<QString, QString> result;
        samplerState.OverReSample(task->req.port,task->req.stationNo, task->req.plcAddr, this->config, "01");  //01表示超标留样

        task->res.result = rs;
        //emit isDone(*task);
        return false;  //false 确保在run方法中  不向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::SET_Sampling_Period){   //设置采样周期  CN=3016
        bool rs = true;
        SamplerState samplerState;
        samplerState.SetSamplePeriodTime(task->req.port, task->req.stationNo, task->req.plcAddr,this->config,task->dataPacket.data.CP.Ctime);
        task->res.result = rs;
        return false;  //false 确保在run方法中 不向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::GET_Sampling_Period){//提取采样周期  CN=3017
        bool rs = true;
        SamplerState samplerState;

        rs = samplerState.GetSamplePeriodTime(task->req.port, task->req.stationNo, task->req.plcAddr, this->config);
//        QString CTime = samplerState.sample_info.value("CTime");

//        task->dataPacket.data.CP.Ctime = CTime;
//        task->taskSource = TaskDirection::CLOUD_REQUEST;
        task->res.resultMap = samplerState.sample_info;
        //emit isDone(*task);
        return true;  //true 确保在run方法中向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::Sampler_Time){//上传出样时间  CN=3018
        bool rs =true;
        SamplerState samplerState;

        rs = samplerState.SamplerTime(task->req.port, task->req.stationNo, this->config);
        task->res.resultMap = samplerState.sample_info;

        return true; //true 确保在run方法中向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::GET_DEV_FLAG){//上传提取设备唯一标识  CN=3019
        bool rs = true;
        SamplerState samplerState;
        QMap<QString, QString> resultMap;
        rs = samplerState.dev(task->req.port, task->req.stationNo, task->req.plcAddr, this->config, task->req.dataAlign,resultMap);
//        QString devSNo = resultMap.value("devSerialNo");
//        task->dataPacket.data.CP.SN=devSNo;
        task->res.resultMap = resultMap;

        //task->taskSource = TaskDirection::CLOUD_REQUEST;
        //emit isDone(*task);
        return true;//true 确保在run方法中向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::GET_INSTRUMENT_INFO){//上传现场机信息  CN=3020
        bool rs = true;
        QMap<QString, QString> resultMap;
        SamplerState samplerState;

        samplerState.StateExtract(task->req.port, task->req.stationNo, task->req.plcAddr, this->config);
        samplerState.Get_StateExtract_Info(resultMap, task->req.dataAlign);

        task->res.resultMap = resultMap;
        //emit isDone(*task);

        return true;//true 确保在run方法中向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::SET_INSTRUMENT_INFO){//设置现场机信息  CN=3021
        bool rs = true;
        SamplerState samplerState;

        QString InfoId = task->dataPacket.data.CP.infoDataList[0].InfoId;
        QString Info = task->dataPacket.data.CP.infoDataList[0].Info;
        //rs = samplerState.Set_instrument_Info(task->req.port, task->dataPacket.data.InfoId, QString value, QString salve_address, SERIAL_PORT_CONFIG config, DataAlign dataformat);
        samplerState.Set_instrument_Info(task->req.port, InfoId, Info,  task->req.stationNo, this->config, task->req.dataAlign);

        return false; //false 确保在run方法中 不向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::CMD_SAMPLER_STOP){//采样器停止  与云端无关
        bool rs = true;
        SamplerState samplerState;
        samplerState.SamplerStartStop(task->req.port, task->req.stationNo, task->req.plcAddr, this->config, "02");
        return false; //false 确保在run方法中 不向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::CMD_SAMPLER_START){//采样器启动  与云端无关
        bool rs = true;
        SamplerState samplerState;
        samplerState.SamplerStartStop(task->req.port, task->req.stationNo, task->req.plcAddr, this->config, "01");
        return false;  //false 确保在run方法中 不向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::CMD_3044){//远程启动系统单次测试  CN=3044
        bool rs = true;
        SamplerState samplerState;
        samplerState.ControlAnalyzer(task->req.port, task->req.stationNo, this->config, "3044");
        return false;  //false 确保在run方法中 不向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::CMD_3080){//远程启动单台仪表标液核查  CN=3080
        bool rs = false;
        SamplerState samplerState;
        samplerState.ControlAnalyzer(task->req.port, task->req.stationNo, this->config, "3080");
        task->res.result = rs;
        return rs;   //false 确保在run方法中 不向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::CMD_3085){//远程启动单台仪表 空白校准  CN=3085
        bool rs = false;
        SamplerState samplerState;
        samplerState.ControlAnalyzer(task->req.port, task->req.stationNo, this->config, "3085");
        task->res.result = rs;
        return rs;  //false 确保在run方法中 不向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::CMD_3086){//远程启动单台仪表标样核准  CN=3086
        bool rs = false;
        SamplerState samplerState;
        samplerState.ControlAnalyzer(task->req.port, task->req.stationNo, this->config, "3086");
        return false;   //false 确保在run方法中 不向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::CMD_ANALYZER_START){  //启动动作 (与云端无关，来自UI)
        bool rs = false;
        URT_PORT port = task->req.port;
        SERIAL_PORT_CONFIG config =this->config;
        SamplerState samplerState;

        samplerState.AnalyzerStartStop(port, task->req.stationNo, task->req.plcAddr, config, "01");  //01表示启动分析仪
        task->res.result = rs;
        return false;   //false 确保在run方法中 不向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::CMD_ANALYZER_STOP){  //停止动作 (与云端无关，来自UI)
        bool rs = false;
        URT_PORT port = task->req.port;
        SERIAL_PORT_CONFIG config =this->config;
        SamplerState samplerState;

        samplerState.AnalyzerStartStop(port, task->req.stationNo, task->req.plcAddr, config, "11");  //11表示停止分析仪
        task->res.result = rs;
        return false;   //false 确保在run方法中 不向云端或UI发isDone(Task)信号
    }

    else if(task->req.action==PLCAction::CMD_CHECK){  //分析仪标样核查 (与云端无关，来自UI)
        bool rs = false;
        URT_PORT port = task->req.port;
        SERIAL_PORT_CONFIG config =this->config;
        SamplerState samplerState;

        samplerState.AnalyzerStartStop(port, task->req.stationNo, task->req.plcAddr, config, "02");  //02表示标样核查
        task->res.result = rs;
        return false;  //false 确保在run方法中 不向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::SET_PLC_TIME){//设置PLC时间 CN=1012
        bool rs =false;
        URT_PORT port = task->req.port;
        SERIAL_PORT_CONFIG config = this->config;

        PLC_Time_Set(port,task->req.stationNo,task->req.plcAddr,(unsigned char*)task->dataPacket.data.CP.SystemTime.toLatin1().data(),config);
        task->res.result = rs;
        return false;

    }
    else if(task->req.action==PLCAction::CMD_INITA){  //分析仪初始化 (与云端无关，来自UI)
        bool rs = false;
        URT_PORT port = task->req.port;
        SERIAL_PORT_CONFIG config =this->config;
        SamplerState samplerState;

        samplerState.AnalyzerStartStop(port, task->req.stationNo, task->req.plcAddr, config, "10");  //02表示分析仪初始化
        task->res.result = rs;
        return false;  //false 确保在run方法中 不向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::CMD_WARNING_REVERSE){//告警(反向)
//        bool rs = false;
//        SamplerState samplerState;
//        RELAYER_CHANNEL ch1 = task->req.ch1;
//        RELAYER_CHANNEL ch2 = task->req.ch2;
//        if(task->req.logic==RELAYER_LOGIC::POSITIVE){//正逻辑
//            Single_Off(ch1);
//            Single_Off(ch2);
//            sleep(3);
//            Single_On(ch2);
//        }
//        else{//反逻辑
//            Single_On(ch1);
//            Single_On(ch2);
//            sleep(3);
//            Single_Off(ch2);
//        }
//        task->res.result =rs;

        bool rs = false;
        SamplerState samplerState;
        RELAYER_CHANNEL ch1 = task->req.ch1;//告警
        RELAYER_CHANNEL ch2 = task->req.ch2;//计数
        bool r = RelayControl::RelayCountAndAlarm(QDateTime::currentDateTime().toString("yyyyMMddHHmmss"),ch1,ch2);

        task->res.result =rs;

        return false;  //false 确保在run方法中 不向云端或UI发isDone(Task)信号
    }
    else if(task->req.action==PLCAction::CMD_WARNING_RESET){//告警(还原)
        bool rs = false;
        SamplerState samplerState;
        RELAYER_CHANNEL ch1 = task->req.ch1;
        RELAYER_CHANNEL ch2 = task->req.ch2;
//        if(task->req.logic==RELAYER_LOGIC::POSITIVE){//正逻辑
//            Single_Off(ch1);
//            Single_Off(ch2);
//        }
//        else{//反逻辑
            Single_Off(ch1);
            Single_Off(ch2);
            RelayControl::historytime = "";
//       }
        task->res.result =rs;
        return false;  //false 确保在run方法中 不向云端或UI发isDone(Task)信号
    }
}


void PLCTaskHandler2::run(){
    while(true){
         m_lock_IsStop.lock();
         bool shouldStop = mIsStop;
         m_lock_IsStop.unlock();
        if(shouldStop){
            m_lock_taskQueue.lock();
            this->taskQueue.clear();
            m_lock_taskQueue.unlock();
            break;
        }

        m_lock_taskQueue.lock();
        bool ok = !this->taskQueue.isEmpty();
        m_lock_taskQueue.unlock();
        if( ok){
            m_lock_taskQueue.lock();
            this->latestTask = this->taskQueue.dequeue();
            m_lock_taskQueue.unlock();
            bool rs = taskResolver(&(this->latestTask));
//qDebug()<<this->latestTask.req.factorCode<<": taskResolver return = "<<rs<<endl;
            if(rs){
                //qDebug()<<"PLCTaskHandler2::run()====>isDone: "<<endl;
                emit isDone(this->latestTask);
            }
        }
        msleep(10);
    }
//    bool rs = taskResolver(&task);

//    if(rs){
//        //发送信号，向相应的调用者的槽函数传递task对象
//        emit isDone(task);
//        // qDebug()<<"PLCTaskHandler2::run()====>isDone: "<<endl;
//    }

}

void PLCTaskHandler2::stopRunning()
{
    m_lock_IsStop.lock();
    this->mIsStop = true;
    m_lock_IsStop.unlock();
}

bool PLCTaskHandler2::readSingleData(Task *task){//读取单个的值
    int count=1;
    do{
        sendPLCCmd(&task->req);

        QThread::msleep(50);
        int c = Single_485_Read(task->req.port, task->res.answer, 9);

        if( (unsigned char)(task->res.answer[0])== task->req.stationNo){

            task->res.answerLen = 9;
            task->res.dataLen = 4;
            for(int i=0; i<task->res.dataLen;i++)
                task->res.data[i] = task->res.answer[i+3];

            task->res.crcData[0] = task->res.answer[task->res.answerLen-2];
            task->res.crcData[1] = task->res.answer[task->res.answerLen-1];

            task->res.result = true;
            return true;
        }

    }while(++count<=task->req.repeats);
    task->res.result = false;
    return true;
}
