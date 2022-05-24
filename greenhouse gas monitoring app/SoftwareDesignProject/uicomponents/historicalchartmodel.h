#ifndef HISTORICALCHARTMODEL_H
#define HISTORICALCHARTMODEL_H

#include <iostream>
#include <vector>
#include <QString>
#include <QLineSeries>
#include <QDateTime>

class HistoricalChartModel : public QObject
{
    Q_OBJECT
public:
    explicit HistoricalChartModel(QObject *parent = nullptr);
    ~HistoricalChartModel();

    std::map<QString, QLineSeries*> getGasValues(std::vector<QString> titles, const QString &startDate, const QString &endDate) const;
    void setGasValues(const std::map<QString, std::shared_ptr<QLineSeries> > &newGasValues);

private:
    std::vector<QString> stations;
    std::map<QString, std::shared_ptr<QLineSeries>> gasValues;


};

#endif // HISTORICALCHARTMODEL_H
