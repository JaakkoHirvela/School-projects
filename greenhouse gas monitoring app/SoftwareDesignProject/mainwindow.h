#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "uicomponents/realtimechartmodel.h"
#include "uicomponents/realtimechartcontroller.h"
#include "uicomponents/historicalchartcontroller.h"

#include "uicomponents/settingsdialog.h"
#include "uicomponents/saveimagedialog.h"

#include <QMainWindow>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QPushButton>
#include <QButtonGroup>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <algorithm>
#include <QMessageBox>
#include <QStringList>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// enum aggregationType {NONE, ARITHMETIC, MIN, MAX};
enum chartType {REALTIME, HISTORICAL, COMPARE};
const std::map<QString, QString>  statfiOptions{{"Khk_yht","gas emissions"},{"Khk_yht_index", "gas emissions indexed"},
                        {"Khk_yht_las","intensity of gas emissions"},{"Khk_yht_las_index","intensity of gases indexed"}};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateSettings(const std::vector<QString> stations, const QString gas,
                        const QString agg, QDateTime realTimeStart, QDateTime realTimeEnd,
                        QDateTime historicalStart, QDateTime historicalEnd,
                        std::vector<QString> historicalTitles = {});

private slots:
    void showRealTimeChart();
    void showHistoricalChart();
    void showCompareCharts();
    void applyAndRefresh();

    void updateRealTimeRange();
    void updateHistoricalTimeRange();
    void updateStations();
    void updateGasType();
    void updateEarliestDates();

    void setAggregationToAverage();
    void setAggregationToMinimum();
    void setAggregationToMaximum();

    void saveCurrentSettings();
    void fetchYearlyBreakdown();

    void on_actionLoadSettings_triggered();

    void on_actionsave_chart_as_image_triggered();
    void settingsSelected(SMEARSettings smear, STATFISettings statfi);
    void imageSaveResult(QString message);

private:
    void createControls();
    QChart* createLineChart(std::string chartKey);

    template <typename T>
    void createAxes(QLineSeries *series, T minY, T maxY, QDateTime startDate, QDateTime endDate,
                    const std::string &chartName, const QString &axisYTitle="", int axisXTicks=10,
                    QString axisXFormat = "dd.MM.yyyy");

    template <typename T>
    std::pair<T, T> getMinMax(const QLineSeries &series, const double &spacing=0.01);
    void updateHistoricalChart();
    void updateRealTimeChart();

    void initializeDate();
    void addConnections();

    void updateAggregationButtons();
    void updateGasTypeButtons();

    const std::vector<QString> getColumnTitles();
    void drawAndShowChart(std::string chartName, std::string chartViewName,
                             QLineSeries *timeScaledGasValues, QString legendName);



private:
    Ui::MainWindow *ui;

    RealTimeChartModel *realTimeChartModel;
    std::map<std::string, QChartView*> chartViews;
    std::map<std::string, QChart*> charts;

    // std::unordered_map<std::string, IGraphicController*> controllerMap;
    RealTimeChartController *realTimeController;
    HistoricalChartController *historicalController;

    std::vector<QWidget*> realTimeControls;
    std::vector<QWidget*> historicalControls;
    QButtonGroup *historicalCheckBoxes;

    QDateTime *realTimeStartDate;
    QDateTime *realTimeEndDate;
    QDateTime *historicalStartDate;
    QDateTime *historicalEndDate;

    QString aggregation = "NONE"; // "NONE", "ARITHMETIC", "MIN", "MAX"
    chartType currentChartType = REALTIME; // REALTIME, HISTORICAL, COMPARE

    std::vector<QString> stations;
    QString gasType = "CO2";
    QString intervalLength = "60";
    /* Can be set to 30 or 60 (minutes)
     * 30-minute interval has some problems in the source.
     * SO2 works with 30min intervals but CO2 doesn't.
    */

};
#endif // MAINWINDOW_H
