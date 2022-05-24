#include "timeseriestable.h"

TimeSeriesTable::TimeSeriesTable(QJsonDocument document)
{
    QJsonObject tableData = document.object();
    aggregation = tableData.value("aggregation").toString();
    aggregationInterval = tableData.value("aggregationInterval").toInt();
    QJsonArray columnsArray = tableData.value("columns").toArray();
    for( auto column : columnsArray) {
        columns.push_back(column.toString());
    }
    addRows(tableData.value("data").toArray());
    endTime = tableData.value("endTime").toString();
    recordCount = tableData.value("recordCount").toInteger();
    startTime = tableData.value("startTime").toString();
}
void TimeSeriesTable::addRows(QJsonArray rowData)
{
    for ( auto row : rowData) {
        data.push_back(TimeSeriesRow(row.toObject()));
    }
}

QString TimeSeriesTable::getAggregation()
{
    return aggregation;
}

const std::vector<TimeSeriesRow> TimeSeriesTable::getData() const
{
    return data;
}

std::map<QString, std::shared_ptr<QLineSeries>> TimeSeriesTable::toLineSeries()
{
    std::map<QString, std::shared_ptr<QLineSeries>> m;
    for(QString stationName : columns) {
        qDebug() << stationName;
        std::shared_ptr<QLineSeries> qls(new QLineSeries());
        m.insert({stationName, qls});
    }
    int time = 0;
    for(TimeSeriesRow timeSeriesRow : data) {
        std::map<QString, float> stations = timeSeriesRow.getStationValues();
        //qDebug()<< timeSeriesRow.getSampleTime();
        //iiirota aika
        for( std::pair<QString, float> stationValue : stations) {
            //qDebug() << stationValue.first << "|" << stationValue.second;
            m.at(stationValue.first).get()->append(time, stationValue.second);
        }
        time += aggregationInterval;
    }
    return m;
}

void TimeSeriesTable::setAggregationInterval(int newAggregationInterval)
{
    aggregationInterval = newAggregationInterval;
}

const QString &TimeSeriesTable::getEndTime() const
{
    return endTime;
}

int TimeSeriesTable::getRecordCount() const
{
    return recordCount;
}

const QString &TimeSeriesTable::getStartTime() const
{
    return startTime;
}

int TimeSeriesTable::getAggregationInterval()
{
    return aggregationInterval;
}

const std::vector<QString> &TimeSeriesTable::getColumns() const
{
    return columns;
}



