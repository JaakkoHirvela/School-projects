#ifndef STATIONMODEL_H
#define STATIONMODEL_H

#include <iostream>
#include <QString>

class StationModel
{
public:
    StationModel(QString dcmiPoint, int id, QString name);

    const QString &getDcmiPoint() const;

    int getId() const;

    const QString &getName() const;

private:
    QString _dcmiPoint;
    int _id;
    QString _name;

};

#endif // STATIONMODEL_H
