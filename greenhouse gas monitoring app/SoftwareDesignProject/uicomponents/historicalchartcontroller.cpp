#include "historicalchartcontroller.h"

HistoricalChartController::HistoricalChartController(QObject *parent)
    : IGraphicController{parent}
{
    statfiService = new StatfiService(this);
    historicalChartModel = new HistoricalChartModel(this);
}
//const std::map<QString, std::shared_ptr<QLineSeries> > HistoricalChartController::getHistoricalData(std::vector<QString> titles, QString startDate, QString endDate)
std::map<QString, QLineSeries*> HistoricalChartController::getHistoricalData(std::vector<QString> titles, QString startDate, QString endDate)
{
    historicalChartModel->setGasValues(statfiService->getHistoricalData(titles, startDate, endDate));
    return historicalChartModel->getGasValues(titles, startDate, endDate);
}

void HistoricalChartController::getTimeSeriesData(QDateTime startDate, QDateTime endDate, std::vector<QString> stations, QString gasType, QString intervalLenght, QString aggregationType)
{

}

