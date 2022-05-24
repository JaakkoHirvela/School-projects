#include "settingsdialog.h"
#include "ui_settingsdialog.h"

enum Type {smear,statfi,compare};

SettingsDialog::SettingsDialog(QWidget *parent,int type) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)

{
    ui->setupUi(this);
    type_ = type;

    database db = database("database.db");
    if(type_ == smear) {
        model = db.retrieveSmearSettings();
    }
    if(type_ == statfi) {
        model = db.retrieveStatfiSettings();
    }
    if(type_ == compare){
        model = db.retrieveCompareSetttings();
    }

    view = new QTableView(this);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    view->setModel(model);
    view->hideColumn(0);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->verticalLayout->addWidget(view);


    view->show();

}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::accept()
{

    if(type_ == smear){
        getSmearValues();

    }
    if(type_ == statfi) {
        getStatfiValues();
    }
    if(type_ == compare){
        getCompareValues();
    }


}

void SettingsDialog::getSmearValues()
{
    QString stations = view->selectionModel()->selectedRows(1).value(0).data().toString();
    QDateTime startDate = changeDateFormat(view->selectionModel()->selectedRows(2).value(0).data().toString());
    QDateTime endDate = QDateTime::fromString(view->selectionModel()->selectedRows(3).value(0).data().toString());
    QString gas = view->selectionModel()->selectedRows(4).value(0).data().toString();
    QStringList stationsList = stations.split("-");

    setSmearValues(stationsList,startDate,endDate,gas);
    emit settingsFinished(smear_,statfi_);

    this->close();
}

void SettingsDialog::getStatfiValues()
{
    QDateTime startDate = changeDateFormat(view->selectionModel()->selectedRows(1).value(0).data().toString());
    QDateTime endDate = changeDateFormat(view->selectionModel()->selectedRows(2).value(0).data().toString());
    QString dataset  = view->selectionModel()->selectedRows(3).value(0).data().toString();
    QStringList datasetList = dataset.split("-");

    setStatfiValues(startDate,endDate,datasetList);
    emit settingsFinished(smear_,statfi_);

    this->close();

}

void SettingsDialog::getCompareValues()
{
    QString stations = view->selectionModel()->selectedRows(1).value(0).data().toString();
    QDateTime SMEARstartDate = changeDateFormat(view->selectionModel()->selectedRows(2).value(0).data().toString());
    QDateTime SMEARendDate = changeDateFormat(view->selectionModel()->selectedRows(3).value(0).data().toString());
    QString gas = view->selectionModel()->selectedRows(4).value(0).data().toString();
    QStringList stationsList = stations.split("-");

    setSmearValues(stationsList,SMEARstartDate,SMEARendDate,gas);

    QDateTime startDate = changeDateFormat(view->selectionModel()->selectedRows(5).value(0).data().toString());
    QDateTime endDate = changeDateFormat(view->selectionModel()->selectedRows(6).value(0).data().toString());
    QString datasets  = view->selectionModel()->selectedRows(7).value(0).data().toString();
    QStringList datasetsList = datasets.split("-");

    setStatfiValues(startDate, endDate, datasetsList);
    emit settingsFinished(smear_,statfi_);

    this->close();
}

void SettingsDialog::setSmearValues(QStringList stations,QDateTime startDate, QDateTime endDate, QString gas)
{
    smear_.stations = stations;
    smear_.start = startDate;
    smear_.end = endDate;
    smear_.gas = gas;
}

void SettingsDialog::setStatfiValues(QDateTime startDate, QDateTime endDate, QStringList datasets)
{
    statfi_.start = startDate;
    statfi_.end = endDate;
    statfi_.dataset = datasets;
}

QDateTime SettingsDialog::changeDateFormat(QString date)
{
    return QDateTime::fromString(date,"yyyy-MM-ddTHH:mm:ss.sss");
}
