#include "RelayControl.h"
QString RelayControl::historytime = "";

RelayControl::RelayControl()
{

}

bool RelayControl::RelayCountAndAlarm(QString time,RELAYER_CHANNEL alarm_ch, RELAYER_CHANNEL count_ch)
{
    QDateTime time1 = QDateTime::fromString(time,"yyyyMMddhhmmss");
    QDateTime time2 = QDateTime::fromString(historytime,"yyyyMMddhhmmss");

    //如果是第一次触发告警信号或者告警超过一小时
    if(historytime==""||time2.secsTo(time1)>3600){
        //执行告警和计数（控制继电器）
        //qDebug()<<"++++++++++++++"<<time2.secsTo(time1)<<endl;
        if(historytime==""){
            Single_On(alarm_ch);
            //RelayControl::relaystatus[(int)alarm_ch-1] = 1;
            //qDebug()<<"######"<<__FILE__<<__func__<<endl;
        }
        if(get_RelayStatus(alarm_ch)==0)//界面上已经处理上一次的超标下一个小时出现新的超标告警
        Single_On(alarm_ch);
        Single_On(count_ch);
        //RelayControl::relaystatus[(int)count_ch-1] = 1;
        QThread::sleep(3);
        //qDebug()<<"----------"<<__FILE__<<__func__<<endl;
        Single_Off(count_ch);
        //RelayControl::relaystatus[(int)count_ch-1] = 0;
        historytime = time;
        return true;
    }
    else{
        if(get_RelayStatus(alarm_ch)==0)
        Single_On(alarm_ch);
        return false;
    }
}
