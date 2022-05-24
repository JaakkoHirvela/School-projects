#include "historicalmodel.h"

HistoricalModel::HistoricalModel()
{

}


std::vector<StatfiModel> HistoricalModel::cleanData(std::vector<StatfiModel> rawData) {
    // Go through each value and remove elements that don't have a return value
    for(auto it = rawData.begin(); it != rawData.end(); ++it) {
        double value = it->getValue();

        // Remove the empty element and return the iterator to it's correct place
        if(value == 0) {
            rawData.erase(it);
            it = --it;
        }
    }
    return rawData;
}


std::map<QString, std::shared_ptr<QLineSeries>> HistoricalModel::toLineSeries(std::vector<StatfiModel> data) {
    std::map<QString, std::shared_ptr<QLineSeries>> uiData;

    // Insert titles (as keys) to map with an empty QLS
    for(unsigned int i = 0; i < data.size(); i++) {
        QString key = data[i].getTitle();

        if(uiData.find(key) == uiData.end()) {
            std::shared_ptr<QLineSeries> series(new QLineSeries());
            uiData.insert({key, series});
        }
    }

    // Go through each map key, compare it to rawData titles
    // and insert correct year/value data to corresponding QLS
    for(auto it = uiData.begin(); it != uiData.end(); ++it) {
        for(unsigned int j = 0; j < data.size(); j++) {
            QString key = data[j].getTitle();

            if(it->first == key) {
                int year = data[j].getYear();
                double value = data[j].getValue();

                it->second->append(year, value);
            }
        }
    }
    return uiData;
}
