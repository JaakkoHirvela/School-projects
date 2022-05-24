#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    realTimeChartModel = new RealTimeChartModel(this);
    realTimeController = new RealTimeChartController(this, realTimeChartModel);
    historicalController = new HistoricalChartController(this);

    chartViews.insert({"left", new QChartView(this)});
    chartViews.insert({"center", new QChartView(this)});
    chartViews.insert({"right", new QChartView(this)});

    // Add manually for right orientation:
    ui->chartLayout->addWidget(chartViews.at("left"));
    ui->chartLayout->addWidget(chartViews.at("center"));
    ui->chartLayout->addWidget(chartViews.at("right"));

    for (auto &pair : chartViews){
        pair.second->hide();
        QSizePolicy sizePol = pair.second->sizePolicy();
        sizePol.setHorizontalStretch(1); // Every chartView stays the same size
        pair.second->setSizePolicy(sizePol);
    }

    initializeDate();
    addConnections();
    updateStations();
    updateAggregationButtons();
    createControls();
    showRealTimeChart();
    updateEarliestDates();
    updateRealTimeRange();

}

MainWindow::~MainWindow()
{
    for (auto &chart : charts){
        delete chart.second;
    }
    delete realTimeStartDate;
    delete realTimeEndDate;
    delete ui;
}

void MainWindow::updateSettings(const std::vector<QString> stations, const QString gas,
                                const QString agg, QDateTime realTimeStart, QDateTime realTimeEnd,
                                QDateTime historicalStart, QDateTime historicalEnd,
                                std::vector<QString> historicalTitles)
{
    if (currentChartType == REALTIME || currentChartType == COMPARE){
        ui->realTimeStartDateEdit->setDateTime(realTimeStart);
        ui->realTimeEndDateEdit->setDateTime(realTimeEnd);
        updateRealTimeRange();

        ui->hyyCheckBox->setChecked(false);
        ui->varCheckBox->setChecked(false);
        ui->kumCheckBox->setChecked(false);
        for (auto &station : stations){
            if (station == "Hyytiälä"){
                ui->hyyCheckBox->setChecked(true);
            }
            if (station == "Värriö"){
                ui->varCheckBox->setChecked(true);
            }
            if (station == "Kumpula"){
                ui->kumCheckBox->setChecked(true);
            }
        }
        updateStations();
        if (gas == "CO2"){
            ui->co2RadioBut->setChecked(true);

        }
        if (gas == "SO2"){
            ui->so2RadioBut->setChecked(true);
        }
        if (gas == "NO"){
            ui->noRadioBut->setChecked(true);
        }
        updateGasType();
        aggregation = "NONE";
        if (agg == "ARITHMETIC"){
            setAggregationToAverage();
        }
        if (agg == "MIN"){
            setAggregationToMinimum();
        }
        if (agg == "MAX"){
            setAggregationToMaximum();
        }
        updateAggregationButtons();
    }
    if (currentChartType == HISTORICAL || currentChartType == COMPARE){
        ui->historicalStartDateEdit->setDateTime(historicalStart);
        ui->historicalEndDateEdit->setDateTime(historicalEnd);
        updateHistoricalTimeRange();

        for (auto control : historicalControls){
            if (control->objectName().contains("Button")
                && control->objectName() != "okButton"){
                QCheckBox *checkBox = static_cast<QCheckBox*>(control);
                checkBox->setChecked(false);
                if (checkBox->objectName() == "ghgEmissionsButton"){
                    if (std::find(historicalTitles.begin(), historicalTitles.end(), "Khk_yht") != historicalTitles.end()){
                        checkBox->setChecked(true);
                    }
                }
                if (checkBox->objectName() == "ghgIndexedButton"){
                    if (std::find(historicalTitles.begin(), historicalTitles.end(), "Khk_yht_index") != historicalTitles.end()){
                        checkBox->setChecked(true);
                    }
                }
                if (checkBox->objectName() == "ghgIntensityButton"){
                    if (std::find(historicalTitles.begin(), historicalTitles.end(), "Khk_yht_las") != historicalTitles.end()){
                        checkBox->setChecked(true);
                    }
                }
                if (checkBox->objectName() == "ghgIntensityIndexedButton"){
                    if (std::find(historicalTitles.begin(), historicalTitles.end(), "Khk_yht_las_index") != historicalTitles.end()){
                        checkBox->setChecked(true);
                    }
                }
            }
        }
    }
}

