#ifndef STATFIMODEL_H
#define STATFIMODEL_H

#include <QString>

class StatfiModel
{
public:
    StatfiModel(QString title, int year, double value);

    const QString &getTitle() const;

    const int &getYear() const;

    const double & getValue() const;

private:
    QString _title;
    int _year;
    double _value;
};

#endif // STATFIMODEL_H
