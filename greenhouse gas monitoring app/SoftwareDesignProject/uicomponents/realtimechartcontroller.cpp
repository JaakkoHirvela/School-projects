#include "realtimechartcontroller.h"

RealTimeChartController::RealTimeChartController(QObject *parent, RealTimeChartModel *model)
    : IGraphicController{parent}
{
    realTimeChartModel = model;
    smearService = new SmearService(this);
}

void RealTimeChartController::getTimeSeriesData(QDateTime startDate, QDateTime endDate, std::vector<QString> stations, QString gasType, QString intervalLenght, QString aggregationType) //parametrit käyttöliitymästä
{
    QDateTime earliestStartDate;
    earliestStartDate = realTimeChartModel->getEarlisestStartDate();
    if (startDate < earliestStartDate){
        startDate = earliestStartDate;
    }
    std::shared_ptr<TimeSeriesTable> timeSeriesTable = smearService->getTimeSeriesData(startDate, endDate, stations, gasType, intervalLenght, aggregationType); //parametrit käyttöliitymästä
    if (timeSeriesTable.get()->getData().size() > 0){
        realTimeChartModel->setStationValues(timeSeriesTable.get()->toLineSeries());
    }
}

void RealTimeChartController::addStartDate(QString station, QString gas)
{
    qDebug() << "addStartDate" << station << gas;
    realTimeChartModel->setEarliestStartDate(smearService->getStartDate(station, gas));
}

void RealTimeChartController::deleteOldStartDates()
{
    realTimeChartModel->deleteAllStartDates();
}