void MainWindow::createControls()
{
    realTimeControls.push_back(ui->hyyCheckBox);
    realTimeControls.push_back(ui->varCheckBox);
    realTimeControls.push_back(ui->kumCheckBox);
    realTimeControls.push_back(ui->co2RadioBut);
    realTimeControls.push_back(ui->so2RadioBut);
    realTimeControls.push_back(ui->noRadioBut);
    realTimeControls.push_back(ui->avgBut);
    realTimeControls.push_back(ui->minBut);
    realTimeControls.push_back(ui->maxBut);
    realTimeControls.push_back(ui->realTimeStartDateEdit);
    realTimeControls.push_back(ui->realTimeEndDateEdit);
    realTimeControls.push_back(ui->realTimeFromLabel);
    realTimeControls.push_back(ui->realTimeToLabel);

    historicalControls.push_back(ui->historicalStartDateEdit);
    historicalControls.push_back(ui->historicalEndDateEdit);
    historicalControls.push_back(ui->historicalFromLabel);
    historicalControls.push_back(ui->historicalToLabel);

    historicalCheckBoxes = new QButtonGroup(this);

    QCheckBox *ghgEmissionsCheckBox = new QCheckBox();
    ghgEmissionsCheckBox->setText("Greenhouse gas emissions");
    ghgEmissionsCheckBox->setObjectName("ghgEmissionsButton");
    ghgEmissionsCheckBox->setChecked(true);
    historicalControls.push_back(ghgEmissionsCheckBox);
    historicalCheckBoxes->addButton(ghgEmissionsCheckBox);

    QCheckBox *ghgIndexedCheckBox = new QCheckBox();
    ghgIndexedCheckBox->setText("Greenhouse gas emissions,\nindexed");
    ghgIndexedCheckBox->setObjectName("ghgIndexedButton");
    historicalControls.push_back(ghgIndexedCheckBox);
    historicalCheckBoxes->addButton(ghgIndexedCheckBox);

    QCheckBox *ghgIntensityCheckBox = new QCheckBox();
    ghgIntensityCheckBox->setText("Intensity of greenhouse\ngas emissions");
    ghgIntensityCheckBox->setObjectName("ghgIntensityButton");
    historicalControls.push_back(ghgIntensityCheckBox);
    historicalCheckBoxes->addButton(ghgIntensityCheckBox);

    QCheckBox *ghgIntensityIndexedCheckBox = new QCheckBox();
    ghgIntensityIndexedCheckBox->setText("Intensity of greenhouse gases,\nindexed");
    ghgIntensityIndexedCheckBox->setObjectName("ghgIntensityIndexedButton");
    historicalControls.push_back(ghgIntensityIndexedCheckBox);
    historicalCheckBoxes->addButton(ghgIntensityIndexedCheckBox);

    QLabel *dateEditLabel = new QLabel();
    dateEditLabel->setAlignment(Qt::AlignBottom);
    dateEditLabel->setText("Get a breakdown for year:");
    dateEditLabel->setObjectName("dateEditLabel");
    historicalControls.push_back(dateEditLabel);

    QDateEdit *yearlyBreakDownDateEdit = new QDateEdit();
    yearlyBreakDownDateEdit->setAlignment(Qt::AlignBottom);
    yearlyBreakDownDateEdit->setObjectName("yearlyBreakDownDate");
    yearlyBreakDownDateEdit->setDisplayFormat("yyyy");
    yearlyBreakDownDateEdit->setMinimumDate(QDate(2012, 1, 1)); //Minimum date for Värriö
    yearlyBreakDownDateEdit->setMaximumDate(QDate(2021, 12, 31));
    historicalControls.push_back(yearlyBreakDownDateEdit);

    QPushButton *okButton = new QPushButton();
    okButton->setText(" OK ");
    okButton->setObjectName("okButton");
    historicalControls.push_back(okButton);
    connect(okButton, &QPushButton::clicked, this, &MainWindow::fetchYearlyBreakdown);

    for (QWidget *control : historicalControls){
        if (!control->objectName().contains("historical")){
            ui->leftControlLayout->addWidget(control);
        }
        control->hide(); // By default real time controls are shown
    }

}

