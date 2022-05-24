#include "mainwindow.h"
#include "uicomponents/realtimechartcontroller.h"
#include "uicomponents/historicalchartcontroller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow(nullptr);
    w->show();
    return a.exec();
}
