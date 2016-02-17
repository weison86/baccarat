#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QSqlQueryModel>
#include "database.h"
#include "config.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // this->setWindowFlags(Qt::FramelessWindowHint);


    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    ui->tableWidget->setTextElideMode(Qt::ElideNone);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // ui->tableWidget->clear();
    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->setSpan(4,0,1,8);
    ui->tableWidget->setSpan(5,0,1,8);
    ui->tableWidget->setSpan(6,0,1,8);
    ui->tableWidget->setSpan(7,0,1,8);
    ui->tableWidget->setSpan(8,0,1,8);
    ui->tableWidget->setSpan(9,0,1,8);

    setupDesk();
    attachTableWidget(Desk);
    setupParam(Desk);
    initParam();

    //    ui->readyBet->setFocus();
    //    ui->readyBet->setDefault(true);

    winpe.setColor(QPalette::Window,Qt::green);

    defpe.setColor(QPalette::Window,Qt::darkBlue);
    ui->BankLabel->setAutoFillBackground(true);
    ui->PlayLabel->setAutoFillBackground(true);
    ui->TieLabel->setAutoFillBackground(true);
    ui->BankPairLabel->setAutoFillBackground(true);
    ui->PlayPairLabel->setAutoFillBackground(true);

    resetItem();

    setupReader(SERIAL);


    inventoryTimer = new QTimer;

    connect(inventoryTimer,SIGNAL(timeout()),this,SLOT(inventory()));
    initANT();//for test
    currentIndex = 0 ;
    currentItem = betList.at(currentIndex);
    reader->Set_ANT(currentItem->getAntNum());
    BacGamble = new BaccaratGamble;

    BacGamble->currentdesk = Desk;
    BacGamble->state = readybetState;
    //  initShowLabel();
    connectdatabase(HOSTNAME,PORT,DATABASENAME,USERNAME,PASSWD);
}




void MainWindow::initANT()
{
    betList.at(0)->SetAntNum(1 << 0);
    betList.at(1)->SetAntNum(1 << 1);
    betList.at(2)->SetAntNum(1 << 2);
    bettestList << betList.mid(0,3);

    betList.at(8)->SetAntNum(1 << 3);
    betList.at(9)->SetAntNum(1 << 4);
    betList.at(10)->SetAntNum(1 << 5);
    bettestList << betList.mid(8,3);

    betList.at(16)->SetAntNum(1 << 6);
    betList.at(17)->SetAntNum(1 << 7);
    betList.at(18)->SetAntNum(1 << 8);
    bettestList << betList.mid(16,3);

    losstestList << lossList.mid(0,3);
    losstestList << lossList.mid(8,3);
    losstestList << lossList.mid(16,3);


}

void MainWindow::initShowLabel()
{
    ui->BankLabel->setEnabled(false);
    ui->PlayLabel->setEnabled(false);
    ui->TieLabel->setEnabled(false);
    ui->BankPairLabel->setEnabled(false);
    ui->PlayPairLabel->setEnabled(false);

}

void MainWindow::keyReleaseEvent (QKeyEvent *event)
{
    int k;

    k = event->key();
    //  int key;

    switch(k)
    {
    case Qt::Key_0:
        keyList << 0;
        break;
    case Qt::Key_1:
        keyList << 1;
        break;
    case Qt::Key_2:
        keyList << 2;
        break;
    case Qt::Key_3:
        keyList << 3;
        break;
    case Qt::Key_4:
        keyList << 4;
        break;
    case Qt::Key_5:
        keyList << 5;
        break;
    case Qt::Key_6:
        keyList << 6;
        break;
    case Qt::Key_7:
        keyList << 7;
        break;
    case Qt::Key_8:
        keyList << 8;
        break;
    case Qt::Key_9:
        keyList << 9;
        break;
        //        default:
    }





}

//enum BaccaratResultType
//{
//    bankwin,
//    playwin,
//    tiewin,
//    bank_bankpair_win,
//    bank_playpair_win,
//    bank_bankpair_playpair_win,
//    play_bankpair_win,
//    play_playpair_win,
//    play_bankpair_playwin_win,
//    tie_bankpair_win,
//    tie_playpair_win,
//    tie_bankpair_playwin_win

//};

void MainWindow::handleResult(int key)
{
    if(key > 12)
    {
        return;
    }
    switch(key)
    {
    case 01: BacGamble->currentresult = bankwin;                    break;
    case 02: BacGamble->currentresult = playwin;                    break;
    case 03: BacGamble->currentresult = tiewin;                     break;
    case 04: BacGamble->currentresult = bank_bankpair_win;          break;
    case 05: BacGamble->currentresult = bank_playpair_win;          break;
    case 06: BacGamble->currentresult = bank_bankpair_playpair_win; break;
    case 07: BacGamble->currentresult = play_bankpair_win;          break;
    case 8: BacGamble->currentresult = play_playpair_win;           break;
    case 9: BacGamble->currentresult = play_bankpair_playwin_win;   break;
    case 10: BacGamble->currentresult = tie_bankpair_win;           break;
    case 11: BacGamble->currentresult = tie_playpair_win;           break;
    case 12: BacGamble->currentresult = tie_bankpair_playwin_win;   break;
    default: ;
    }
    keyList.clear();
    handleBaccaratResult();


}
void MainWindow::setWinArea(QLabel *label )
{
    if(label->isEnabled())
    {
        label->setPalette(winpe);

    }


}
void MainWindow::attachTableWidget(BaccaratDesk *desk)
{

    attachBaccaratArea(desk->Bank);
    attachBaccaratArea(desk->Play);
    attachBaccaratArea(desk->Tie);
    attachBaccaratArea(desk->Bankpair);
    attachBaccaratArea(desk->Playpair);

}