QChart* MainWindow::createLineChart(std::string chartKey)
{
    QChart *chart;
    if (charts.find(chartKey) == charts.end()){
        chart = new QChart();
        QDateTimeAxis *axisX = new QDateTimeAxis(this);
        QValueAxis *axisY = new QValueAxis(this);
        chart->addAxis(axisX, Qt::AlignBottom);
        chart->addAxis(axisY, Qt::AlignLeft);

        charts.insert({chartKey, chart});
    }
    else{
        chart = charts.at(chartKey);
    }
    return chart;
}

template <typename T>
void MainWindow::createAxes(QLineSeries* series, T minY, T maxY, QDateTime startDate, QDateTime endDate,
                            const std::string &chartName, const QString &axisYTitle, int axisXTicks, QString axisXFormat)
{
    if (series != nullptr){
        QDateTimeAxis *axisX = static_cast<QDateTimeAxis*>(charts.at(chartName)->axes(Qt::Horizontal).at(0));
        QValueAxis *axisY = static_cast<QValueAxis*>(charts.at(chartName)->axes(Qt::Vertical).at(0));

        QDateTime startingDate = startDate;
        QDateTime endingDate = endDate;

        axisX->setRange(startingDate, endingDate);
        axisX->setFormat(axisXFormat);

        if (axisXTicks > 20){ // Compress for very long intervals:
            axisXTicks = 21;
        }
        axisX->setTickCount(axisXTicks);
        axisX->setTitleText("Date");

        axisY->setTickCount(12);
        axisY->setRange(minY, maxY);

        axisY->setLabelFormat("%.1f");
        axisY->setTitleText(axisYTitle);
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }
}

template <typename T>
std::pair<T, T> MainWindow::getMinMax(const QLineSeries &series, const double &spacing)
{
    QVector<QPointF> v = series.points();
    auto maxIt = std::max_element(v.begin(), v.end(), [] (auto x, auto y){
        return x.y() < y.y();
    });
    auto minIt = std::min_element(v.begin(), v.end(), [] (auto x, auto y){
        return x.y() < y.y();
    });
    T max = maxIt->y();
    T min = minIt->y();

    min -= spacing*min;
    max += spacing*max;
    return {min, max};
}

void MainWindow::initializeDate()
{
    qint64 sevenDaysInMs = 7*24*60*60*1000;
    realTimeEndDate = new QDateTime(QDateTime::currentDateTime());
    realTimeStartDate = new QDateTime(QDateTime::fromMSecsSinceEpoch(realTimeEndDate->toMSecsSinceEpoch() - sevenDaysInMs));
    ui->realTimeStartDateEdit->setDate(realTimeStartDate->date());
    ui->realTimeEndDateEdit->setDate(realTimeEndDate->date());

    historicalStartDate = new QDateTime(ui->historicalStartDateEdit->dateTime());
    historicalEndDate = new QDateTime(ui->historicalEndDateEdit->dateTime());

    updateHistoricalTimeRange();
}

