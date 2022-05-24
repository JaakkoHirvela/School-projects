#ifndef SMEARSERVICE_H
#define SMEARSERVICE_H

#include "repositories/smearrepository.h"
#include "iservice.h"
#include "apimodels/timeseriestable.h"
#include <QObject>
#include <QUrl>
#include <map>
#include <QLineSeries>


class SmearService : virtual public IService
{
    Q_OBJECT
public:
    explicit SmearService(QObject *parent = nullptr);
    QDateTime getStartDate(QString stationName, QString gas);
    std::shared_ptr<TimeSeriesTable> getTimeSeriesData(QDateTime startDate, QDateTime endDate, std::vector<QString> stations,QString gasType,QString intervalLenght, QString aggregationType);

private:
    SmearRepository *smearRepository;
    QString changeDateFormat(QDateTime date);
    QDateTime StringtoQDate(QString);


};


#endif // SMEARSERVICE_H
