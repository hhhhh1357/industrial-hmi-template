#include "sysutil.h"
#include "set_sys_time.h"
#include "net.h"

#include <QFile>
#include <QTextCodec>

SysUtil::SysUtil()
{

}

bool SysUtil::setDatetime(QDateTime &timestamp)
{
    // 需要调用 C的设置时间
    Set_Sys_Time(timestamp.toString("yyyyMMddHHmmss").toLatin1().data());
    return true;
}

bool SysUtil::exportExcel(QString fileName, QList<QString> colName, QList<QVector<double> *> &data)
{
//    QXlsx::Document xlsx;
//    for (int i=0;i<colName.count();i++)
//    {
//       QString colN;
//       colN = QString("%1").arg(QString('A'+i))+"1";
//       xlsx.write( colN, colName.at(i));
//    }
//    for (int i=0;i<data.at(0)->count();i++)
//    {
//        QString colN;
//        colN = QString("%1%2").arg(QString('A')).arg((i+2));
//        xlsx.write( colN, i+1);

//        colN = QString("%1%2").arg(QString('B')).arg((i+2));
//        xlsx.write( colN, (QDateTime::fromMSecsSinceEpoch(data.at(0)->at(i))).toString("yyyy-MM-dd HH-mm-ss"));

//        for (int j=1;j<data.count();j++)
//        {
//            colN = QString("%1%2").arg(QString('B'+j)).arg((i+2));
//            xlsx.write( colN, data.at(j)->at(i));
//        }
//    }
//    xlsx.saveAs(fileName);
    //    return  true;
}

//bool SysUtil::exportCSV(QString fileName, QList<QString> colName, QList<QVector<double> *> &data)
//{
//    QFile file(fileName);
// qDebug()<<__FILE__<<"::"<<__FUNCTION__<<" : filename="<<fileName<<endl;


//    if (file.exists(fileName))
//    {
//        file.setPermissions(QFileDevice::ReadOther|QFileDevice::ReadGroup|QFileDevice::ReadOther|QFileDevice::ReadOwner
//                            |QFileDevice::WriteUser|QFileDevice::WriteGroup|QFileDevice::WriteOther|QFileDevice::WriteOwner);
//        file.remove();
//    }

//    bool r = file.open(QIODevice::WriteOnly | QIODevice::Text);

//    if (r==false)
//    {
//        qDebug()<<__FILE__<<"::"<<__FUNCTION__<<" : filename open failed!"<<endl;
//        return false;
//    }


//    int k=0;
//    int len=0;
//    do
//    {
//       len=file.write(colName.at(k).toUtf8().data());
//    qDebug()<<__FILE__<<"::"<<__FUNCTION__<<" : write: len="<<len<<"#"<<colName.at(k).toUtf8().data()<<endl;

//       k++;
//       if (k<colName.length())
//          file.write(",");
//    }while (k<colName.length());
//    file.write("\r\n");

//    for (int i=0;i<data.at(0)->count();i++)
//    {
//        file.write( (QString::number(i+1)+",").toLatin1().data());
//        file.write( ((QDateTime::fromMSecsSinceEpoch(data.at(0)->at(i))).toString("yyyy-MM-dd HH-mm-ss")+",").toLatin1().data());

//        k=1;
//        for (int j=1;j<data.count();j++)
//        {
//            len = file.write( (QString::number(data.at(j)->at(i))).toLatin1().data());
//   qDebug()<<__FILE__<<"::"<<__FUNCTION__<<" : write: len="<<len<<"#"<<(QString::number(data.at(j)->at(i))).toLatin1().data()<<endl;
//            k++;
//            if (k<data.count())
//                file.write(",");
//        }
//        file.write("\r\n");
//    }

//    file.flush();
//    file.close();
//    system("sync");

//    return  true;
//}

bool SysUtil::exportCSV(QString fileName, QList<QString> colName, QList<QVector<double> *> &data)
{
    QFile file(fileName);

    if (file.exists(fileName))
    {
        file.setPermissions(QFileDevice::ReadOther|QFileDevice::ReadGroup|QFileDevice::ReadOther|QFileDevice::ReadOwner
                            |QFileDevice::WriteUser|QFileDevice::WriteGroup|QFileDevice::WriteOther|QFileDevice::WriteOwner);
        file.remove();
    }

    bool r = file.open(QIODevice::WriteOnly | QIODevice::Text);

    if (r==false)
    {
        //qDebug()<<__FILE__<<"::"<<__FUNCTION__<<" : filename open failed!"<<endl;
        return false;
    }
    file.close();
    file.setPermissions(QFileDevice::ReadOther|QFileDevice::ReadGroup|QFileDevice::ReadOther|QFileDevice::ReadOwner
                        |QFileDevice::WriteUser|QFileDevice::WriteGroup|QFileDevice::WriteOther|QFileDevice::WriteOwner);
    file.open(QIODevice::ReadWrite | QIODevice::Text);

    QTextCodec *code;
    code = QTextCodec::codecForName("UTF-8");
    QTextStream out(&file);
    out.setGenerateByteOrderMark(true);
    out.setCodec(code);

    int k=0;
    int len=0;
    do
    {
       len=file.write(colName.at(k).toUtf8().data());

       out<<colName.at(k);

       k++;
       if (k<colName.length())
          out<<",";

    }while (k<colName.length());

    out<<endl;

    for (int i=0;i<data.at(0)->count();i++)
    {
        out<<QString::number(i+1)+",";
        out<<((QDateTime::fromMSecsSinceEpoch(data.at(0)->at(i))).toString("yyyy-MM-dd HH-mm-ss")+",");

        k=1;
        for (int j=1;j<data.count();j++)
        {
            out<<QString::number(data.at(j)->at(i));
            k++;
            if (k<data.count())
                out<<",";
        }
        out<<endl;
    }
    out.flush();
    file.close();
    system("sync");
    return  true;
}