void MainWindow::addConnections()
{
    // Time range edit:
    connect(ui->realTimeStartDateEdit, SIGNAL(editingFinished()), this, SLOT(updateRealTimeRange()));
    connect(ui->realTimeEndDateEdit, SIGNAL(editingFinished()), this, SLOT(updateRealTimeRange()));
    connect(ui->historicalStartDateEdit, SIGNAL(editingFinished()), this, SLOT(updateHistoricalTimeRange()));
    connect(ui->historicalEndDateEdit, SIGNAL(editingFinished()), this, SLOT(updateHistoricalTimeRange()));
    // Station checkboxes:
    connect(ui->hyyCheckBox, &QCheckBox::stateChanged, this, &MainWindow::updateStations);
    connect(ui->kumCheckBox, &QCheckBox::stateChanged, this, &MainWindow::updateStations);
    connect(ui->varCheckBox, &QCheckBox::stateChanged, this, &MainWindow::updateStations);
    // Aggregation pushbuttons:
    connect(ui->avgBut, &QPushButton::clicked, this, &MainWindow::setAggregationToAverage);
    connect(ui->minBut, &QPushButton::clicked, this, &MainWindow::setAggregationToMinimum);
    connect(ui->maxBut, &QPushButton::clicked, this, &MainWindow::setAggregationToMaximum);
    // GasType radiobuttons:
    connect(ui->co2RadioBut, &QRadioButton::toggled, this, &MainWindow::updateGasType);
    connect(ui->so2RadioBut, &QRadioButton::toggled, this, &MainWindow::updateGasType);
    connect(ui->noRadioBut, &QRadioButton::toggled, this, &MainWindow::updateGasType);
    // Menubar options:
    connect(ui->actionSaveCurrentSettings, &QAction::triggered, this, &MainWindow::saveCurrentSettings);

    connect(ui->realTimeBut, &QPushButton::clicked, this, &MainWindow::showRealTimeChart);
    connect(ui->historicalBut, &QPushButton::clicked, this, &MainWindow::showHistoricalChart);
    connect(ui->compareBut, &QPushButton::clicked, this, &MainWindow::showCompareCharts);
    connect(ui->applyBut, &QPushButton::clicked, this, &MainWindow::applyAndRefresh);

}

void MainWindow::showRealTimeChart()
{
    currentChartType = REALTIME;
    ui->historicalBut->setChecked(false);
    ui->compareBut->setChecked(false);
    for (QWidget *control : realTimeControls){
        control->show();
    }
    for (QWidget *control : historicalControls){
        control->hide();
    }

    chartViews.at("center")->hide();
    chartViews.at("right")->hide();
    chartViews.at("left")->show();
}

void MainWindow::updateRealTimeChart()
{
    ui->applyBut->setDisabled(true);

    realTimeController->getTimeSeriesData(*realTimeStartDate, *realTimeEndDate, stations, gasType, intervalLength, aggregation);

    createLineChart("REALTIME");
    charts.at("REALTIME")->removeAllSeries();

    QLineSeries allSeries;
    std::vector <QLineSeries*> seriesVec;
    for (auto &station : stations){
        QLineSeries *series = realTimeChartModel->getStationValues(station, gasType);
        if (series != nullptr){
            drawAndShowChart("REALTIME", "left", series, station);
            allSeries.append(series->points());
            seriesVec.push_back(series);
        }
    }
    if (allSeries.points().size() != 0){
        QString titleText;
        int axisXTicks = (realTimeEndDate->toMSecsSinceEpoch() - realTimeStartDate->toMSecsSinceEpoch())
                        / (24*60*60*1000); // Date difference converted from ms to days
        if (gasType == "CO2"){
            titleText = gasType + " (ppm)";
            std::pair<int, int> minMax = getMinMax<int>(allSeries, 0.005);
            for (auto series : seriesVec){
                createAxes(series, minMax.first, minMax.second, *realTimeStartDate, *realTimeEndDate, "REALTIME", titleText, axisXTicks);
            }
        }
        else{
            titleText = gasType + " (ppb)";
            std::pair<double, double> minMax = getMinMax<double>(allSeries, 0.005);
            for (auto series : seriesVec){
                createAxes(series, minMax.first, minMax.second, *realTimeStartDate, *realTimeEndDate, "REALTIME", titleText, axisXTicks);
            }
        }
    }
    ui->applyBut->setDisabled(false);
}

