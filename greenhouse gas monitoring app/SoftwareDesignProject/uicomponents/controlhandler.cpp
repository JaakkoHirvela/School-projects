#include "controlhandler.h"

ControlHandler::ControlHandler(QObject *parent)
    : QObject{parent}
{
}

ControlHandler::~ControlHandler()
{
}

void ControlHandler::addControls(std::vector<QWidget *> realTime, std::vector<QWidget *> historical)
{
    realTimeControls = realTime;
    historicalControls = historical;
}
