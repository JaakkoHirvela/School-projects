#ifndef REALTIMECHARTCONTROLLER_H
#define REALTIMECHARTCONTROLLER_H

#include "services/smearservice.h"
#include "realtimechartmodel.h"
#include "igraphiccontroller.h"


class RealTimeChartController : virtual public IGraphicController
{
    Q_OBJECT
public:
    explicit RealTimeChartController(QObject *parent = nullptr, RealTimeChartModel *model = nullptr);
    void getTimeSeriesData(QDateTime startDate, QDateTime endDate, std::vector<QString> stations, QString gasType, QString intervalLenght, QString aggregationType);  
    void addStartDate(QString station, QString gas);
    void deleteOldStartDates();

private:
    QDateTime getStartDates(QString station, QString gas);
    SmearService *smearService;
    RealTimeChartModel *realTimeChartModel;

};

#endif // REALTIMECHARTCONTROLLER_H