void MainWindow::setupParam(BaccaratDesk *desk)
{
    setupList(desk->Bank);
    setupList(desk->Play);
    setupList(desk->Tie);
    setupList(desk->Bankpair);
    setupList(desk->Playpair);


}

void MainWindow::setupList(BaccaratArea *area)
{
    apendbetList(area->betArea->items);
    apendlossList(area->lossArea->items);


}

void MainWindow::apendbetList(QList<BaccaratItem *> list)
{

    betList.append(list);
}

void MainWindow::apendlossList(QList<BaccaratItem *> list)
{

    lossList.append(list);
}

void MainWindow::attachBaccaratArea(BaccaratArea *area)
{

    attachTypeArea(area->betArea);
    attachTypeArea(area->lossArea);
}

void MainWindow::attachTypeArea(TypeArea *type)
{
    attachItems(type->items,type->rowNum);
    ui->tableWidget->setItem(type->rowNum,8,type->totalChipsItem);
    ui->tableWidget->setItem(type->rowNum,9,type->totalMoneyItem);

}

void MainWindow::attachItems(QList<BaccaratItem*> items,int rownum)
{
    int col = items.length();
    for(int i=0; i < col;i++)
    {
        ui->tableWidget->setItem(rownum,i,items.at(i));

    }

}
MainWindow::~MainWindow()
{
    delete ui;
    delete Bank->betArea;
    delete Bank->lossArea;
    delete Bank;
    delete Play->betArea;
    delete Play->lossArea;
    delete Play;
    delete Tie->betArea;
    delete Tie->lossArea;
    delete Tie;
    delete Bankpair->betArea;
    delete Bankpair->lossArea;
    delete Bankpair;
    delete Playpair->betArea;
    delete Playpair->lossArea;
    delete Playpair;
    delete Desk;
}

void MainWindow::setupReader(QString comnum)
{
    reader = new Reader();
    reader->setPortName(comnum);
    reader->setQueryMode(QextSerialBase::Polling);
    reader->open(QIODevice::ReadWrite);
    reader->setBaudRate(BAUD38400);
    reader->setDataBits(DATA_8);
    reader->setParity(PAR_EVEN);
    reader->setStopBits(STOP_1);
    reader->setFlowControl(FLOW_OFF);
    reader->setTimeout(0);
    // notifier = new QSocketNotifier(reader->Posix_File->handle(),QSocketNotifier::Read,this);

    reader->notifier = new QSocketNotifier(reader->Posix_File->handle(),QSocketNotifier::Read,this);
    connect(reader->notifier ,SIGNAL(activated(int)),reader,SLOT(readMyCom()));

    connect(reader,SIGNAL(uidReady(QList<QByteArray>)),this,SLOT(ReadUid(QList<QByteArray>)));


}

void MainWindow::initParam()
{

    inIndex=0;
    outIndex=0;
    initbetList(&betList);
    initlossList(&lossList);

}

void MainWindow::initlossList(QList<BaccaratItem *> *list)
{
    quint16 ant=10;
    for(int i = 0; i < 19; i++ )
    {
        list->at(i)->SetAntNum(ant);
    }

}
void MainWindow::initbetList(QList<BaccaratItem *> *list)
{
    quint16 ant=0;
    int lenght = list->length();
    for(int i = 0; i <lenght;i++)
    {
        ant = (1 << i);
        list->at(i)->SetAntNum(ant);

    }
}
void MainWindow::setupDesk()
{
    TypeArea *betArea;
    TypeArea *lossArea;
    betArea  = new TypeArea(0,8,bettype);
    lossArea = new TypeArea(1,8,losstype);


    Bank = new BaccaratArea(betArea,lossArea);

    betArea  = new TypeArea(2,8,bettype);
    lossArea = new TypeArea(3,8,losstype);
    Play = new BaccaratArea(betArea,lossArea);

    betArea  = new TypeArea(4,1,bettype);
    lossArea = new TypeArea(5,1,losstype);
    Tie =  new BaccaratArea(betArea,lossArea);

    betArea  = new TypeArea(6,1,bettype);
    lossArea = new TypeArea(7,1,losstype);
    Bankpair = new BaccaratArea(betArea,lossArea);

    betArea  = new TypeArea(8,1,bettype);
    lossArea = new TypeArea(9,1,losstype);
    Playpair = new BaccaratArea(betArea,lossArea);

    Desk     = new BaccaratDesk(Bank,Play,Tie,Bankpair,Playpair);


}