bool SysUtil::getSDcardStatus()
{
    if (SDcardCheck()==1)
        return true;
    else
        return false;
}

bool SysUtil::getUSBStatus()
{
    if (USBCheck()==1)
        return true;
    else
        return false;
}

bool SysUtil::setEthernet(int ch, QString ip, QString mask, QString gateway)
{
    //设置网口
    int r = Set_Ethernet((Interface)ch , ip.toLatin1().data(), mask.toLatin1().data(), gateway.toLatin1().data());
    if (r==1)
        return true;
    else
        return false;
}

bool SysUtil::setEthernet(QString portName, QString ip, QString mask, QString gateway)
{
    int ch=portName.right(1).toInt()-1;
    if (ch<2 && ch>=0)
    {
        return setEthernet(ch, ip, mask, gateway);
    }
    else
    {
        return false;
    }
}

bool SysUtil::dial4GDtu( QString provider )
{
    //4G拨号联网

    APN apn;
    if(provider=="ChinaMobile")
        apn=China_Mobile;
    else if(provider=="ChinaUnicom")
        apn = China_Unicom;
    else
        apn = China_Telecom;
    int r = Set_Wlan_Net(apn);

    if (r==1)
        return true;
    else
        return false;
}

void SysUtil::closeNetwork(int ch)
{
    Close_Network((Interface)ch);
}

void SysUtil::closeNetwork(QString portName)
{
    int ch=portName.right(1).toInt()-1;
    if (ch<2 && ch>=0)
    {
        Close_Network((Interface)ch);
    }
}

bool SysUtil::SetNetWork(BSysConfig &sysConfig)
{
    //设置本地网口
    for (int i=0;i<sysConfig.EthernetList.count();i++)
    {
        if (sysConfig.EthernetList.at(i)->status!=0) //网口启用
        {
            //设置网口参数
            SysUtil::setEthernet(sysConfig.EthernetList.at(i)->portName,
                                 sysConfig.EthernetList.at(i)->ip,
                                 sysConfig.EthernetList.at(i)->mask,
                                 sysConfig.EthernetList.at(i)->gate);
        }
    }

//*************************20220606测试4G时屏蔽冯老师的代码************************//
    //打开4G网络
    if(sysConfig.dtu4G.status!=0)
    {
        SysUtil::dial4GDtu(sysConfig.dtu4G.provider);
        //4G启用，关闭以太网口
        SysUtil::closeNetwork(ETH0);
        SysUtil::closeNetwork(ETH1);
    }
    else{
        SysUtil::closeNetwork(PPPD);
    }

//*************************20220606测试4G时新添加代码************************//
//    if(sysConfig.EthernetList.at(3)->Status!=0){
//        SysUtil::dial4GDtu(sysConfig.EthernetList.at(3)->provider);
//        //4G启用,关闭以太网
//        SysUtil::closeNetwork(ETH0);
//        SysUtil::closeNetwork(ETH1);
//    }

}

bool SysUtil::getEthernetStatus(int ch)
{
    int r= EthernetCheck((Interface)ch);
    if(r==1)
    {
        return true;
    }
    else
    {
        return false;
    }


}

bool SysUtil::getEthernetStatus(QString portName)
{
    int ch=portName.right(1).toInt()-1;
    if (ch<2 && ch>=0)
    {
        return getEthernetStatus((Interface)ch);
    }
    else
    {
        return false;
    }
}

bool SysUtil::get4gDtuStatus()
{
    int r= Dtu4GCheck();
    if(r==1)
    {
        return true;
    }
    else
    {
        return false;
    }

}

bool SysUtil::getSwitchStatus(KEY key)
{
    OpenKey();

   int r =  Single_Press(key);
   if(r==0)
   {
       return true;
   }
   else
   {
       return false;
   }
}

bool SysUtil::getSwitchStatus(QString portName)
{
    int ch=portName.right(1).toInt();
    if (ch<=4 && ch>=1)
    {
        return getSwitchStatus((KEY)ch);
    }
    else
    {
        return false;
    }

}

bool SysUtil::getRelayStatus(RELAYER_CHANNEL ch)
{
    int r = get_RelayStatus(ch);
    if(r==1)
    {
        return  true;
    }
    else
    {
        return false;
    }

}

bool SysUtil::getRelayStatus(QString portName)
{
    int ch=portName.right(1).toInt();
    if (ch<=4 && ch>=1)
    {
        return getRelayStatus((RELAYER_CHANNEL)ch);
    }
    else
    {
        return false;
    }
}
