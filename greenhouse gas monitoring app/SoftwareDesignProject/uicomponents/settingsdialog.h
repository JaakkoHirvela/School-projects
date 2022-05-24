#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "database/database.h"
#include <QTableView>
#include <QSqlTableModel>
#include <QTableWidget>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr, int type=0);
    ~SettingsDialog();

signals:
    void settingsFinished(struct SMEARSettings,struct STATFISettings);


private slots:
    void accept();

private:
    void getSmearValues();
    void getStatfiValues();
    void getCompareValues();
    void setSmearValues(QStringList,QDateTime startDate, QDateTime endDate, QString gas);
    void setStatfiValues(QDateTime startDate, QDateTime endDate, QStringList datasets);
    QDateTime changeDateFormat(QString date);
    Ui::SettingsDialog *ui;
    QTableView *view;
    QSqlTableModel *model;
    int type_;
    SMEARSettings smear_;
    STATFISettings statfi_;
};

#endif // SETTINGSDIALOG_H
