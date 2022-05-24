#ifndef SAVEIMAGEDIALOG_H
#define SAVEIMAGEDIALOG_H

#include <QDialog>
#include <QtCharts>

namespace Ui {
class SaveImageDialog;
}

class SaveImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveImageDialog(QWidget *parent = nullptr, QChartView* view = nullptr);
    ~SaveImageDialog();

signals:
    void saveResult(QString message);

private slots:
    void on_buttonBox_accepted();

private:
    bool saveImage(QString fileName);
    Ui::SaveImageDialog *ui;
    QChartView* view_;
};

#endif // SAVEIMAGEDIALOG_H