void MainWindow::showHistoricalChart()
{
    currentChartType = HISTORICAL;
    ui->realTimeBut->setChecked(false);
    ui->compareBut->setChecked(false);
    ui->bufferLabel->hide();
    historicalCheckBoxes->setExclusive(false);
    for (QWidget *control : realTimeControls){
        control->hide();
    }
    for (QWidget *control : historicalControls){
        if (!control->objectName().contains("historical")){
            ui->leftControlLayout->addWidget(control);
        }
        control->show();
    }
    for (auto &chart : chartViews){
        chart.second->hide();
    }
    chartViews.at("right")->show();
}



void MainWindow::updateHistoricalChart()
{
    const std::vector<QString> columnTitles = getColumnTitles();
    int axisXTicks = (historicalEndDate->date().year() - historicalStartDate->date().year()+1);
    QDateTime startDate = *historicalStartDate;
    QDateTime endDate = *historicalEndDate;

    if (columnTitles.size() > 1){
        if (axisXTicks > 10){
            axisXTicks = 10;
        }
    }
    for (auto &chart : charts){
        chart.second->removeAllSeries();
    }
    ui->applyBut->setDisabled(true);
    std::map<QString, QLineSeries*> timeScaledGasValues = historicalController->getHistoricalData(getColumnTitles(),
                QString::number(historicalStartDate->date().year()), QString::number(historicalEndDate->date().year()));
    for (auto &chartView : chartViews){
        chartView.second->hide();
    }
    if (std::find(columnTitles.begin(), columnTitles.end(), "Khk_yht") != columnTitles.end()){
        drawAndShowChart("HISTORICAL_EMISSIONS", "right", timeScaledGasValues.at("Khk_yht"), "Greenhouse gas emissions");
        std::pair<int, int> minMax = getMinMax<int>(*timeScaledGasValues.at("Khk_yht"), 0.01);
        createAxes(timeScaledGasValues.at("Khk_yht"), minMax.first, minMax.second, startDate, endDate,
                   "HISTORICAL_EMISSIONS", "CO2 equivalent 1000t", axisXTicks, "yyyy");
    }
    if (std::find(columnTitles.begin(), columnTitles.end(), "Khk_yht_las") != columnTitles.end()){
        drawAndShowChart("HISTORICAL_INTENSITY", "center", timeScaledGasValues.at("Khk_yht_las"), "Intensity of emissions");
        std::pair<int, int> minMax = getMinMax<int>(*timeScaledGasValues.at("Khk_yht_las"), 0.01);
        createAxes(timeScaledGasValues.at("Khk_yht_las"), minMax.first, minMax.second, startDate, endDate,
                   "HISTORICAL_INTENSITY","", axisXTicks, "yyyy");
    }
    QLineSeries *allIndexed = new QLineSeries();
    std::string chartViewKey = "left";
    if (currentChartType == COMPARE){
        chartViewKey = "right"; // Real time charts are drawn to "left" chartView.
    }
    if (std::find(columnTitles.begin(), columnTitles.end(), "Khk_yht_index") != columnTitles.end()){
        drawAndShowChart("HISTORICAL_INDEXED", chartViewKey, timeScaledGasValues.at("Khk_yht_index"), "Emissions indexed");
        std::pair<int, int> minMax = getMinMax<int>(*timeScaledGasValues.at("Khk_yht_index"), 0.01);
        createAxes(timeScaledGasValues.at("Khk_yht_index"), minMax.first, minMax.second, startDate, endDate,
                   "HISTORICAL_INDEXED", "year 1990 = 100", axisXTicks, "yyyy");
        allIndexed->append(timeScaledGasValues.at("Khk_yht_index")->points());
    }
    if (std::find(columnTitles.begin(), columnTitles.end(), "Khk_yht_las_index") != columnTitles.end()){
        drawAndShowChart("HISTORICAL_INDEXED", chartViewKey, timeScaledGasValues.at("Khk_yht_las_index"),
                            "Intensity of emissions, indexed");
        allIndexed->append(timeScaledGasValues.at("Khk_yht_las_index")->points());
        std::pair<int, int> minMax = getMinMax<int>(*allIndexed, 0.01);
        createAxes(timeScaledGasValues.at("Khk_yht_las_index"), minMax.first, minMax.second, startDate, endDate,
                   "HISTORICAL_INDEXED", "year 1990 = 100", axisXTicks, "yyyy");
    }
    delete allIndexed;
    ui->applyBut->setDisabled(false);
}

