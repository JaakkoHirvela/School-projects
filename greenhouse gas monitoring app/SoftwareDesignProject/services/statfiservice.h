#ifndef STATFISERVICE_H
#define STATFISERVICE_H

#include "repositories/statfirepository.h"
#include "iservice.h"
#include <QObject>

class StatfiService : virtual public IService
{
    Q_OBJECT
public:
    explicit StatfiService(QObject *parent = nullptr);
    std::vector<QString> getTextData();
    std::vector<QString> getYearData();
    QJsonArray createTimerange(QString startDate, QString endDate);
    std::vector<StatfiModel> handlePostRequest(QJsonArray, QJsonArray);
    std::map<QString, std::shared_ptr<QLineSeries>> getHistoricalData(std::vector<QString> titles, QString startDate, QString endDate);

private:
    StatfiRepository *statfiRepository;

};
#endif // STATFISERVICE_H
