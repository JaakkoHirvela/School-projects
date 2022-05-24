#include "saveimagedialog.h"
#include "ui_saveimagedialog.h"

SaveImageDialog::SaveImageDialog(QWidget *parent, QChartView* view) :
    QDialog(parent),
    ui(new Ui::SaveImageDialog)
{
    ui->setupUi(this);
    view_= view;
}

SaveImageDialog::~SaveImageDialog()
{
    delete ui;
}

bool SaveImageDialog::saveImage(QString fileName)
{
    QPixmap p = view_->grab();
    if(p.save(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/"+fileName + ".png", "PNG")) {
            return true;
    }
    this->close();
    return false;

}

void SaveImageDialog::on_buttonBox_accepted()
{
    QString fileName = ui->lineEdit->text();
    bool succeeded = saveImage(fileName);
    QString message;
    if(succeeded) {
        message = "Image saved to " + QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    }
    else {
        message = "Something went wrong";
    }
    emit saveResult(message);
}