void MainWindow::drawAndShowChart(std::string chartName, std::string chartViewName,
                                     QLineSeries *series, QString legendName)
{
    chartViews.at(chartViewName)->setChart(createLineChart(chartName));
    chartViews.at(chartViewName)->show();
    series->setName(legendName);
    charts.at(chartName)->addSeries(series);
}

const std::vector<QString> MainWindow::getColumnTitles()
{
    std::vector<QString> titles;
    for (QWidget *control : historicalControls){
        if (control->objectName().contains("Button") &&
                control->objectName() != "okButton"){
            QCheckBox *checkBox = static_cast<QCheckBox*>(control);
            if (checkBox->objectName() == "ghgEmissionsButton"){
                if (checkBox->isChecked()){
                    titles.push_back("Khk_yht");
                }
            }
            if (checkBox->objectName() == "ghgIndexedButton"){
                if (checkBox->isChecked()){
                    titles.push_back("Khk_yht_index");
                }
            }
            if (checkBox->objectName() == "ghgIntensityButton"){
                if (checkBox->isChecked()){
                    titles.push_back("Khk_yht_las");
                }
            }
            if (checkBox->objectName() == "ghgIntensityIndexedButton"){
                if (checkBox->isChecked()){
                    titles.push_back("Khk_yht_las_index");
                }
            }
        }
    }
    return titles;
}

void MainWindow::showCompareCharts()
{
    currentChartType = COMPARE;
    ui->realTimeBut->setChecked(false);
    ui->historicalBut->setChecked(false);
    ui->bufferLabel->show();
    historicalCheckBoxes->setExclusive(true); // Turn checkboxes into radiobuttons
    for (auto &control : historicalControls){
        if (!control->objectName().contains("historical")){
            ui->rightControlLayout->addWidget(control);
            control->hide();
        }
        else{
            control->show();
        }
        if (control->objectName().contains("Button") && control->objectName() != "okButton"){
            control->show();
            QCheckBox *checkBox = static_cast<QCheckBox*>(control);
            checkBox->setChecked(false);
        }
    }
    for (auto &control : realTimeControls){
        control->show();
    }
    chartViews.at("left")->show();
    chartViews.at("center")->hide();
    chartViews.at("right")->show();
}

void MainWindow::applyAndRefresh()
{
    switch (currentChartType){
        case REALTIME:
            updateRealTimeChart();
            break;
        case HISTORICAL:
            updateHistoricalChart();
            break;
        case COMPARE:
            updateHistoricalChart();
            chartViews.at("left")->show();
            updateRealTimeChart();
            break;
    }
}