void MainWindow::inventory()
{
    inventoryTimer->stop();

    reader->inventory();


}

void MainWindow::ReadUid(QList<QByteArray> list)
{



    static int num;
    int count = list.length();
    QByteArray uid;
    int moneyVal=0;;
    int moneySum=0;
    for(int i = 0; i  < count; i++)
    {



        uid = SwaptoDataBaseBytes(list.at(i)).toHex();
       // qDebug() << uid;

        moneyVal = getMoneyVal(uid);
        moneySum+=moneyVal;


    }
    if(BacGamble->state == startbetState)
    {

        currentItem->SetMoney(moneySum);
        currentItem->SetCount(count);
        currentItem->SetUids(list);
        currentItem->setText(QString::number(moneySum));
        scheduleNextBet();
    }

    if(BacGamble->state == incomeState)
    {
//        qDebug() << moneySum;
        ui->recMoneyLabel->setText(QString::number(moneySum));
        inventoryTimer->start(50);

    }

    if(BacGamble->state == outputState)
    {
       outputList.at(outIndex)->SetMoney(moneySum);
       outputList.at(outIndex)->SetCount(count);
       outputList.at(outIndex)->SetUids(list);
       outputList.at(outIndex)->setText(QString::number(moneySum));

       inventoryTimer->start(50);

    }

}

QByteArray MainWindow::SwaptoDataBaseBytes(QByteArray bd)
{
    QByteArray temp;
    temp.clear();
    for(int i = bd.length()-1; i >=0; i--)
    {
        temp.append(bd.at(i));

    }
    return temp;
}

int MainWindow::getMoneyVal(QString uid)
{


    int moneyval;
    QSqlQuery query;
    query.prepare("select moneyval from bargainingchip where uid=?");
    query.addBindValue(uid);
    query.exec();
    query.next();
    moneyval = query.value(0).toInt();

    return moneyval;

}





void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    BaccaratItem * bacItem;
    bacItem = (BaccaratItem *)item;
    if(bacItem->type == calChipstype || bacItem->type == calMoneytype)
    {
        return;
    }

    TypeArea *type = bacItem->typearea;
    int totalCount = 0;
    int totalMoney = 0;
    type->totalMoneyItem->uidList.clear();

    if(type)
    {

        int lenght = type->items.length();
        for(int i = 0; i < lenght;i++ )
        {


            int count = type->items.at(i)->getChipsCount();
            int money = type->items.at(i)->getMoney();
            type->totalMoneyItem->uidList.append(type->items.at(i)->uidList);
            totalCount +=count;
            totalMoney +=money;
        }

        type->totalChipsItem->setText(QString::number(totalCount));
        type->totalChipsItem->SetCount(totalCount);
        type->totalMoneyItem->setText(QString::number(totalMoney));
        type->totalMoneyItem->SetMoney(totalMoney);
    }



}


void MainWindow::scheduleNextBet()
{

    currentIndex++;
    if(currentIndex == MaxAnt)
    {
        currentIndex =  0;
    }

    currentItem = bettestList.at(currentIndex);
    quint16 Ant = currentItem->getAntNum();
    reader->Set_ANT(Ant);

    inventoryTimer->start(50);


}
void MainWindow::resetItem()
{
    for(int i =0;i < betList.length(); i++)
    {
        betList.at(i)->setText("0");
        betList.at(i)->setBackgroundColor(QColor(0, 0, 0, 0));
        betList.at(i)->typearea->totalChipsItem->setText("0");
        betList.at(i)->typearea->totalMoneyItem->setText("0");
    }

    for(int i =0;i < lossList.length(); i++)
    {
        lossList.at(i)->setText("0");
        lossList.at(i)->setBackgroundColor(QColor(0, 0, 0, 0));
        lossList.at(i)->typearea->totalChipsItem->setText("0");
        lossList.at(i)->typearea->totalMoneyItem->setText("0");
    }

    ui->BankLabel->setPalette(defpe);
    ui->BankPairLabel->setPalette(defpe);
    ui->PlayLabel->setPalette(defpe);
    ui->PlayPairLabel->setPalette(defpe);
    ui->TieLabel->setPalette(defpe);

    ui->readyBet->setFocus();
    ui->readyBet->setDefault(true);
    ui->startBet->setEnabled(false);
    ui->waitResult->setEnabled(false);
    ui->income->setEnabled(false);
    ui->output->setEnabled(false);
    ui->End->setEnabled(false);

    winList.clear();
    outputList.clear();
    incomeList.clear();
    outIndex = 0;
    inIndex = 0;

    ui->realMoneyLabel->clear();
    ui->recMoneyLabel->clear();
    incomeUids.clear();
    outputUids.clear();



}

