//
// Created by pavel on 11.09.17.
//

#pragma once

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

	void onSimulateCheckBoxChanged(int);

	void onSerialFail(QString);

protected:
    void showEvent(QShowEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

	void timerEvent(QTimerEvent* event) override;

	void checkPorts();

    void initPlot() const;

    QCustomPlot *customPlot = nullptr;
    std::unique_ptr<SerialPortThread> serialPortThread;

	int checkPortTimerId = ~0;
};