void MainWindow::updateRealTimeRange()
{
    QDate start = ui->realTimeStartDateEdit->date();
    ui->realTimeEndDateEdit->setMinimumDate(start.addDays(1)); // Time range can't be zero.
    QDate end = ui->realTimeEndDateEdit->date();
    ui->realTimeStartDateEdit->setMaximumDate(end);
    ui->realTimeStartDateEdit->setMinimumDate(realTimeChartModel->getEarlisestStartDate().date());

    realTimeStartDate->setDate(start);
    realTimeEndDate->setDate(end);
    realTimeChartModel->setStartDateTime(*realTimeStartDate);
    realTimeChartModel->setEndDateTime(*realTimeEndDate);
}

void MainWindow::updateHistoricalTimeRange()
{
    QDate start = ui->historicalStartDateEdit->date();
    ui->historicalEndDateEdit->setMinimumDate(start);
    QDate end = ui->historicalEndDateEdit->date();
    ui->historicalStartDateEdit->setMaximumDate(end);

    historicalStartDate->setDate(start);
    historicalEndDate->setDate(end);
}

void MainWindow::updateStations()
{
    std::vector<QString> newStations;
    if (ui->hyyCheckBox->isChecked()){
        newStations.push_back("Hyytiälä");
    }
    if (ui->kumCheckBox->isChecked()){
        newStations.push_back("Kumpula");
    }
    if (ui->varCheckBox->isChecked()){
        newStations.push_back("Värriö");
    }
    stations = newStations;
}

void MainWindow::setAggregationToAverage()
{
    if (aggregation == "ARITHMETIC"){
        aggregation = "NONE";
    }
    else{
        aggregation = "ARITHMETIC";
    }
    updateAggregationButtons();
}

void MainWindow::setAggregationToMinimum()
{
    if (aggregation == "MIN"){
        aggregation = "NONE";
    }
    else{
        aggregation = "MIN";
    }
    updateAggregationButtons();
}

void MainWindow::setAggregationToMaximum()
{
    if (aggregation == "MAX"){
        aggregation = "NONE";
    }
    else{
        aggregation = "MAX";
    }
    updateAggregationButtons();
}

void MainWindow::updateAggregationButtons()
{
    QPushButton* avgBut = ui->avgBut;
    QPushButton* minBut = ui->minBut;
    QPushButton* maxBut = ui->maxBut;

    if (aggregation == "ARITHMETIC"){
        maxBut->setChecked(false);
        minBut->setChecked(false);
    }
    else if (aggregation == "MIN"){
        avgBut->setChecked(false);
        maxBut->setChecked(false);
    }
    else if (aggregation == "MAX"){
        avgBut->setChecked(false);
        minBut->setChecked(false);
    }
    else{
        avgBut->setChecked(false);
        minBut->setChecked(false);
        maxBut->setChecked(false);
    }
}

void MainWindow::updateGasType()
{
    if (ui->co2RadioBut->isChecked()){
        gasType = "CO2";
        intervalLength = "60";
    }
    if (ui->so2RadioBut->isChecked()){
        gasType = "SO2";
        intervalLength = "30";
    }
    if (ui->noRadioBut->isChecked()){
        gasType = "NO";
        intervalLength = "30";
    }
    updateEarliestDates();
    updateRealTimeRange();
}

void MainWindow::updateEarliestDates()
{
    realTimeController->deleteOldStartDates();
    realTimeController->addStartDate("Hyytiälä", gasType);
    realTimeController->addStartDate("Kumpula", gasType);
    realTimeController->addStartDate("Värriö", gasType);
}

