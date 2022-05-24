#ifndef DIALOGSAVE_H
#define DIALOGSAVE_H

#include <QDialog>

namespace Ui {
class DialogSave;
}

class DialogSave : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSave(QWidget *parent = nullptr);
    ~DialogSave();

private:
    Ui::DialogSave *ui;
};

#endif // DIALOGSAVE_H
