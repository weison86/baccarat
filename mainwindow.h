#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSqlDatabase>
#include <QLabel>
#include <QCloseEvent>
#include "baccarat.h"
#include "reader.h"

#define MaxAnt 9

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void ReadUid(QList<QByteArray> list);
    void inventory();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_readyBet_clicked();

    void on_startBet_clicked();

    void on_income_clicked();

    void on_output_clicked();

    void on_End_clicked();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    QByteArray SwaptoDataBaseBytes(QByteArray bd);
    int getMoneyVal(QString uid);







    void handleBankWin();
    void handlePlayWin();
    void handleTieWin();
    void handleBank_BankpairWin();
    void handleBank_PlaypairWin();
    void handleBank_Bankpair_PlaypairWin();
    void handlePlay_BankpairWin();
    void handlePlay_PlaypairWin();
    void handlePlay_Bankpair_PlaypairWin();
    void handleTie_BankpairWin();
    void handleTie_PlaypairWin();
    void handleTie_Bankpair_PlaypairWin();
    void setWinArea(QLabel *label);
    void initANT();
    void initlossList(QList<BaccaratItem *> *list);
    void initShowLabel();
    void handleResult(int key);

private:
    Ui::MainWindow *ui;

    QTimer *inventoryTimer;
    BaccaratGamble *BacGamble;

    void setupReader(QString comnum);
    void initParam();
    void initbetList(QList<BaccaratItem *> *list);
    void scheduleNextBet();
    QList<BaccaratItem *> betList;
    QList<BaccaratItem *> lossList;
    QList<BaccaratItem *> bettestList;
    QList<BaccaratItem *> losstestList;

    QList<BaccaratItem *> currentwinList;
    QList<BaccaratItem *> currentlossList;

    QList<BaccaratItem *> incomeList;
    QList<BaccaratItem *> outputList;
    quint32 outTotalMVal;
    quint32 outTotalChips;
    quint32 inTotalChips;
    quint32 inTotalMVal;
    BaccaratItem *currentItem;
    quint32 currentIndex;
    virtual void keyReleaseEvent (QKeyEvent * event);
    virtual void closeEvent( QCloseEvent *event );
 /*=============================================================================*/
    BaccaratArea *Bank;
    BaccaratArea *Play;
    BaccaratArea *Tie;
    BaccaratArea *Bankpair;
    BaccaratArea *Playpair;

    BaccaratDesk *Desk;

    void attachTableWidget(BaccaratDesk *desk);
    void attachBaccaratArea(BaccaratArea *area);
    void attachTypeArea(TypeArea *type);
    void attachItems(QList<BaccaratItem *> items, int rownum);

    void setupParam(BaccaratDesk *desk);
    void setupDesk();
    void setupList(BaccaratArea *area);
    void apendbetList(QList<BaccaratItem *> list);
    void apendlossList(QList<BaccaratItem *> list);

    void setBaccaratGambleResult(BaccaratResultType Result);
    BaccaratResultType  getBaccaratGambleResult();
    void setBaccaratGambleState( BaccaratGambleState state);
    BaccaratGambleState getBaccaratGambleState();

    void nextItem();
/*=============================================================================*/
    Reader  *reader;
    QList<int>keyList;
    void resetItem();
    void handleBaccaratResult();

    QPalette winpe;
};

#endif // MAINWINDOW_H
