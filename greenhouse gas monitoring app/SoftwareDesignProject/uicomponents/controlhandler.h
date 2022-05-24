#ifndef CONTROLHANDLER_H
#define CONTROLHANDLER_H

//#include "../mainwindow.h"

#include <QObject>

class ControlHandler : public QObject
{
    Q_OBJECT
public:
    explicit ControlHandler(QObject *parent = nullptr);
    ~ControlHandler();

    void addControls(std::vector<QWidget*> realTime, std::vector<QWidget*> historical);

private:

private:
    std::vector<QWidget*> realTimeControls;
    std::vector<QWidget*> historicalControls;
};

#endif // CONTROLHANDLER_H
