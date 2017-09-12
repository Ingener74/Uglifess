//
// Created by pavel on 11.09.17.
//

#ifndef QT_MAINWIDGET_H
#define QT_MAINWIDGET_H

#include <memory>

#include <QtWidgets/QWidget>
#include <ui_mainwidget.h>

#include "qcustomplot.h"

#include "SerialPortThread.h"

class MainWidget : public QWidget, public Ui::MainWidget {
Q_OBJECT
public:
    MainWidget(QWidget *parent = nullptr, const Qt::WindowFlags &f = 0);

public slots:

    void onConnectButtonClick();

    void onDataReady(DoubleVector, DoubleVector);

protected:
    void showEvent(QShowEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void checkPorts() const;

    void initPlot() const;

    QCustomPlot *customPlot;
    std::unique_ptr<SerialPortThread> serialPortThread;
};


#endif //QT_MAINWIDGET_H
