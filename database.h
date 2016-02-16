#ifndef DATABASE_H
#define DATABASE_H



bool connectdatabase(QString hostname,int port ,QString databasename,QString username,QString passwd)
{

    QSqlDatabase ChipDataBase=QSqlDatabase::addDatabase("QMYSQL");
    ChipDataBase.setHostName(hostname);
    ChipDataBase.setPort(port);
    ChipDataBase.setDatabaseName(databasename);
    ChipDataBase.setUserName(username);
    ChipDataBase.setPassword(passwd);
    if( !ChipDataBase.open() )
    {
        qDebug()<<"this,warning,failure";
        return false;
    }
    else
    {
        qDebug()<<"this,ok,success";
        return true;
    }


}

#endif // DATABASE_H
