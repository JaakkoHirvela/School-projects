#ifndef ISERVICE_H
#define ISERVICE_H

#include <QObject>
#include "repositories/irepository.h"

class IService : public QObject
{
    Q_OBJECT
public:
    explicit IService(QObject *parent = 0) : QObject(parent) {};
    virtual ~IService(){};


signals:



};

#endif // ISERVICE_H