void MainWindow::on_readyBet_clicked()
{


    ui->readyBet->setEnabled(false);
    ui->startBet->setEnabled(true);
    ui->startBet->setDefault(true);
    ui->startBet->setFocus();
    ui->waitResult->setEnabled(false);
    ui->income->setEnabled(false);
    ui->output->setEnabled(false);
    ui->End->setEnabled(false);
    connect(inventoryTimer,SIGNAL(timeout()),this,SLOT(inventory()));
    inventoryTimer->start(0);
    BacGamble->state = startbetState;

}



void MainWindow::on_startBet_clicked()
{



    ui->readyBet->setEnabled(false);
    ui->startBet->setEnabled(false);
    ui->waitResult->setEnabled(true);
    ui->waitResult->setFocus();
    ui->waitResult->setDefault(true);
    ui->income->setEnabled(false);
    ui->output->setEnabled(false);
    ui->End->setEnabled(false);
    //disconnect(this,)
    inventoryTimer->disconnect(this);

    // disconnect(inventory,tim);
    inventoryTimer->stop();

    BacGamble->state = waitResultState;
}

void MainWindow::on_waitResult_clicked()
{
    int key;
    if(keyList.isEmpty())
    {
        return;
    }
    key = keyList.at(0)*10 + keyList.at(1);

    if(key > 12)
    {
        keyList.clear();
        return;
    }

    handleResult(key);
    for(int i =0; i < incomeList.length();i++)
    {
        incomeList.at(i)->setBackgroundColor(QColor(255, 0, 0, 127));
    }
    reader->Set_ANT(1 << 11);
    connect(inventoryTimer,SIGNAL(timeout()),this,SLOT(inventory()));
    inventoryTimer->start(50);
    ui->readyBet->setEnabled(false);
    ui->startBet->setEnabled(false);
    ui->waitResult->setEnabled(false);
    ui->income->setEnabled(true);
    ui->income->setDefault(true);
    ui->income->setFocus();
    ui->output->setEnabled(false);
    ui->End->setEnabled(false);

    BacGamble->state = incomeState;
    for(int i = 0;i < incomeUids.length(); i++)
    {
        qDebug() << incomeUids.at(i).toHex();
    }

    // QBrush(QColor(238, 59, 59))


}

void MainWindow::on_income_clicked()
{


    if(ui->realMoneyLabel->text() != ui->recMoneyLabel->text())
    {
        return;
    }

    BacGamble->state = outputState;

    ui->readyBet->setEnabled(false);
    ui->startBet->setEnabled(false);
    ui->waitResult->setEnabled(false);
    ui->income->setEnabled(false);
    ui->output->setEnabled(true);
    ui->output->setFocus();
    ui->output->setDefault(true);
    ui->End->setEnabled(false);
    outputList.at(outIndex)->setBackgroundColor(QColor(255, 255, 0, 127));
    winList.at(outIndex)->setBackgroundColor(QColor(255, 255, 0, 127));

    reader->Set_ANT(1 << 11);
    connect(inventoryTimer,SIGNAL(timeout()),this,SLOT(inventory()));
    inventoryTimer->start(50);


}

void MainWindow::on_output_clicked()
{
    if(outputList.at(outIndex)->text() != winList.at(outIndex)->text())
    {
       return;
    }
    outIndex++;

    if(outIndex < outputList.length()){
        outputList.at(outIndex)->setBackgroundColor(QColor(255, 255, 0, 127));
        winList.at(outIndex)->setBackgroundColor(QColor(255, 255, 0, 127));


    }
    else
    {
        ui->readyBet->setEnabled(false);
        ui->startBet->setEnabled(false);
        ui->waitResult->setEnabled(false);
        ui->income->setEnabled(false);
        ui->output->setEnabled(false);;
        ui->End->setEnabled(true);
        ui->End->setFocus();
        ui->End->setDefault(true);
        BacGamble->state = GambleEnd;
    }


}

void MainWindow::on_End_clicked()
{ui->readyBet->setEnabled(true);

    resetItem();
    BacGamble->state = readybetState;


}

void MainWindow::setBaccaratGambleResult(BaccaratResultType Result)
{
    BacGamble->currentresult = Result;

}

BaccaratResultType  MainWindow::getBaccaratGambleResult()
{

    return BacGamble->currentresult;

}

void MainWindow::setBaccaratGambleState(BaccaratGambleState state)
{

    BacGamble->state = state;

}

BaccaratGambleState MainWindow::getBaccaratGambleState()
{

    return BacGamble->state;
}


void MainWindow::handleBaccaratResult()
{
    BaccaratResultType Result = getBaccaratGambleResult();
    switch(Result){
    case bankwin: handleBankWin() ; break;
    case playwin: handlePlayWin() ; break;
    case tiewin:  handleTieWin() ; break;
    case bank_bankpair_win: handleBank_BankpairWin() ; break;
    case bank_playpair_win: handleBank_PlaypairWin() ; break;
    case bank_bankpair_playpair_win : handleBank_Bankpair_PlaypairWin(); break;
    case play_bankpair_win: handlePlay_BankpairWin() ; break;
    case play_playpair_win: handlePlay_PlaypairWin() ;break;
    case play_bankpair_playwin_win : handlePlay_Bankpair_PlaypairWin();break;
    case tie_bankpair_win:handleTie_BankpairWin();break;
    case tie_playpair_win:handleTie_PlaypairWin();break;
    case tie_bankpair_playwin_win:handleTie_Bankpair_PlaypairWin();break;
    default: qDebug() << "other";
    }


}

