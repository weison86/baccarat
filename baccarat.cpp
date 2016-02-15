
#include "baccarat.h"
#include <QDebug>

BaccaratItem::BaccaratItem()
{
    AntNum =0;
    money =0;
    uidList.clear();

    setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}


BaccaratItem::~BaccaratItem()
{
    qDebug() << "Bac";
    //clearData();
}
//BaccaratItem::~QTableWidgetItem ()
//{


//}


void BaccaratItem::SetAntNum(quint16 num)
{
    AntNum = num;
}

void BaccaratItem::SetMoney(quint32 moneyVal)
{
    money = moneyVal;
}

void BaccaratItem::SetCount(quint32 Count)
{
    count = Count;
}

void BaccaratItem::SetUids(QList<QByteArray> idList)
{
    uidList = idList;
}

void BaccaratItem::setItemType(BaccaratItemType itemType)
{
    type =itemType;
}

void BaccaratItem::clearData()
{
    money = 0;
    uidList.clear();
}

quint16 BaccaratItem::getAntNum()
{
    return AntNum;
}

quint32 BaccaratItem::getMoney()
{
    return money;
}

QList<QByteArray> BaccaratItem::getUids()
{

    return uidList;
}


quint32 BaccaratItem::getChipsCount()
{
    return uidList.count();
}



/*=================================================================================================================*/


TypeArea::TypeArea(quint32 row, quint32 count, BaccaratItemType areatype)
{
    type = areatype;
    rowNum = row;
    colCount = count;
    TypeArea *typeArea;
    typeArea = this;

    BaccaratItem *bacitem = new BaccaratItem[count];
    for(int i = 0; i < count; i++)
    {
        (bacitem+i)->type = type;
        (bacitem+i)->typearea = typeArea;

        items << (bacitem + i);

    }
    BaccaratItem *calitem = new BaccaratItem[2];
    totalChipsItem = calitem;
    totalChipsItem->type = calChipstype;
    totalMoneyItem = calitem + 1;
    totalMoneyItem ->type = calMoneytype;
    calChpsNumCol = 8;
    calMoneyNumCol = 9;


}

QList<BaccaratItem *> TypeArea::getItems()
{
    return items;
}

BaccaratItem *TypeArea::getItem(int index)
{
    return items.at(index);
}


TypeArea::~TypeArea()
{

    delete items.at(0);
    for(int i = 0; i < items.length();i++)
    {
       delete items.at(i);

    }
//    delete totalChipsItem;
//    delete totalMoneyItem;
}



/*=======================================================================================================================*/

BaccaratArea::BaccaratArea(TypeArea *bet, TypeArea *loss)
{
    betArea  = bet;
    list << betArea;
    lossArea = loss;
    list << lossArea;

}

BaccaratArea::~BaccaratArea()
{

}


void BaccaratArea::setOdds(quint32 od)
{
    odds = od;
}

quint32 BaccaratArea::getOdds()
{
    return odds;
}





/*=======================================================================================================================*/

BaccaratDesk::BaccaratDesk(BaccaratArea *bank, BaccaratArea *play, BaccaratArea *tie, BaccaratArea *bankpair, BaccaratArea *playpair)
{

    Bank = bank;
    list << Bank;
    Play = play;
    list << Play;
    Tie = tie;
    list << Tie;
    Bankpair = bankpair;
    list << Bankpair;
    Playpair = playpair;
    list << Playpair;

}

BaccaratDesk::~BaccaratDesk()
{


}



void BaccaratDesk::setstaffname(QString name)
{
    StaffName = name;
}

void BaccaratDesk::setdesknum(quint32 num)
{
    DeskNum = num;
}

void BaccaratDesk::sethostname(QString name)
{
    HostName = name;
}

QString BaccaratDesk::getstaffname()
{
    return StaffName;
}

quint32 BaccaratDesk::getdesknum()
{
    return DeskNum;
}

QString BaccaratDesk::gethostname()
{
    return HostName;
}





