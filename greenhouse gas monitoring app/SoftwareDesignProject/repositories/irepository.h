#ifndef IREPOSITORY_H
#define IREPOSITORY_H

#include <QObject>

class IRepository : public QObject
{
    Q_OBJECT
public:
    explicit IRepository(QObject *parent = 0) : QObject(parent) {};
    virtual ~IRepository(){};

signals:

};

#endif // IREPOSITORY_H