void MainWindow::handleBankWin()
{
    outTotalMVal = 0;
    outTotalChips = 0;
    inTotalMVal = 0;
    inTotalChips = 0;

    ui->BankLabel->setPalette(winpe);

    outputList  <<   Bank->lossArea->items;
    winList  <<   Bank->betArea->items;
    outTotalMVal  += Bank->betArea->totalMoneyItem->getMoney();
    outputUids.append(Bank->betArea->totalMoneyItem->uidList);

    outTotalChips += Bank->betArea->totalChipsItem->getChipsCount();

    incomeList << Play->betArea->items;
    inTotalMVal  +=Play->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Play->betArea->totalMoneyItem->uidList);
    inTotalChips +=Play->betArea->totalChipsItem->getChipsCount();

    incomeList << Tie->betArea->items.at(0);
    inTotalMVal  +=Tie->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Tie->betArea->totalMoneyItem->uidList);
    inTotalChips +=Tie->betArea->totalChipsItem->getChipsCount();

    incomeList << Bankpair->betArea->items.at(0);
    inTotalMVal  +=Bankpair->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Bankpair->betArea->totalMoneyItem->uidList);
    inTotalChips +=Bankpair->betArea->totalChipsItem->getChipsCount();

    incomeList << Playpair->betArea->items.at(0);
    inTotalMVal  +=Playpair->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Playpair->betArea->totalMoneyItem->uidList);
    inTotalChips +=Playpair->betArea->totalChipsItem->getChipsCount();
    ui->realMoneyLabel->setText(QString::number(inTotalMVal));





}

void MainWindow::handlePlayWin()
{
    outTotalMVal = 0;
    outTotalChips = 0;
    inTotalMVal = 0;
    inTotalChips = 0;

    ui->PlayLabel->setPalette(winpe);

    outputList << Play->lossArea->items;
    winList << Play->betArea->items;
    outTotalMVal += Play->betArea->totalMoneyItem->getMoney();
    outTotalChips += Play->betArea->totalChipsItem->getChipsCount();

    incomeList << Bank->betArea->items;
    inTotalMVal  +=Bank->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Bank->betArea->totalMoneyItem->uidList);
    inTotalChips +=Bank->betArea->totalChipsItem->getChipsCount();

    incomeList << Tie->betArea->items.at(0);
    inTotalMVal  +=Tie->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Tie->betArea->totalMoneyItem->uidList);
    inTotalChips +=Tie->betArea->totalChipsItem->getChipsCount();

    incomeList << Bankpair->betArea->items.at(0);
    inTotalMVal  +=Bankpair->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Bankpair->betArea->totalMoneyItem->uidList);
    inTotalChips +=Bankpair->betArea->totalChipsItem->getChipsCount();

    incomeList << Playpair->betArea->items.at(0);
    inTotalMVal  +=Playpair->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Playpair->betArea->totalMoneyItem->uidList);
    inTotalChips +=Playpair->betArea->totalChipsItem->getChipsCount();
    ui->realMoneyLabel->setText(QString::number(inTotalMVal));

}

void MainWindow::handleTieWin()
{
    outTotalMVal = 0;
    outTotalChips = 0;
    inTotalMVal = 0;
    inTotalChips = 0;

    ui->TieLabel->setPalette(winpe);

    outputList << Tie->lossArea->items.at(0);
    winList << Tie->betArea->items.at(0);
    outTotalMVal += Tie->betArea->totalMoneyItem->getMoney();
    outTotalChips += Tie->betArea->totalChipsItem->getChipsCount();

    incomeList << Bank->betArea->items;
    inTotalMVal  +=Bank->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Bank->betArea->totalMoneyItem->uidList);
    inTotalChips +=Bank->betArea->totalChipsItem->getChipsCount();

    incomeList << Play->betArea->items;
    inTotalMVal  +=Play->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Play->betArea->totalMoneyItem->uidList);
    inTotalChips +=Play->betArea->totalChipsItem->getChipsCount();

    incomeList << Bankpair->betArea->items.at(0);
    inTotalMVal  +=Bankpair->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Bankpair->betArea->totalMoneyItem->uidList);
    inTotalChips +=Bankpair->betArea->totalChipsItem->getChipsCount();

    incomeList << Playpair->betArea->items.at(0);
    inTotalMVal  +=Playpair->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Playpair->betArea->totalMoneyItem->uidList);
    inTotalChips +=Playpair->betArea->totalChipsItem->getChipsCount();
    ui->realMoneyLabel->setText(QString::number(inTotalMVal));

}

