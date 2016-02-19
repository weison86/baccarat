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
        qDebug()<<"connect DataBase fail";
        return false;
    }
    else
    {
        qDebug()<<"connect DataBase Success";
        return true;
    }


}

#endif // DATABASE_H
