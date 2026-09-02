
#include "plc.h"

#include <Sources/Tools/datatype.h>

#include <Headers/BLL/BFactor.h>


PLC::PLC()
{
    configs[0].baud=BD9600;
    configs[0].startBits=START_ONE;
    configs[0].dataBits =DATA_EIGHT;
    configs[0].checkBits=PARITY_CHECK::NONE;
    configs[0].stopBits=STOP_ONE;
    configs[1] = configs[2] = configs[3] = configs[4] = configs[5]=configs[6] = configs[0];

    //Single_Port_OpenAndConfig(URT_PORT::PORT0,configs[0]);

    Single_Port_OpenAndConfig(URT_PORT::PORT1,configs[1]);
    Single_Port_OpenAndConfig(URT_PORT::PORT2,configs[2]);
    Single_Port_OpenAndConfig(URT_PORT::PORT3,configs[3]);
    Single_Port_OpenAndConfig(URT_PORT::PORT4,configs[4]);
    Single_Port_OpenAndConfig(URT_PORT::PORT5,configs[5]);
    Single_Port_OpenAndConfig(URT_PORT::PORT6,configs[6]);

    OpenRelayer();

    for(int i=0; i<16; i++){
        connect(&plcHandlers[i], SIGNAL(isDone(Task)), this, SLOT(receivePlcResult(Task)));
        connect(this, &PLC::isRequestPLC, &plcHandlers[i], &PLCTaskHandler2::doWork);//入队
        plcHandlers[i].start();
    }
}

PLC::~PLC()
{
    //qDebug()<<"~PLC() was called !"<<endl;
    for(int i=0; i<16; i++){
        plcHandlers[i].stopRunning();
        plcHandlers[i].quit();
        plcHandlers[i].wait();
    }
    All_Port_Close();
}

// 接收UI信号的槽
bool PLC::requestHandler(Task task)
{

    // 测试代码：生成随机数据
//        task.res.result=true;
//        BFactor bf;
//        MFactor mf;
//        bf.getFactorInfoByCode(task.req.factorCode,mf);
//        if (mf.flag==0)
//           task.res.singleData.v = RandomData::getRadomData(mf.warningValueMax,mf.warningValueMin);
//        else
//           task.res.singleData.v = RandomData::getRadomData(10,5);

//        //qDebug()<<"Code="<<task.req.factorCode<<", value="<<task.res.singleData.v<<endl;

//        emit(isDone(task));
    // //////////////////////////////////////

//    PLCTaskHandler2 *plcTaskHandler2 = new PLCTaskHandler2(this);
//    plcTaskHandler2->task = task;
//    connect(plcTaskHandler2, SIGNAL(isDone(Task)), this, SLOT(receivePlcResult(Task)));
//    connect(plcTaskHandler2, &QThread::finished, plcTaskHandler2, &QObject::deleteLater);

    //qDebug()<< __FILE__ <<", "<< __FUNCTION__<<" # code:"<< task.req.factorCode<<", port:"<<task.req.port<<", Action: "<<task.req.action<<endl;

    // 正式版本，要解开下面的注释
    emit(isRequestPLC(task));

/**********************************************************************************************************************/



    //启动线程
//    plcTaskHandler2->start();


}

bool PLC::receivePlcResult(Task task)
{
    if(task.taskSource==TaskDirection::UI_REQUEST)    //来自UI请求
        emit isDone(task);  //向 UI 发
    else if(task.taskSource == TaskDirection::CLOUD_REQUEST)  //来自云端请求
        emit isCloudRequestDone(task);   //向CloudTalker发
    else
        emit isSelfReportDone(task);    //向Worker 发
    return true;
}


void PLC::changePortConfig(SERIAL_PORT_CONFIG newValue, URT_PORT port)
{
    configs[port] = newValue;
    Single_PortConfig(port, newValue);
    plcHandlers[port].changePortConfig(newValue, port);
}
