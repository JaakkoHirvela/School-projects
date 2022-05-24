#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <QString>
#include <QDateTime>
#include <QSql>
#include <QSqlQuery>
#include <QList>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QTableView>
#include <QHeaderView>
#include <QFile>

struct SMEARSettings{
    QStringList stations;
    QDateTime start;
    QDateTime end;
    QString gas;
};

struct STATFISettings {
    QDateTime start;
    QDateTime end;
    QStringList dataset;

};

class database
{
public:
    database(const QString& path);
    bool saveSmearSettings(QStringList stations, QDateTime* start, QDateTime* end, QString gas );
    QSqlTableModel* retrieveSmearSettings();
    bool saveStatfiSettings(QDateTime* start, QDateTime* end, QStringList datasets);
    bool saveCompareSettings(QStringList stations, QDateTime* SMEARstart, QDateTime* SMEARend, QString gas,QDateTime* start, QDateTime* end, QStringList datasets);
    QSqlTableModel* retrieveStatfiSettings();
    QSqlTableModel* retrieveCompareSetttings();


private:
    void createTables();
    QString formatString(QStringList values);
    QString path_;
};

#endif // DATABASE_H