void MainWindow::saveCurrentSettings()
{
    database db = database("database.db");
    bool savingSucceeded;
    QStringList s;
    s.reserve(stations.size());
    std::copy(stations.begin(), stations.end(), std::back_inserter(s));
    QStringList list;
    list.reserve(getColumnTitles().size());
    for (auto column : getColumnTitles()){
        list.append(statfiOptions.find(column)->second);
    }
    if(currentChartType == REALTIME) { 
        savingSucceeded = db.saveSmearSettings(s,realTimeStartDate, realTimeEndDate, gasType);
    }
    else if(currentChartType == HISTORICAL) {
        QStringList list;
        list.reserve(getColumnTitles().size());
        for (auto &column : getColumnTitles()){
            list.append(statfiOptions.find(column)->second);
        }

        savingSucceeded = db.saveStatfiSettings(historicalStartDate, historicalEndDate,list);
    }
    else{
         savingSucceeded = db.saveCompareSettings(s,realTimeStartDate, realTimeEndDate, gasType,historicalStartDate, historicalEndDate,list);
    }
    QMessageBox msgBox;
    if (savingSucceeded){
        msgBox.setText("Settings saved");
    }
    else {
        msgBox.setText("Something went wrong. Try again");
    }
    msgBox.exec();

}

void MainWindow::fetchYearlyBreakdown()
{
        QDateEdit *dateEdit = nullptr;
        for (auto &control : historicalControls){
            if (control->objectName() == "yearlyBreakDownDate"){
                dateEdit = static_cast<QDateEdit*>(control);
            }
        }
        if (dateEdit != nullptr){
            for (auto &chartView : chartViews){
                chartView.second->hide();
            }
            chartViews.at("left")->setChart(createLineChart("REALTIME"));
            chartViews.at("left")->show();

            int year = dateEdit->date().year();
            QDateTime startingDate;
            startingDate.setDate(QDate(year, 1, 1));
            QDateTime endingDate;
            endingDate.setDate(QDate(year, 12, 31));

            realTimeController->getTimeSeriesData(startingDate, endingDate, {"Värriö"}, "CO2", intervalLength, "ARITHMETIC");
            QLineSeries *series = realTimeChartModel->getStationValues("Värriö", "CO2");
            std::pair<int, int> minMax = getMinMax<int>(*series);

            charts.at("REALTIME")->removeAllSeries();
            charts.at("REALTIME")->addSeries(realTimeChartModel->getStationValues("Värriö", "CO2"));

            createAxes(series, minMax.first, minMax.second, startingDate, endingDate, "REALTIME", "CO2 (ppm)", 12, "MM.yyyy");
        }
    }

// Set loaded settings with updateSettings()
void MainWindow::on_actionLoadSettings_triggered()
{
    SettingsDialog *dialog = nullptr;

    if (currentChartType == REALTIME) {
        dialog = new SettingsDialog(this,0);
    }
    if(currentChartType == HISTORICAL) {
         dialog = new SettingsDialog(this,1);
    }
    if(currentChartType == COMPARE){
        dialog = new SettingsDialog(this,2);
    }
    dialog->show();
    connect(dialog, &SettingsDialog::settingsFinished,this, &MainWindow::settingsSelected );
}


void MainWindow::on_actionsave_chart_as_image_triggered()
{
    std::string chartView;
    if(currentChartType == REALTIME){
        chartView = "left";
    }
    if(currentChartType == HISTORICAL){
        chartView = "right";
    }
    if(currentChartType == COMPARE){
        chartView = "right";
    }
    SaveImageDialog* dialog = new SaveImageDialog(this,chartViews.at(chartView));
    dialog->show();
    connect(dialog, &SaveImageDialog::saveResult,this, &MainWindow::imageSaveResult);

}


void MainWindow::settingsSelected(SMEARSettings smear, STATFISettings statfi)
{
    std::vector<QString> stations;
    foreach(QString str,smear.stations){
        stations.push_back(str);
    }
    std::vector<QString> statfiTitles;
    foreach(QString str,statfi.dataset){
        QString value;
        for (auto it = statfiOptions.begin(); it != statfiOptions.end(); ++it)
            if (it->second == str)
                 value = it->first;
        statfiTitles.push_back(value);
    }
    updateSettings(stations,smear.gas,"NONE",smear.start,smear.end,statfi.start,statfi.end,statfiTitles);
}

void MainWindow::imageSaveResult(QString message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}


