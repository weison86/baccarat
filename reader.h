#ifndef READER_H
#define READER_H
#include "posix_qextserialport.h"
//#include <QSocketNotifier>
#include <qsocketnotifier.h>
#include <QtNetwork>
#include <QObject>

#define SOF 0x05
#define SOF_POS 0x00
#define cmd_inventory  0xF001
#define cmd_config     0xF101 /*д*/
enum state
{
   readuid=0,
   setAnt,
};

#define CNF1Addr 0x01
struct CONFIG1
{
    quint8 pwr;
    quint8 ANT[2];
    quint8  ModDepth;
    quint8  reserve[];
};

class Reader :public Posix_QextSerialPort
{
     Q_OBJECT
public:
    Reader();
    ~Reader();
signals:
    void inventoryReady(QByteArray bd);
    void uidReady(QList<QByteArray > list);
    void SetAntOk();
private slots:
    void readMyCom();
    void DecodeChip(QByteArray Frames);
private:
    QByteArray result;
    struct CONFIG1 config1;
    QByteArray Config2;
    QByteArray Frames;
    QList<QByteArray> uidList;
    QList<QByteArray> FrameList;
    quint16 ANT;
    quint16 resultAnt;
    enum state currentstate;
    quint16 times;

public:

    quint16 cal_crc16(QByteArray ptr,  unsigned short len);
    QByteArray write_cmd(quint16 cmd, QByteArray Data);
    void Set_ANT(quint16 number);
    QByteArray WriteConfig1();
    QByteArray inventory();
    QByteArray get_result();
    quint16 getAnt();
    QSocketNotifier *notifier;

};

#endif // READER_H
