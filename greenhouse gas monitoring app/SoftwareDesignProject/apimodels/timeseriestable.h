#ifndef TIMESERIESTABLE_H
#define TIMESERIESTABLE_H

#include "timeseriesrow.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineSeries>


class TimeSeriesTable
{

public:
    TimeSeriesTable(QJsonDocument tableData);
    void addRows(QJsonArray rowData);

    QString getAggregation();
    const std::vector<TimeSeriesRow> getData() const;
    int getAggregationInterval();
    const std::vector<QString> &getColumns() const;

    std::map<QString, std::shared_ptr<QLineSeries>> toLineSeries();
    void setAggregationInterval(int newAggregationInterval);

    const QString &getEndTime() const;

    int getRecordCount() const;

    const QString &getStartTime() const;

private:
    QString aggregation;
    int aggregationInterval;
    std::vector<QString> columns;
    std::vector<TimeSeriesRow> data;
    QString endTime;
    int recordCount;
    QString startTime;


};

#endif // TIMESERIESTABLE_H
