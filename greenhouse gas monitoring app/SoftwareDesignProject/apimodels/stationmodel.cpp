#include "stationmodel.h"


StationModel::StationModel(QString dcmiPoint, int id, QString name)
{
    _dcmiPoint = dcmiPoint;
    _id = id;
    _name = name;

}

const QString &StationModel::getDcmiPoint() const
{
    return _dcmiPoint;
}

int StationModel::getId() const
{
    return _id;
}

const QString &StationModel::getName() const
{
    return _name;
}
