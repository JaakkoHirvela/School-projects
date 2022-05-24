#ifndef SMEARREPOSITORY_H
#define SMEARREPOSITORY_H

#include "apimodels/stationmodel.h"
#include "apimodels/timeseriestable.h"
#include "irepository.h"
#include <vector>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>

class SmearRepository : virtual public IRepository
{
    Q_OBJECT
public:
    explicit SmearRepository(QObject *parent = nullptr);
    std::shared_ptr<TimeSeriesTable> getTimeSeries(QString startDate, QString endDate, std::vector<QString> stations, QString gasType, QString intervalLenght, QString aggregationType);
    QString getStartDate(QString stationId, QString tableId, QString variableId);

private:
    QNetworkAccessManager manager_;
    QNetworkReply* sendRequest(QString url);
    QJsonDocument toJson(QNetworkReply* reply);
    QString formTableVariableString(std::vector<QString> stations, QString gasType);


};
#endif // SMEARREPOSITORY_H
