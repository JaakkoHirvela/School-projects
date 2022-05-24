#include "statfirepository.h"

StatfiRepository::StatfiRepository(QObject *parent)
    : IRepository{parent}
    ,manager_(new QNetworkAccessManager(this))
{

}

// Fetch the JSON document
QJsonDocument StatfiRepository::fetchData(QUrl url) {
    // Create a get request
    QNetworkRequest request(url);
    QEventLoop event;
    connect(manager_, SIGNAL(finished(QNetworkReply*)),&event, SLOT(quit()));
    QNetworkReply *reply = manager_->get(request);
    event.exec();

    // Save the response
    QByteArray response = reply->readAll();
    QJsonDocument document = QJsonDocument::fromJson(response);

    return document;

}

QJsonDocument StatfiRepository::postRequest(QUrl url, QByteArray data) {
    // Create a post request
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = manager_->post(request, data);

    QEventLoop event;
    connect(manager_, SIGNAL(finished(QNetworkReply*)),&event, SLOT(quit()));
    event.exec();

    // Read the response and save it
    QByteArray response = reply->readAll();
    QJsonDocument document = QJsonDocument::fromJson(response);

    return document;

}

QByteArray StatfiRepository::createQuery(QJsonArray values, QJsonArray timerange) {
    // JSON element for the query
    QJsonArray query;

    // Add values to the query element
    QJsonObject valueSelection {
        {"filter", "item"},
        {"values", values}
    };

    QJsonObject tiedot {
        {"code", "Tiedot"},
        {"selection", valueSelection}
    };

    query.append(tiedot);

    // Add timerange to the query element
    QJsonObject timeSelection {
        {"filter", "item"},
        {"values", timerange}
    };

    QJsonObject vuosi {
        {"code", "Vuosi"},
        {"selection", timeSelection}
    };

    query.append(vuosi);

    // Combine all the query elements
    QJsonObject main {
        {"query", query}
    };

    // Create a JSON doc from the array
    QJsonDocument doc(main);
    QByteArray data = doc.toJson();

    return data;
}
