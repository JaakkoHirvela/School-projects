#include "statfimodel.h"

StatfiModel::StatfiModel(QString title, int year, double value) {
    _title = title;
    _year = year;
    _value = value;
}

const QString &StatfiModel::getTitle() const {
    return _title;
}

const int &StatfiModel::getYear() const {
    return _year;
}

const double &StatfiModel::getValue() const {
    return _value;
}
