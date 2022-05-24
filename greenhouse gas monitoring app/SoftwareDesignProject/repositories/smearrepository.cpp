#include "smearrepository.h"
#include <QApplication>


SmearRepository::SmearRepository(QObject *parent)
    : IRepository{parent}
    ,manager_(new QNetworkAccessManager(this))
{


}

std::map<QString, QString> v = {{"CO2","VAR_EDDY.av_c"},{"SO2","VAR_META.SO2_1"},{"NO","VAR_META.NO_1"}};
std::map<QString, QString> h = {{"CO2","HYY_META.CO2icos168"},{"SO2","HYY_META.SO2168"},{"NO","HYY_META.NO168"}};
std::map<QString, QString> k = {{"CO2","KUM_EDDY.av_c_ep"},{"SO2","KUM_META.SO_2"},{"NO","KUM_META.NO"}};


std::shared_ptr<TimeSeriesTable> SmearRepository::getTimeSeries(QString startDate, QString endDate, std::vector<QString> stations, QString gasType, QString intervalLenght, QString aggregationType)
{
    QString tableVariables = formTableVariableString(stations, gasType);


    QString url = "https://smear-backend.rahtiapp.fi/search/timeseries?aggregation=" + aggregationType + "&interval=" + intervalLenght+ "&from=" + startDate + "&to=" + endDate + tableVariables;
    QNetworkReply* reply = sendRequest(url);
    std::shared_ptr<TimeSeriesTable> timeSeriesTable(new TimeSeriesTable(toJson(reply)));
    return timeSeriesTable;
}

QString SmearRepository::getStartDate(QString stationId, QString tableId, QString variableId)
{
    QString url = "https://smear-backend.rahtiapp.fi/station/" + stationId +"/table/" + tableId + "/variable/" + variableId;
    QNetworkReply* reply = sendRequest(url);
    QJsonDocument jsonDocument = toJson(reply);
    return jsonDocument.object().value("periodStart").toString();
}



QNetworkReply *SmearRepository::sendRequest(QString url)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    QEventLoop loop;
    connect(&manager_, SIGNAL(finished(QNetworkReply*)),&loop, SLOT(quit()));
    QNetworkReply* reply = manager_.get(request);

    loop.exec();

    return reply;

}

QJsonDocument SmearRepository::toJson(QNetworkReply *reply)
{
    QByteArray response = reply->readAll();
    QJsonDocument document = QJsonDocument::fromJson(response);
    reply->deleteLater();

    return document;
}

QString SmearRepository::formTableVariableString(std::vector<QString> stations, QString gasType)
{
    QString tableVariables = "";
    for(unsigned int i=0; i < stations.size(); i++){
           if(stations.at(i) == "Värriö"){
                tableVariables += "&tablevariable=" + v[gasType];
           }
           if(stations.at(i) == "Kumpula") {
                tableVariables += "&tablevariable=" + k[gasType];
           }
           if(stations.at(i) == "Hyytiälä") {
                tableVariables += "&tablevariable=" + h[gasType];
           }
    }
    return tableVariables;
}
