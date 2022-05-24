#include "realtimechartmodel.h"

RealTimeChartModel::RealTimeChartModel(QObject *parent)
{
    this->setParent(parent);

}

RealTimeChartModel::~RealTimeChartModel()
{
}

void RealTimeChartModel::setStationValues(const std::map<QString, std::shared_ptr<QLineSeries>> &newStationValues)
{
    stationValues = newStationValues;
}

QLineSeries *RealTimeChartModel::getStationValues(const QString &station, const QString &gasType)
{
    std::map<QString, QString> v = {{"CO2","VAR_EDDY.av_c"},{"SO2","VAR_META.SO2_1"},{"NO","VAR_META.NO_1"}};
    std::map<QString, QString> h = {{"CO2","HYY_META.CO2icos168"},{"SO2","HYY_META.SO2168"},{"NO","HYY_META.NO168"}};
    std::map<QString, QString> k = {{"CO2","KUM_EDDY.av_c_ep"},{"SO2","KUM_META.SO_2"},{"NO","KUM_META.NO"}};

    QString key;
    if (station == "Värriö"){
        key = v.at(gasType);
    }
    if (station == "Hyytiälä"){
        key = h.at(gasType);
    }
    if (station == "Kumpula"){
        key = k.at(gasType);
    }

    if (stationValues.find(key) != stationValues.end()){
        return filterAndScale(stationValues.at(key));
    }
    else{
        return nullptr;
    }
}

void RealTimeChartModel::setStartDateTime(const QDateTime &newStartDateTime)
{
    startDateTime = newStartDateTime;
}

void RealTimeChartModel::setEndDateTime(const QDateTime &newEndDateTime)
{
    endDateTime = newEndDateTime;
}

void RealTimeChartModel::setEarliestStartDate(const QDateTime &newStartDateRealTime)
{
    qDebug() << "setEarliestStartDate" << newStartDateRealTime;
    startDates.insert(newStartDateRealTime);
    for( auto date : startDates) {
        qDebug() << date;
    }
    earliestStartDate = *startDates.begin();
    qDebug() << "earliest date" << earliestStartDate;
}

void RealTimeChartModel::deleteAllStartDates()
{
   startDates.clear();
}

const QDateTime RealTimeChartModel::getEarlisestStartDate()
{
    return earliestStartDate;
}

// private
QLineSeries *RealTimeChartModel::filterAndScale(std::shared_ptr<QLineSeries> originalSeries)
{
    QLineSeries *filteredSeries = new QLineSeries;

    qint64 startingTime = startDateTime.toMSecsSinceEpoch();

    qint64 dateDifferenceInMs = startDateTime.msecsTo(endDateTime);

    const int stationValueCount = originalSeries.get()->count();
    qint64 intervalInMs = dateDifferenceInMs / stationValueCount;

    for (int i = 0; i < stationValueCount; i++){
        auto value = originalSeries.get()->at(i).y();
        if(value > 0 && value < 1000){
            filteredSeries->append(startingTime, value);
        }
        startingTime += intervalInMs;
    }
    return filteredSeries;
}

