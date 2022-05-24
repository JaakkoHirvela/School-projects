#ifndef IGRAPHICCONTROLLER_H
#define IGRAPHICCONTROLLER_H

#include <QObject>
#include <QLineSeries>
/*
 * Interface-class for UI's controllers.
 *
*/
class IGraphicController : public QObject
{

public:
    explicit IGraphicController(QObject *parent = 0) : QObject(parent) {};
    virtual ~IGraphicController(){};

    // Nämä vois yhdistää
    virtual void getTimeSeriesData(QDateTime startDate, QDateTime endDate, std::vector<QString> stations, QString gasType, QString intervalLenght, QString aggregationType) = 0;
    //virtual std::map<const QString&, QLineSeries*> getHistoricalData(std::vector<QString> titles, QString startDate, QString endDate) = 0;
signals:
};
#endif // IGRAPHICCONTROLLER_H
