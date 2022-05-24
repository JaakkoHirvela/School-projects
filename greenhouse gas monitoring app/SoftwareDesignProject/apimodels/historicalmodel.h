#ifndef HISTORICALMODEL_H
#define HISTORICALMODEL_H

#include "statfimodel.h"
#include <QLineSeries>


class HistoricalModel
{
public:
    HistoricalModel();
    std::vector<StatfiModel> cleanData(std::vector<StatfiModel>);
    std::map<QString, std::shared_ptr<QLineSeries>> toLineSeries(std::vector<StatfiModel>);
};

#endif // HISTORICALMODEL_H
