#include "PackingTools.h"

PackingTools::PackingTools(QObject *parent) : QObject(parent)
{

}
//参数，接收云端字符串msg（包含包头2字符、数据段长度4字符、包尾巴2字符）
QMap<QString, QString> PackingTools::Split(QString pack)
{
    QMap<QString, QString> datamap;
    int poschar = 0;
    int posnum = 0;
    QRegExp spitchar("(\\w{6}-\\w{3,4})|[a-zA-Z]{2,20}(?==[^&])");//提取key的正则表达式
    QRegExp spitnum("[\\w|(\\d.)|\\s]+(?=[;|,|&])");//提取value的正则表达式

    while((poschar = spitchar.indexIn(pack, poschar))!= -1 && ((posnum = spitnum.indexIn(pack, posnum)))!= -1){
        datamap.insert(spitchar.cap(0), spitnum.cap(0));//插入key和value
         posnum += spitnum.matchedLength();
         poschar += spitchar.matchedLength();
    }
    QMap<QString, QString>::const_iterator i;
    for(i=datamap.constBegin(); i!=datamap.constEnd(); i++){
        //qDebug() <<i.key()<<":"<<i.value()<<endl;
    }
    return datamap;
}

CloudDataPacket PackingTools::CloudDataPacketBuilder(QString strCloudDataPacket)
{
    QString strHeader = strCloudDataPacket.mid(0,2);    //包头
    QString strLen = strCloudDataPacket.mid(2,4);       //数据段长度
    int intLen = strLen.toInt();
    QString strData = strCloudDataPacket.mid(6,intLen); //数据段
    QString strCRC = strCloudDataPacket.mid(intLen+6, 4);   //CRC
    //QString strTail = strCloudDataPacket.mid(intLen+10,2);  //包尾
    QString strTail = "\r\n";
    CloudDataPacket cdp;
    cdp.header = strHeader;
    cdp.len = strLen;
    cdp.crc = strCRC;
    cdp.tail = strTail;
    //构建DataPacketData
    DataPacketData dpd = DataPacketDataBuilder(strData);
    cdp.data = dpd;
    return cdp;

}
//
DataPacketData PackingTools::DataPacketDataBuilder(QString strDataPacketData){
    QMap<QString, QString> dataMap = Split(strDataPacketData);
    DataPacketData dpd;
    dpd.QN = dataMap.value("QN", "");
    dpd.ST = dataMap.value("ST", "");
    dpd.CN = dataMap.value("CN", "");
    dpd.PW = dataMap.value("PW", "");
    dpd.MN = dataMap.value("MN", "");
    dpd.Flag = dataMap.value("Flag", "");
    dpd.PNUM = dataMap.value("PNUM", "");
    dpd.PNO = dataMap.value("PNO", "");
    dpd.PolId = dataMap.value("PolId", "");  //在线监控（监测）仪器仪表对应污染物编码
    dpd.InfoId = dataMap.value("InfoId","");

    //dpd.CP;
    QString strCmdParas = dataMap.value("CP", "");
    CmdParas cmdParas = CmdParasBuilder(dataMap);
    dpd.CP = cmdParas;
    return dpd;
}
//根据CP内容字符串构建CmdParas
CmdParas PackingTools::CmdParasBuilder(QMap<QString, QString> dataMap){
    //QMap<QString, QString> dataMap = Split(strCmdParas);
//    QString MN = dataMap.value("MN", "");
    CmdParas cmdParas;
    cmdParas.SystemTime = dataMap.value("SystemTime", "");
    cmdParas.QnRtn = dataMap.value("QnRtn","");
    cmdParas.ExeRtn = dataMap.value("ExeRtn","");
    cmdParas.RtdInterval = dataMap.value("RtdInterval","");
    cmdParas.MinInterval = dataMap.value("MinInterval","");
    cmdParas.RestartTime = dataMap.value("RestartTime","");
    cmdParas.BeginTime = dataMap.value("BeginTime","");
    cmdParas.EndTime = dataMap.value("EndTime","");
    cmdParas.DataTime = dataMap.value("DataTime","");
    cmdParas.NewPW = dataMap.value("NewPW","");
    cmdParas.OverTime = dataMap.value("OverTime","");
    cmdParas.ReCount = dataMap.value("ReCount","");
    cmdParas.VaseNo = dataMap.value("VaseNo","");
    cmdParas.CstartTime = dataMap.value("CstartTime","");
    cmdParas.Ctime = dataMap.value("Ctime","");
    cmdParas.Stime = dataMap.value("Stime","");

    //查找所有PolId
    //1. 所有键放入一个QList中
    QList<QString> keyList = dataMap.keys();
    //1. 查找所有含'-'的键，将键中'-'左边内容放入一个QSet中(防止重复)，得到所有污染因子的集合
    QSet<QString> polIdSet;   //所有污染因子
    QSet<QString> sbSet;      //所有设备号（上报时可能有多个设备号，对应SBxxx-RT和SBxxxRS）
    QSet<QString> infoSet;    //所有现场端信息编码
    for (int i = 0; i < keyList.size(); ++i){
        if(keyList.at(i).contains('-')){
            QStringList list = keyList.at(i).split('-');
            if(list[1]=="PolId")
                polIdSet.insert(list[0]);
            if((list[1]=="RS"||list[1]=="RT")&&list[0].indexOf("SB")==0){
                sbSet.insert(list[0]);   //存放的是类似SBxxx-RT中的SBxxx
            }
            if(list[1]=="Info"){
                infoSet.insert(list[0]);
            }
        }
    }
    //2.针对每个设备号InfoId,生成一个InfoData对象，并存入infoDataList中
    foreach(const QString &infoId, infoSet){//infoSet存放的是类似xxxxxx-Info中的xxxxxx
        QString id = infoId.mid(0,6);    // id 是xxxxxx-Info之中的xxxxxx
        QString info = dataMap.value(id+"-"+"Info", "");

        InfoData infoData(id, info);
        cmdParas.infoDataList.append(infoData);
    }

    //3.针对每个设备号SBId,生成一个SBData对象，并存入sbDataList中
    foreach(const QString &sbId, sbSet){//sbSet存放的是类似SBxxx-RT中的SBxxx
        QString sbRT = dataMap.value(sbId+"-"+"RT", "");
        QString sbRS = dataMap.value(sbId+"-"+"RS", "");
        QString id = sbId.mid(2,3);    // id 是SBxxx之中的xxx
        SBData sbData(sbRT, sbRS, id);
        cmdParas.sbDataList.append(sbData);
    }


    //4. 对应每个污染因子，生成一个PolData对象，并存入到polDataList中
    foreach (const QString &polId, polIdSet){
         //qDebug() << polId <<endl;
         PolData polData;
         polData.Avg = dataMap.value(polId+"-"+"Avg", "");
         polData.Max = dataMap.value(polId+"-"+"Max", "");
         polData.Min = dataMap.value(polId+"-"+"Min", "");
         polData.Rtd = dataMap.value(polId+"-"+"Rtd", "");
         polData.Data = dataMap.value(polId+"-"+"Data", "");

         polData.Flag = dataMap.value(polId+"-"+"Flag", "");
         polData.EFlag = dataMap.value(polId+"-"+"EFlag", "");
         polData.PolId = polId;
         polData.ZsAvg = dataMap.value(polId+"-"+"ZsAvg", "");
         polData.ZsMax = dataMap.value(polId+"-"+"ZsMax", "");

         polData.ZsMin = dataMap.value(polId+"-"+"ZsMin", "");
         polData.ZsRtd = dataMap.value(polId+"-"+"ZsRtd", "");
         polData.Cou  = dataMap.value(polId+"-"+"Cou", "");
         polData.DayData = dataMap.value(polId+"-"+"DayData", "");
         polData.NightData = dataMap.value(polId+"-"+"NightData", "");

         polData.SampleTime = dataMap.value(polId+"-"+"SampleTime", "");
         polData.DataTime = dataMap.value(polId+"-"+"DataTime", "");
         polData.Check = dataMap.value(polId+"-"+"Check", "");
         polData.StandardValue = dataMap.value(polId +"-"+"StandardValue", "");
         polData.SN = dataMap.value(polId +"-"+"SN", "");

         cmdParas.polDataList.append(polData);
    }

    return cmdParas;
}
//根据一个DataPacketData构建一个发往云端的命令串，调用者先判断isOk，然后决定是否采用该函数的返回结果（发往云端的字符串）
QString PackingTools::CloudCmdBuilder(DataPacketData dpd, bool *isOk)
{
    *isOk = true;
    QString strCmd="##";
    QString strCPData = "";
    QString strDataLen;
    QString strData= dpd.SelfReportCmd;

    if(strData.length()<=950 ){

        *isOk = true;
    }
    else{
        *isOk = false;
    }
    strDataLen = QString::number(10000+strData.length()).mid(1,4);
    quint16 crc = CRC16_Checkout((unsigned char*)(strData.toLatin1().data()), strData.length());
    QString strCRC = QString::number(crc,16).toUpper();
    int len = strCRC.length();
    for(int i=len; i< 4; i++){
        strCRC = "0" + strCRC;
    }
    strCmd = strCmd + strDataLen + strData + strCRC + "\r\n";
    return strCmd;

}
//根据一个DataPacketData对应字符串构建一个发往云端的命令串，调用者先判断isOk，然后决定是否采用该函数的返回结果（发往云端的字符串）
QString PackingTools::CloudCmdBuilder(QString dataPacketDataString, bool *isOk)
{
    *isOk = true;
    QString strCmd="##";
    QString strDataLen;
    QString strData=dataPacketDataString;

    if(strData.length()<=950){

        *isOk = true;
    }
    else{
        *isOk = false;
    }

    strDataLen = QString::number(10000+strData.length()).mid(1,4);
    quint16 crc = CRC16_Checkout((unsigned char*)(strData.toLatin1().data()), strData.length());
    QString strCRC = QString::number(crc,16).toUpper();
    int len = strCRC.length();
    for(int i=len; i< 4; i++){
        strCRC = "0" + strCRC;
    }

    strCmd = strCmd + strDataLen + strData + strCRC + "\r\n";
    return strCmd;
}
PolData PackingTools::StrToPolData(QString str)
{
    DataPacketData dpd = DataPacketDataBuilder(str);

    PolData polData = dpd.CP.polDataList[0];
    return polData;
}

SBData PackingTools::StrToSBData(QString str)
{
    DataPacketData dpd = DataPacketDataBuilder(str);

    SBData sbData = dpd.CP.sbDataList[0];
    return sbData;
}

InfoData PackingTools::StrToInfoData(QString str)
{
    DataPacketData dpd = DataPacketDataBuilder(str);

    InfoData infoData = dpd.CP.infoDataList[0];
    return infoData;
}
//CRC
quint16 PackingTools::CRC16(unsigned char * data, int len){//modubus
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

unsigned int PackingTools::CRC16_Checkout ( unsigned char *puchMsg, unsigned int usDataLen ) //Cloud CRC
{
    unsigned int i,j,crc_reg,check;
    crc_reg = 0xFFFF;
    for(i=0;i<usDataLen;i++)
    {
        crc_reg = (crc_reg>>8) ^ puchMsg[i];
        for(j=0;j<8;j++)
        {
            check = crc_reg & 0x0001;
            crc_reg >>= 1;
            if(check==0x0001)
            {
                crc_reg ^= 0xA001;
            }
        }
    }
    return crc_reg;
}
