#include "dialogsave.h"
#include "ui_dialogsave.h"

DialogSave::DialogSave(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSave)
{
    ui->setupUi(this);
}

DialogSave::~DialogSave()
{
    delete ui;
}
