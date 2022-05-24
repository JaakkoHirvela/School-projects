#include "timeseriesrow.h"

TimeSeriesRow::TimeSeriesRow(QJsonObject rowData)
{
    foreach(const QString& key, rowData.keys()) {
        QJsonValue value = rowData.value(key);
        if(key == "samptime") {
            sampleTime_ = value.toString();

        } else {
            stationValues_[key] = value.toDouble();
        }
    }
}

const std::map<QString, float> TimeSeriesRow::getStationValues()
{
    return stationValues_;
}

const QString TimeSeriesRow::getSampleTime()
{
    return sampleTime_;
}

