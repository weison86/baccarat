#ifndef BACCARAT_H
#define BACCARAT_H

#include <QTableWidgetItem>
#include <QList>
#include <QByteArray>
#include <QSqlDatabase>
#include <QKeyEvent>

class TypeArea;
enum BaccaratItemType
{
    bettype,
    losstype,
    calChipstype,
    calMoneytype
};

enum BaccaratAreaType
{
    banktype,
    playtype,
    tietype,
    bankpairtype,
    playtpepairtype
};

enum BaccaratResultType
{
    bankwin,
    playwin,
    tiewin,
    bank_bankpair_win,
    bank_playpair_win,
    bank_bankpair_playpair_win,
    play_bankpair_win,
    play_playpair_win,
    play_bankpair_playwin_win,
    tie_bankpair_win,
    tie_playpair_win,
    tie_bankpair_playwin_win

};

enum BaccaratGambleState
{
    readybetState,
    startbetState,
    incomeState,
    outputState,
    GambleEnd

};

class  BaccaratItem : public QTableWidgetItem
{

public:
    BaccaratItem();
    ~ BaccaratItem();
  //  virtual ~QTableWidgetItem ();
    BaccaratItemType type;
    TypeArea *typearea;

    void SetAntNum(quint16 num);
    void SetMoney(quint32 );
    void SetCount(quint32);
    //  void SetUidCount(quint32 count);
    //  void SetUid(QByteArray id);
    void SetUids(QList<QByteArray> idList);
    void setItemType(BaccaratItemType itemType);
    void clearData();
    void clearAll();
    quint32 getChipsCount();
    quint16 getAntNum();
    quint32 getMoney();
    //   QByteArray getUids();
    QList<QByteArray> getUids();


private:
    quint16 Number;
    quint16 AntNum;
    quint32 money;
    quint32 count;
    quint32 uidCount;
    //QByteArray uid;
    QList<QByteArray> uidList;
};

//QMap<BaccaratAreaType,BaccaratArea*> areamap;

class TypeArea
{
public:
    TypeArea(quint32 row, quint32 count, BaccaratItemType  areatype);

    //    void setRow(quint32 row);
    //    void setColcount(quint32 count);
    //  void setRange(quint32 row,quint32 count,BaccaratItemType type);
    //  TypeArea(quint32 row,quint32 count);
    QList<BaccaratItem*> getItems();
    BaccaratItem *getItem(int index);
    QList<BaccaratItem*> items;
    BaccaratItemType type;
    BaccaratItem *bacitem;
    BaccaratItem *calitem;
    BaccaratItem *totalChipsItem;
    BaccaratItem *totalMoneyItem;
    quint32 rowNum;

    ~TypeArea();
 //   friend class QQTableWidgetItem;
private:

    quint32 calChpsNumCol;
    quint32 calMoneyNumCol;

    quint32 colCount;

};

class BaccaratArea
{
public:
    void setOdds(quint32 od);
    quint32 getOdds();
    BaccaratArea(TypeArea *bet,TypeArea *loss);
    TypeArea *betArea;
    TypeArea *lossArea;
    ~BaccaratArea();
private:
    BaccaratAreaType areatype;
    quint32 odds;
    QList<TypeArea *> list;


};


class BaccaratDesk
{

public:
    BaccaratDesk(BaccaratArea *bank,BaccaratArea *play,BaccaratArea *tie,BaccaratArea *bankpair,BaccaratArea *playpair);
    ~BaccaratDesk();
    void setstaffname(QString name);
    void setdesknum(quint32 num);
    void sethostname(QString name);
    QString getstaffname();
    quint32 getdesknum();
    QString gethostname();
    BaccaratArea *Bank;
    BaccaratArea *Play;
    BaccaratArea *Tie;
    BaccaratArea *Bankpair;
    BaccaratArea *Playpair;
private:

    QString HostName;
    QString StaffName;
    quint32 DeskNum;

    QList<BaccaratArea *> list;
    // BaccaratResultType result;

};
class BaccaratGamble
{


public:
    BaccaratDesk *currentdesk;
    QList<BaccaratItem> incomelist;
    QList<BaccaratItem> outputlist;
    //QDateTime starttime;
    //QDateTime endtime;
    BaccaratResultType currentresult;
    BaccaratGambleState state;

};


#endif // BACCARAT_H
