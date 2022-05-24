#include "historicalchartmodel.h"


HistoricalChartModel::HistoricalChartModel(QObject *parent)
{
    this->setParent(parent);
}

HistoricalChartModel::~HistoricalChartModel()
{

}

std::map<QString, QLineSeries*> HistoricalChartModel::getGasValues(std::vector<QString> titles, const QString &startDate, const QString &endDate) const
{
    std::map<QString, QLineSeries*> timeScaledGasValues;

    QDateTime startDateTime;
    QDateTime endDateTime;
    QDateTime earliestDateTime;
    QDateTime latestDateTime;

    startDateTime.setDate(QDate(stoi(startDate.toStdString()), 1, 1));
    endDateTime.setDate(QDate(stoi(endDate.toStdString()), 1, 1));

    for (const QString &title : titles){

        const int valueCount = gasValues.at(title)->count();
        long long startingTime = startDateTime.toMSecsSinceEpoch();
        earliestDateTime.setDate(QDate(gasValues.at(title).get()->at(0).x(), 1, 1));
        latestDateTime.setDate(QDate(gasValues.at(title).get()->at(valueCount-1).x(), 1, 1));
        if (startingTime < earliestDateTime.toMSecsSinceEpoch()){
            startingTime = earliestDateTime.toMSecsSinceEpoch();
        }
        if (endDateTime > latestDateTime){
            endDateTime = latestDateTime;
        }
        qint64 dateDifferenceInMs = startDateTime.msecsTo(endDateTime);
        qint64 intervalInMs = dateDifferenceInMs / (valueCount-1);
        QLineSeries *series = new QLineSeries;

        for (int i = 0; i < valueCount; i++){
            auto value = gasValues.at(title)->at(i).y();
            series->append(startingTime, value);
            startingTime += intervalInMs;
        }
        timeScaledGasValues.insert({title, series});
        series = nullptr;
    }
    return timeScaledGasValues;
}

void HistoricalChartModel::setGasValues(const std::map<QString, std::shared_ptr<QLineSeries> > &newGasValues)
{
    gasValues = newGasValues;
}