void MainWindow::handleBank_BankpairWin()
{
    outTotalMVal = 0;
    outTotalChips = 0;
    inTotalMVal = 0;
    inTotalChips = 0;

    ui->BankLabel->setPalette(winpe);
    ui->BankPairLabel->setPalette(winpe);

    outputList  <<   Bank->lossArea->items;
    winList << Bank->betArea->items;
    outTotalMVal  += Bank->betArea->totalMoneyItem->getMoney();
    outTotalChips += Bank->betArea->totalChipsItem->getChipsCount();

    outputList  <<   Bankpair->lossArea->items;
    winList << Bankpair->betArea->items;
    outTotalMVal  += Bankpair->betArea->totalMoneyItem->getMoney();
    outTotalChips += Bankpair->betArea->totalChipsItem->getChipsCount();

    incomeList << Play->betArea->items;
    inTotalMVal  +=Play->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Play->betArea->totalMoneyItem->uidList);
    inTotalChips +=Play->betArea->totalChipsItem->getChipsCount();

    incomeList << Tie->betArea->items.at(0);
    inTotalMVal  +=Tie->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Tie->betArea->totalMoneyItem->uidList);
    inTotalChips +=Tie->betArea->totalChipsItem->getChipsCount();


    incomeList << Playpair->betArea->items.at(0);
    inTotalMVal  +=Playpair->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Playpair->betArea->totalMoneyItem->uidList);
    inTotalChips +=Playpair->betArea->totalChipsItem->getChipsCount();

    ui->realMoneyLabel->setText(QString::number(inTotalMVal));

}

void MainWindow::handleBank_PlaypairWin()
{
    outTotalMVal = 0;
    outTotalChips = 0;
    inTotalMVal = 0;
    inTotalChips = 0;

    ui->BankLabel->setPalette(winpe);
    ui->PlayPairLabel->setPalette(winpe);

    outputList  <<   Bank->lossArea->items;
    winList  <<   Bank->betArea->items;
    outTotalMVal  += Bank->betArea->totalMoneyItem->getMoney();
    outTotalChips += Bank->betArea->totalChipsItem->getChipsCount();

    outputList  <<   Playpair->lossArea->items.at(0);
    winList    << Playpair->betArea->items.at(0);
    outTotalMVal  += Playpair->betArea->totalMoneyItem->getMoney();
    outTotalChips += Playpair->betArea->totalChipsItem->getChipsCount();

    incomeList << Play->betArea->items;
    inTotalMVal  +=Play->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Play->betArea->totalMoneyItem->uidList);
    inTotalChips +=Play->betArea->totalChipsItem->getChipsCount();

    incomeList << Tie->betArea->items.at(0);
    inTotalMVal  +=Tie->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Tie->betArea->totalMoneyItem->uidList);
    inTotalChips +=Tie->betArea->totalChipsItem->getChipsCount();


    incomeList << Bankpair->betArea->items.at(0);
    inTotalMVal  +=Bankpair->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Bankpair->betArea->totalMoneyItem->uidList);
    inTotalChips +=Bankpair->betArea->totalChipsItem->getChipsCount();
    ui->realMoneyLabel->setText(QString::number(inTotalMVal));
    ui->realMoneyLabel->setText(QString::number(inTotalMVal));

}

void MainWindow::handleBank_Bankpair_PlaypairWin()
{

    outTotalMVal = 0;
    outTotalChips = 0;
    inTotalMVal = 0;
    inTotalChips = 0;

    ui->BankLabel->setPalette(winpe);
    ui->BankPairLabel->setPalette(winpe);
    ui->PlayPairLabel->setPalette(winpe);

    outputList  <<   Bank->lossArea->items;
    winList     <<   Bank->betArea->items;
    outTotalMVal  += Bank->betArea->totalMoneyItem->getMoney();
    outTotalChips += Bank->betArea->totalChipsItem->getChipsCount();

    outputList << Bankpair->lossArea->items.at(0);
    winList    << Bankpair->betArea->items.at(0);
    outTotalMVal  +=Bankpair->betArea->totalMoneyItem->getMoney();
    outTotalChips +=Bankpair->betArea->totalChipsItem->getChipsCount();

    outputList  <<   Playpair->lossArea->items.at(0);
    winList     <<  Playpair->betArea->items.at(0);
    outTotalMVal  += Playpair->betArea->totalMoneyItem->getMoney();
    outTotalChips += Playpair->betArea->totalChipsItem->getChipsCount();



    incomeList << Play->betArea->items;
    inTotalMVal  +=Play->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Play->betArea->totalMoneyItem->uidList);
    inTotalChips +=Play->betArea->totalChipsItem->getChipsCount();

    incomeList << Tie->betArea->items.at(0);
    inTotalMVal  +=Tie->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Tie->betArea->totalMoneyItem->uidList);
    inTotalChips +=Tie->betArea->totalChipsItem->getChipsCount();

    ui->realMoneyLabel->setText(QString::number(inTotalMVal));



}

