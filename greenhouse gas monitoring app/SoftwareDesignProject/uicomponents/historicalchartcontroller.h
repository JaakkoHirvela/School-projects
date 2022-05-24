#ifndef HISTORICALCHARTCONTROLLER_H
#define HISTORICALCHARTCONTROLLER_H

#include "services/statfiservice.h"
#include "igraphiccontroller.h"
#include "historicalchartmodel.h"

class HistoricalChartController : virtual public IGraphicController
{
    Q_OBJECT
public:
    explicit HistoricalChartController(QObject *parent = nullptr);
    std::map<QString, QLineSeries*> getHistoricalData(std::vector<QString> titles,  QString startDate, QString endDate);

    // From interface:
    void getTimeSeriesData(QDateTime startDate, QDateTime endDate, std::vector<QString> stations, QString gasType, QString intervalLenght, QString aggregationType);

private:
    StatfiService *statfiService;
    HistoricalChartModel *historicalChartModel;
};

#endif // HISTORICALCHARTCONTROLLER_H
