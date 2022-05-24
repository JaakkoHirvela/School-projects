#include "smearservice.h"
SmearService::SmearService(QObject *parent)
    : IService{parent}
{
    smearRepository = new SmearRepository(this);

}

QDateTime SmearService::getStartDate(QString stationName, QString gas)
{
    QString stationId;
    QString tableId;
    QString variableId;
    if( stationName == "Värriö") {
        stationId = "1"; //id of Värriö
        if( gas == "CO2") {
            tableId = "20"; //id of table VAR_EDDY
            variableId = "860"; //id of av_c
        } else if( gas == "SO2") {
            tableId = "11"; //id of table VAR_META
            variableId = "555"; //id of SO2_1
        } else if (gas == "NO") {
            tableId = "11"; //id of table VAR_META
            variableId = "537"; //id of NO_1
        }
    } else if ( stationName == "Hyytiälä") {
        stationId = "2";
        if( gas == "CO2") {
            tableId = "4"; //id of table VAR_META
            variableId = "114"; //id of CO2icos168
        } else if( gas == "SO2") {
            tableId = "4"; //id of table VAR_META
            variableId = "163"; //id of SO2168
        } else if (gas == "NO") {
            tableId = "4"; //id of table VAR_META
            variableId = "147"; //id of NO168
        }
    } else if ( stationName == "Kumpula") {
        stationId = "3";
        if( gas == "CO2") {
            tableId = "8"; //id of table VAR_EDDY
            variableId = "2232"; //id of av_c_ep
        } else if( gas == "SO2") {
            tableId = "2"; //id of table VAR_META
            variableId = "46"; //id of SO_2
        } else if (gas == "NO") {
            tableId = "2"; //id of table VAR_META
            variableId = "44"; //id of NO
        }
    }
    return StringtoQDate(smearRepository->getStartDate(stationId, tableId, variableId));
}

std::shared_ptr<TimeSeriesTable> SmearService::getTimeSeriesData(QDateTime startDate, QDateTime endDate, std::vector<QString> stations, QString gasType, QString intervalLenght, QString aggregationType)
{
    return smearRepository->getTimeSeries(changeDateFormat(startDate), changeDateFormat(endDate), stations, gasType, intervalLenght, aggregationType);
}

QString SmearService::changeDateFormat(QDateTime date)
{
    return  date.toString("yyyy-MM-ddTHH:mm:ss.sss");
}

QDateTime SmearService::StringtoQDate(QString date)
{
    QString format = "yyyy-MM-ddTHH:mm:ss.z";
    return QDateTime::fromString(date,format);
}