void MainWindow::handlePlay_BankpairWin()
{
    outTotalMVal = 0;
    outTotalChips = 0;
    inTotalMVal = 0;
    inTotalChips = 0;

    ui->PlayLabel->setPalette(winpe);
    ui->BankPairLabel->setPalette(winpe);

    outputList  <<   Play->lossArea->items;
    winList     <<   Play->betArea->items;
    outTotalMVal  += Play->betArea->totalMoneyItem->getMoney();
    outTotalChips += Play->betArea->totalChipsItem->getChipsCount();

    outputList  <<   Bankpair->lossArea->items.at(0);
    winList     <<   Bankpair->betArea->items.at(0);
    outTotalMVal  += Bankpair->betArea->totalMoneyItem->getMoney();
    outTotalChips += Bankpair->betArea->totalChipsItem->getChipsCount();

    incomeList << Bank->betArea->items;
    inTotalMVal  +=Bank->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Bank->betArea->totalMoneyItem->uidList);
    inTotalChips +=Bank->betArea->totalChipsItem->getChipsCount();

    incomeList << Tie->betArea->items.at(0);
    inTotalMVal  +=Tie->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Tie->betArea->totalMoneyItem->uidList);
    inTotalChips +=Tie->betArea->totalChipsItem->getChipsCount();


    incomeList << Playpair->betArea->items.at(0);
    inTotalMVal  +=Playpair->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Playpair->betArea->totalMoneyItem->uidList);
    inTotalChips +=Playpair->betArea->totalChipsItem->getChipsCount();

    ui->realMoneyLabel->setText(QString::number(inTotalMVal));
}

void MainWindow::handlePlay_PlaypairWin()
{

    outTotalMVal = 0;
    outTotalChips = 0;
    inTotalMVal = 0;
    inTotalChips = 0;

    ui->PlayLabel->setPalette(winpe);
    ui->PlayPairLabel->setPalette(winpe);

    outputList  <<   Play->lossArea->items;
    winList     <<   Play->betArea->items;
    outTotalMVal  += Play->betArea->totalMoneyItem->getMoney();
    outTotalChips += Play->betArea->totalChipsItem->getChipsCount();

    outputList  <<   Playpair->lossArea->items.at(0);
    winList     <<   Playpair->betArea->items.at(0);
    outTotalMVal  += Playpair->betArea->totalMoneyItem->getMoney();
    outTotalChips += Playpair->betArea->totalChipsItem->getChipsCount();

    incomeList << Bank->betArea->items;
    inTotalMVal  +=Bank->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Bank->betArea->totalMoneyItem->uidList);
    inTotalChips +=Bank->betArea->totalChipsItem->getChipsCount();

    incomeList << Tie->betArea->items.at(0);
    inTotalMVal  +=Tie->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Tie->betArea->totalMoneyItem->uidList);
    inTotalChips +=Tie->betArea->totalChipsItem->getChipsCount();


    incomeList << Bankpair->betArea->items.at(0);
    inTotalMVal  +=Bankpair->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Bankpair->betArea->totalMoneyItem->uidList);
    inTotalChips +=Bankpair->betArea->totalChipsItem->getChipsCount();

    ui->realMoneyLabel->setText(QString::number(inTotalMVal));
}

void MainWindow::handlePlay_Bankpair_PlaypairWin()
{

    outTotalMVal = 0;
    outTotalChips = 0;
    inTotalMVal = 0;
    inTotalChips = 0;

    ui->PlayLabel->setPalette(winpe);
    ui->BankPairLabel->setPalette(winpe);
    ui->PlayPairLabel->setPalette(winpe);

    outputList  <<   Play->lossArea->items;
    winList     <<   Play->betArea->items;
    outTotalMVal  += Play->betArea->totalMoneyItem->getMoney();
    outTotalChips += Play->betArea->totalChipsItem->getChipsCount();

    outputList  << Bankpair->lossArea->items.at(0);
    winList    << Bankpair->betArea->items.at(0);
    outTotalMVal  +=Bankpair->lossArea->totalMoneyItem->getMoney();
    outTotalMVal +=Bankpair->lossArea->totalChipsItem->getChipsCount();

    outputList  <<   Playpair->lossArea->items.at(0);
    winList  <<   Playpair->betArea->items.at(0);
    outTotalMVal  += Playpair->betArea->totalMoneyItem->getMoney();
    outTotalChips += Playpair->betArea->totalChipsItem->getChipsCount();

    incomeList << Bank->betArea->items;
    inTotalMVal  +=Bank->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Bank->betArea->totalMoneyItem->uidList);
    inTotalChips +=Bank->betArea->totalChipsItem->getChipsCount();

    incomeList << Tie->betArea->items.at(0);
    inTotalMVal  +=Tie->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Tie->betArea->totalMoneyItem->uidList);
    inTotalChips +=Tie->betArea->totalChipsItem->getChipsCount();

    ui->realMoneyLabel->setText(QString::number(inTotalMVal));



}

