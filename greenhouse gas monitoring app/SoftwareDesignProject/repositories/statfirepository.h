#ifndef STATFIREPOSITORY_H
#define STATFIREPOSITORY_H

#include "apimodels/statfimodel.h"
#include "apimodels/historicalmodel.h"
#include "irepository.h"
#include <vector>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>

class StatfiRepository : virtual public IRepository
{
    Q_OBJECT
public:
    explicit StatfiRepository(QObject *parent = nullptr);
    QJsonDocument fetchData(QUrl url);
    QJsonDocument postRequest(QUrl url, QByteArray data);
    QByteArray createQuery(QJsonArray values, QJsonArray timerange);

private:
    QNetworkAccessManager* manager_;

};
#endif // STATFIREPOSITORY_H

