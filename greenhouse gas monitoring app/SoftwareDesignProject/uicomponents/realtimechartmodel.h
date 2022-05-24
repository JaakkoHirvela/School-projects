#ifndef REALTIMECHARTMODEL_H
#define REALTIMECHARTMODEL_H

#include <iostream>
#include <vector>
#include <set>
#include <QString>
#include <QLineSeries>
#include <QDateTime>

class RealTimeChartModel : public QObject
{
    Q_OBJECT
public:
    explicit RealTimeChartModel(QObject *parent = nullptr);
    ~RealTimeChartModel();

    void setStationValues(const std::map<QString, std::shared_ptr<QLineSeries>> &newStationValues);
    QLineSeries *getStationValues(const QString &station, const QString &gasType);

    void setStartDateTime(const QDateTime &newStartDateTime);
    void setEndDateTime(const QDateTime &newEndDateTime);

    void setEarliestStartDate(const QDateTime &newStartDateRealTime);
    void deleteAllStartDates();
    const QDateTime getEarlisestStartDate();

private:
    QLineSeries *filterAndScale(std::shared_ptr<QLineSeries> originalSeries);

private:
    std::map<QString, std::shared_ptr<QLineSeries>> stationValues;
    QDateTime startDateTime;
    QDateTime endDateTime;
    std::set<QDateTime> startDates;
    QDateTime earliestStartDate;



};

#endif // REALTIMECHARTMODEL_H