void MainWindow::handleTie_BankpairWin()
{
    outTotalMVal = 0;
    outTotalChips = 0;
    inTotalMVal = 0;
    inTotalChips = 0;

    ui->TieLabel->setPalette(winpe);
    ui->BankPairLabel->setPalette(winpe);

    outputList  <<   Tie->lossArea->items;
    winList  <<   Tie->betArea->items;
    outTotalMVal  += Tie->betArea->totalMoneyItem->getMoney();
    outTotalChips += Tie->betArea->totalChipsItem->getChipsCount();

    outputList << Bankpair->lossArea->items.at(0);
    winList    << Bankpair->betArea->items.at(0);
    outTotalMVal  +=Bankpair->betArea->totalMoneyItem->getMoney();
    outTotalMVal +=Bankpair->betArea->totalChipsItem->getChipsCount();

    incomeList << Bank->betArea->items;
    inTotalMVal  +=Bank->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Bank->betArea->totalMoneyItem->uidList);
    inTotalChips +=Bank->betArea->totalChipsItem->getChipsCount();

    incomeList << Tie->betArea->items.at(0);
    inTotalMVal  +=Tie->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Tie->betArea->totalMoneyItem->uidList);
    inTotalChips +=Tie->betArea->totalChipsItem->getChipsCount();

    incomeList  <<  Playpair->lossArea->items.at(0);
    inTotalMVal  += Playpair->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Playpair->betArea->totalMoneyItem->uidList);
    inTotalChips += Playpair->betArea->totalChipsItem->getChipsCount();

    ui->realMoneyLabel->setText(QString::number(inTotalMVal));
}

void MainWindow::handleTie_PlaypairWin()
{
    outTotalMVal = 0;
    outTotalChips = 0;
    inTotalMVal = 0;
    inTotalChips = 0;

    ui->TieLabel->setPalette(winpe);
    ui->PlayPairLabel->setPalette(winpe);

    outputList  <<   Tie->lossArea->items.at(0);
    winList     <<   Tie->betArea->items.at(0);
    outTotalMVal  += Tie->betArea->totalMoneyItem->getMoney();
    outTotalChips += Tie->betArea->totalChipsItem->getChipsCount();

    outputList << Playpair->lossArea->items.at(0);
    winList    << Playpair->betArea->items.at(0);
    outTotalMVal  +=Playpair->betArea->totalMoneyItem->getMoney();
    outTotalMVal +=Playpair->betArea->totalChipsItem->getChipsCount();

    incomeList << Bank->betArea->items;
    inTotalMVal  +=Bank->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Bank->betArea->totalMoneyItem->uidList);
    inTotalChips +=Bank->betArea->totalChipsItem->getChipsCount();

    incomeList << Play->betArea->items;
    inTotalMVal  +=Play->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Play->betArea->totalMoneyItem->uidList);
    inTotalChips +=Play->betArea->totalChipsItem->getChipsCount();

    incomeList  <<  Bankpair->lossArea->items.at(0);
    inTotalMVal  += Bankpair->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Bankpair->betArea->totalMoneyItem->uidList);
    inTotalChips += Bankpair->betArea->totalChipsItem->getChipsCount();

    ui->realMoneyLabel->setText(QString::number(inTotalMVal));
}


void MainWindow::handleTie_Bankpair_PlaypairWin()
{

    outTotalMVal = 0;
    outTotalChips = 0;
    inTotalMVal = 0;
    inTotalChips = 0;

    ui->TieLabel->setPalette(winpe);
    ui->BankPairLabel->setPalette(winpe);
    ui->PlayPairLabel->setPalette(winpe);

    outputList  <<   Tie->lossArea->items.at(0);
    winList     <<   Tie->betArea->items.at(0);
    outTotalMVal  += Tie->betArea->totalMoneyItem->getMoney();
    outTotalChips += Tie->betArea->totalChipsItem->getChipsCount();

    outputList   <<  Bankpair->lossArea->items.at(0);
    winList      <<  Bankpair->betArea->items.at(0);
    outTotalMVal  += Bankpair->betArea->totalMoneyItem->getMoney();
    outTotalChips += Bankpair->betArea->totalChipsItem->getChipsCount();

    outputList << Playpair->lossArea->items.at(0);
    winList    << Playpair->betArea->items.at(0);
    outTotalMVal  +=Playpair->betArea->totalMoneyItem->getMoney();
    outTotalMVal +=Playpair->betArea->totalChipsItem->getChipsCount();

    incomeList << Bank->betArea->items;
    inTotalMVal  +=Bank->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Bank->betArea->totalMoneyItem->uidList);
    inTotalChips +=Bank->betArea->totalChipsItem->getChipsCount();

    incomeList << Play->betArea->items;
    inTotalMVal  +=Play->betArea->totalMoneyItem->getMoney();
    incomeUids.append(Play->betArea->totalMoneyItem->uidList);
    inTotalChips +=Play->betArea->totalChipsItem->getChipsCount();

    ui->realMoneyLabel->setText(QString::number(inTotalMVal));


}

void MainWindow::closeEvent ( QCloseEvent *event )
{


}


