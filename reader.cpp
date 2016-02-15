#ifndef READER_CPP
#define READER_CPP

#include "reader.h"
#include <QDebug>
#include <QByteArray>


Reader::Reader():Posix_QextSerialPort()
{
    //    ANT = 0x0;
    //    static const char defcon12[] ={0x01,0x08,0x01,0x00,0x05,0x00,0x00,0x00,0x00};
    //    defConfig1 = QByteArray::fromRawData(defconf1 ,sizeof(defconf1));
    times = 0;
    config1.pwr = 0x08;
    config1.ANT[0] = 0x01;
    config1.ANT[1] = 0x00;
    config1.ModDepth = 0x05;
    config1.reserve[0] =0x00;
    config1.reserve[1]=0x00;
    config1.reserve[2]=0x00;
    config1.reserve[3]=0x00;

//    QSocketNotifier *notifier;



    // {0x08,0x01,0x00,0x05,0x00,0x00,0x00,0x00};

  //  connect(this,SIGNAL(readyRead()),this,SLOT(readMyCom()));

    connect(this,SIGNAL(inventoryReady(QByteArray)),this,SLOT(DecodeChip(QByteArray)));
}



Reader::~Reader()
{

    close();
}

void Reader::readMyCom()
{
   // static const char idecode[] = {0x03,0x00,0x01,0xf0,0x01,0x07,0x05,0x89};
    static const char endcode[] = {0x05,0x07,0x01,0xF0,0x01,0x00,0x03,0x89};

    static const char configback[] = {0x05,0x07,0x01,0xF1,0x01,0x00,0xdf,0xd3}; // 050701f10100dfd3
//    QByteArray idecodebd = QByteArray::fromRawData(idecode ,sizeof(idecode));
    QByteArray endcodebd = QByteArray::fromRawData(endcode ,sizeof(endcode));
    QByteArray configbackbd = QByteArray::fromRawData(configback ,sizeof(configback));
    QByteArray bd = readAll();

    //      if(bytesAvailable() > 17)
    //      {
    //        bd = read(17);

    //      }


    Frames.append(bd);
    /*qDebug() << Frames.toHex()*/;
//    if(Frames.right(8) == idecodebd)
//    {
//        return;
//    }
    if(Frames.right(8) == endcodebd && currentstate == readuid  )
    {


        emit inventoryReady(Frames);
//        Frames.clear();
//        qDebug() << "readuid";


    }
    else if(Frames.right(8) == configbackbd && currentstate == setAnt)
    {

//        qDebug() << "setANT";
        Frames.clear();
        reset();
        emit SetAntOk();
    }
    else
    {
        reset();
    }

    //    times++;

    //    qDebug() << "times: " << times;

}

QByteArray  Reader::write_cmd(quint16 cmd, QByteArray Data)
{

    QByteArray bd;
    bd.append(0xFF);
    bd.append(cmd >> 8 & 0x00ff );
    bd.append(cmd & 0x00ff );
    bd.append(Data);
    bd.prepend(bd.length()+3);
    quint16 crc16 = cal_crc16( bd, bd.length());
    bd.append((crc16  )& 0x00ff);
    bd.append((crc16 >> 8 ) & 0x00ff);
    bd.prepend(SOF);

    //   qDebug() << bd.toHex();
    write(bd);
    return bd;

}
QByteArray Reader::WriteConfig1()
{
    QByteArray bd;
    bd.append(0x01);
    bd.append(config1.pwr);
    bd.append(config1.ANT[0]);
    bd.append(config1.ANT[1]);
    bd.append(config1.ModDepth);
    bd.append(config1.reserve[0]);
    bd.append(config1.reserve[1]);
    bd.append(config1.reserve[2]);
    bd.append(config1.reserve[3]);

    return write_cmd(cmd_config,bd);//0x08,0x02,0x00,0x05,0x00,0x00,0x00,0x00  2

}
void Reader::Set_ANT(quint16 number)
{
    //Config2.replace(2,1,0x02);
    currentstate = setAnt;
    config1.ANT[0] = (number  ) & (0x00ff);
    config1.ANT[1] = (number >> 8) & (0x00ff);
    WriteConfig1();

}

QByteArray Reader::inventory()
{

    Frames.clear();
    uidList.clear();
    FrameList.clear();
    currentstate = readuid;
//    static const char query[] ={0x00};
    static const char query[] ={0x00};

    QByteArray bd = QByteArray::fromRawData(query ,sizeof(query));
    return   write_cmd(cmd_inventory,bd);
    // return bd;

}
quint16 Reader::cal_crc16(QByteArray ptr,  unsigned short len)
{
    unsigned short crc;
    unsigned short i,j;
    crc=0xffff;
    for(i=0;i<len;i++)
    {
        crc^=(unsigned char )ptr.at(i);
        for(j=0;j<8;j++)
        {
            if(crc&0x0001)
                crc=(crc>>1)^0x8408;
            else
                crc=(crc>>1);
        }
    }
    return(crc);


}
quint16 Reader::getAnt()
{
   return resultAnt;
}
void Reader::DecodeChip(QByteArray Frames)
{

    QByteArray ant;
//    qDebug()<<"DecodeChip"<<Frames.toHex();
    for(int i = 0; i < Frames.length() - 8;i = i + 18)
    {
        if(!uidList.contains(Frames.mid(i,18)))
        {
            FrameList << Frames.mid(i,18);

            uidList << Frames.mid(i+7,8);
            ant = Frames.mid(i+15,1);
      //     qDebug()<<"Reader:" << uidList.at(i);
        }
    }


    for(int i = 0 ; i < FrameList.length(); i++)
    {
        QByteArray bd;
        unsigned short crc16;
//        qDebug()<<"DecodeChip" << FrameList.at(i).toHex();
        bd.clear();
        //            qDebug() << FrameList.at(i).toHex();
        //            qDebug() << FrameList.at(i).mid(1,14).toHex();
        //            qDebug() << FrameList.at(i).right(2).toHex();
        crc16 = cal_crc16(FrameList.at(i).mid(1,15),15);


        bd.append((crc16 ) & 0x00ff);
        bd.append((crc16 >> 8 )&0x00ff);

        if(bd != FrameList.at(i).right(2))
        {

            qDebug("CRC16 error");

        }

    }
    //if(!uidList.empty())
        emit uidReady(uidList);


}
QByteArray Reader::get_result()
{
    return result;
}


#endif // READER_CPP
