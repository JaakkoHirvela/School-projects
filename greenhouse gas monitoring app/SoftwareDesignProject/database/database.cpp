#include "database.h"



database::database(const QString& path)
{

    path_  = path;
    QString dbName = path;
    if( !QFile::exists(dbName)){
        qDebug()<<"Data base doesn't exist....";
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(path_);
        db.open();
        createTables();
    }

}

bool database::saveSmearSettings(QStringList stations, QDateTime* start, QDateTime* end, QString gas)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_);
    db.open();

    QSqlQuery query;
    query.prepare("INSERT INTO SMEAR (stations, start, end, gas) VALUES (:stations,:start,:end,:gas)");

    QString stationsString;
    for(QString station:stations){
        stationsString+= station + "-";
    }
    stationsString.truncate(stationsString.lastIndexOf(QChar('-')));

    query.bindValue(":stations", stationsString);
    query.bindValue(":start", start->toString("yyyy-MM-ddTHH:mm:ss.sss"));
    query.bindValue(":end", end->toString("yyyy-MM-ddTHH:mm:ss.sss"));
    query.bindValue(":gas", gas);


    if(query.exec()){
        return true;
    }
    return false;
}

QSqlTableModel* database::retrieveSmearSettings()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_);
    db.open();

    QSqlTableModel*  model = new QSqlTableModel;
    model->setTable("SMEAR");
    model->select();

    return model;

}

bool database::saveStatfiSettings(QDateTime* start, QDateTime* end, QStringList datasets)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_);
    db.open();

    QString datasetsString;
    for(QString dataset:datasets){
        datasetsString+= dataset + "-";
    }
    datasetsString.truncate(datasetsString.lastIndexOf(QChar('-')));

    QSqlQuery query;
    query.prepare("INSERT INTO STATFI (start, end, dataset) VALUES (:start,:end,:dataset)");
    query.bindValue(":start", start->toString("yyyy-MM-ddTHH:mm:ss.sss"));
    query.bindValue(":end", end->toString("yyyy-MM-ddTHH:mm:ss.sss"));
    query.bindValue(":dataset", datasetsString);


    if(query.exec()){
        return true;
    }
    return false;
}

bool database::saveCompareSettings(QStringList stations, QDateTime* SMEARstart, QDateTime* SMEARend, QString gas,QDateTime* start, QDateTime* end, QStringList datasets)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_);
    db.open();

    QString stationsString = formatString(stations);
    QString datasetsString = formatString(datasets);

    qDebug() << stationsString;
    qDebug() << datasetsString;

    QSqlQuery query;
    query.prepare("INSERT INTO COMPARE (stations, SMEARstart, SMEARend, gas, STATFIstart, STATFIend, dataset)VALUES (:stations,:SMEARstart,:SMEARend,:gas,:start,:end,:datasets)");
    query.bindValue(":stations", stationsString);
    query.bindValue(":SMEARstart", SMEARstart->toString("yyyy-MM-ddTHH:mm:ss.sss"));
    query.bindValue(":SMEARend", SMEARend->toString("yyyy-MM-ddTHH:mm:ss.sss"));
    query.bindValue(":gas", gas);
    query.bindValue(":start", start->toString("yyyy-MM-ddTHH:mm:ss.sss"));
    query.bindValue(":end", end->toString("yyyy-MM-ddTHH:mm:ss.sss"));
    query.bindValue(":datasets", datasetsString);

    if(query.exec()){
        return true;
    }
    return false;

}

QSqlTableModel* database::retrieveStatfiSettings()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_);
    db.open();

    QSqlTableModel*  model = new QSqlTableModel;
    model->setTable("STATFI");
    model->select();

    return model;

}

QSqlTableModel *database::retrieveCompareSetttings()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_);
    db.open();

    QSqlTableModel*  model = new QSqlTableModel;
    model->setTable("COMPARE");
    model->select();

    return model;
}

void database::createTables()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_);
    db.open();

    QSqlQuery smear("CREATE TABLE SMEAR (id integer primary key,stations varchar(255),start varchar(255),end varchar(255),gas varchar(255))");
    smear.exec();
    QSqlQuery statfi("CREATE TABLE STATFI (id integer primary key,start varchar(255),end varchar(255),dataset varchar(255))");
    statfi.exec();
    QSqlQuery compare("CREATE TABLE COMPARE (id integer primary key,stations varchar(255),SMEARstart varchar(255),SMEARend varchar(255),gas varchar(255),STATFIstart varchar(255),STATFIend varchar(255),dataset varchar(255))");
    compare.exec();

}

QString database::formatString(QStringList values)
{
    QString newString;
    for(QString value:values){
        newString += value + "-";
    }
    newString.truncate(newString.lastIndexOf(QChar('-')));
    return newString;
}
