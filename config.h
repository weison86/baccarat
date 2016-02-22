#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG 1

#define MaxAnt 9   //定义最大天线数

#define SERIAL "/dev/ttyS1"
//=======================================================

/*数据库连接 默认参数设置*/
#define HOSTNAME  "192.168.1.61"
#define PORT       3306
#define DATABASENAME "bargainingchipsys"
#define USERNAME      "test"
#define PASSWD        "123"


/* gamblingrecord 数据库*/
#define WIN_TYPE        1  //收入标识
#define LOST_TYPE       2  //赔付标识

#define DESK_ID         1  //桌号
#define USER_ID         "" //用户ID

//=======================================================

/*默认天线号定义*/
#ifndef DEBUG
#define ANT_BANK1     (1 << 0)
#define ANT_BANK2     (1 << 1)
#define ANT_BANK3     (1 << 2)
#define ANT_BANK4     (1 << 3)
#define ANT_BANK5     (1 << 4)
#define ANT_BANK6     (1 << 5)
#define ANT_BANK7     (1 << 6)
#define ANT_BANK8     (1 << 7)

#define ANT_PLAY1     (1 << 8)
#define ANT_PLAY2     (1 << 9)
#define ANT_PLAY3     (1 << 10)
#define ANT_PLAY4     (1 << 11)
#define ANT_PLAY5     (1 << 12)
#define ANT_PLAY6     (1 << 13)
#define ANT_PLAY7     (1 << 14)
#define ANT_PLAY8     (1 << 15)

#define ANT_TIE       (1 << 16)

#define ANT_BANKPAIR  (1 << 17)

#define ANT_PLAYPAIR  (1 << 18)

#define M_ANT         (1 << 19)

#endif
#ifdef DEBUG
#define M_ANT         (11)


#endif



//=======================================================
#define DEF_COR_ITEM     QColor(255, 0, 0, 0)


//=======================================================



#endif // CONFIG_H
