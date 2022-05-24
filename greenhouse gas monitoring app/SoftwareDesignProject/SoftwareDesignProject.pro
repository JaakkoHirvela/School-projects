QT       += core gui \
            network \
            charts \
            sql \
            printsupport \


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    apimodels/historicalmodel.cpp \
    apimodels/statfimodel.cpp \
    apimodels/stationmodel.cpp \
    apimodels/timeseriesrow.cpp \
    apimodels/timeseriestable.cpp \
    database/database.cpp \
    main.cpp \
    mainwindow.cpp \
    services/smearservice.cpp \
    services/statfiservice.cpp \
    repositories/smearrepository.cpp \
    repositories/statfirepository.cpp \
    uicomponents/historicalchartcontroller.cpp \
    uicomponents/historicalchartmodel.cpp \
    uicomponents/realtimechartcontroller.cpp \
    uicomponents/realtimechartmodel.cpp \
    uicomponents/saveimagedialog.cpp \
    uicomponents/settingsdialog.cpp


HEADERS += \
    apimodels/historicalmodel.h \
    apimodels/statfimodel.h \
    apimodels/stationmodel.h \
    apimodels/timeseriesrow.h \
    apimodels/timeseriestable.h \
    chartcontroller.h \
    chartmodel.h \
    database/database.h \
    mainwindow.h \
    repositories/smearrepository.h \
    repositories/statfirepository.h \
    repositories/irepository.h \
    services/smearservice.h \
    services/statfiservice.h \
    services/iservice.h \
    uicomponents/historicalchartcontroller.h \
    uicomponents/historicalchartmodel.h \
    uicomponents/igraphiccontroller.h \
    uicomponents/realtimechartcontroller.h \
    uicomponents/realtimechartmodel.h   \
    uicomponents/saveimagedialog.h \
    uicomponents/settingsdialog.h


FORMS += \
    mainwindow.ui \
    uicomponents/saveimagedialog.ui \
    uicomponents/settingsdialog.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=



