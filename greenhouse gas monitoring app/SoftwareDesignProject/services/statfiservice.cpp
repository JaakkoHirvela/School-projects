#include "statfiservice.h"

StatfiService::StatfiService(QObject *parent)
{
    statfiRepository = new StatfiRepository(this);
}

std::vector<QString> StatfiService::getYearData() {
    // Fetch the year data from the api and save it
    auto url = "https://pxnet2.stat.fi/PXWeb/api/v1/en/ymp/taulukot/Kokodata.px";
    QJsonDocument document = statfiRepository->fetchData(QUrl(url));

    QJsonObject years = document["variables"][1].toObject();
    QJsonArray yearsArray = years["values"].toArray();

    // Save the first and last years stored in the api
    std::vector<QString> yearLimits;

    auto it = yearsArray.begin();
    QString min = it->toString();
    yearLimits.push_back(min);

    it = yearsArray.end()-1;
    QString max = it->toString();
    yearLimits.push_back(max);

    return yearLimits;
}

QJsonArray StatfiService::createTimerange(QString startDate, QString endDate) {
    QJsonArray timerange;

    // Get the timerange limits from the fetched data
    std::vector<QString> yearLimits = getYearData();
    int min = yearLimits[0].toInt();
    int max = yearLimits[1].toInt();

    // Add years into the given timerange that fit the limit
    for(int year = startDate.toInt(); year < endDate.toInt(); year++) {
        if(year >= min && year <= max) {
            timerange.push_back(year);
        }
    }

    if(endDate.toInt() <= max) {
        timerange.push_back(endDate.toInt());
    }

    return timerange;
}

std::vector<StatfiModel> StatfiService::handlePostRequest(QJsonArray titles, QJsonArray timerange) {
    QString url = "https://pxnet2.stat.fi/PXWeb/api/v1/en/ymp/taulukot/Kokodata.px";

    // Create a query with parameters "titles, timerange", get the response from post rq
    QByteArray query = statfiRepository->createQuery(titles, timerange);
    QJsonDocument response = statfiRepository->postRequest(QUrl(url), query);

    QJsonArray data = response["value"].toArray();

    std::vector<StatfiModel> v;

    // Index for the response data
    int x = 0;

    // Go through every year in the timerange for each value
    for(int i = 0; i < titles.size(); i++) {
        for(int j = 0; j < timerange.size(); j++) {
            QString title = titles[i].toString();
            int year = timerange[j].toInt();
            double value = data[x].toDouble();

            // Push model data to a vector
            StatfiModel model = StatfiModel(title, year, value);
            v.push_back(model);

            x++;

        }
    }
    return v;
}

std::map<QString, std::shared_ptr<QLineSeries>> StatfiService::getHistoricalData(std::vector<QString> columnTitles,  QString startDate, QString endDate) {
    QJsonArray titles;

    foreach(const QString title, columnTitles) {
        titles.push_back(title);
    }

    // Create timerange with the given limits
    QJsonArray timerange = createTimerange(startDate, endDate);

    // Get the post request data
    std::vector<StatfiModel> rawData = handlePostRequest(titles, timerange);

    // Clean the data from empty values and turn it into a form that is suitable for the UI
    std::shared_ptr<HistoricalModel> hm;
    rawData = hm->cleanData(rawData);
    std::map<QString, std::shared_ptr<QLineSeries>> uiData = hm->toLineSeries(rawData);

    return uiData;
}
