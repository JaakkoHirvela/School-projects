#ifndef TIMESERIESROW_H
#define TIMESERIESROW_H

#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <iostream>
#include <QLineSeries>

class TimeSeriesRow
{
public:
    TimeSeriesRow(QJsonObject rowData);

    const std::map<QString, float> getStationValues();
    const QString getSampleTime();


private:

    QString sampleTime_;
    std::map<QString, float> stationValues_;

};

#endif // TIMESERIESROW_H
