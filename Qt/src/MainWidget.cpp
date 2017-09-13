//
// Created by pavel on 11.09.17.
//

#include <QtCore/QSettings>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtGui/QKeyEvent>
#include <iostream>

#include "MainWidget.h"

const char *const COMPANY = "ShnaiderPavel";

const char *const PROGRAM = "Uglifess";

const char *const GEOMETRY = "geometry";

const char *const SPLITTER_1 = "splitter1";

MainWidget::MainWidget(QWidget *parent, const Qt::WindowFlags &f) : QWidget(parent, f), Ui::MainWidget() {
    setupUi(this);

    auto plotLayout = new QVBoxLayout();
    customPlot = new QCustomPlot(this);
    plotLayout->addWidget(customPlot);
    groupBoxPlot->setLayout(plotLayout);

    connect(pushButtonConnect, SIGNAL(clicked()), SLOT(onConnectButtonClick()));
    connect(checkBoxSimulate, SIGNAL(stateChanged(int)), SLOT(onSimulateCheckBoxChanged(int)));
}

void MainWidget::onConnectButtonClick() {
    if (serialPortThread) {
        disconnect(this, SLOT(onDataReady(DoubleVector, DoubleVector)));
        disconnect(this, SLOT(onSerialFail(QString)));
        serialPortThread.reset();
        pushButtonConnect->setText("Connect");
    } else {
        serialPortThread = std::make_unique<SerialPortThread>(nullptr,
                                                              comboBoxSerialPort->currentData().toString(),
                                                              checkBoxSimulate->isChecked());

        connect(serialPortThread.get(),
                SIGNAL(onDataReady(DoubleVector, DoubleVector)),
                SLOT(onDataReady(DoubleVector, DoubleVector)));

        connect(serialPortThread.get(), SIGNAL(onFail(QString)), SLOT(onSerialFail(QString)));

        serialPortThread->start();

        pushButtonConnect->setText("Disconnect");
    }
}

void MainWidget::onDataReady(QVector<double> time, QVector<double> voltage) {
    if (time.empty() || voltage.empty() || time.size() != voltage.size())
        return;

    customPlot->xAxis->setRange(time.front(), time.back());
//    customPlot->yAxis->setRange(*std::min_element(voltage.begin(), voltage.end()),
//                                *std::max_element(voltage.begin(), voltage.end()));
    customPlot->yAxis->setRange(0, 3.5);
    customPlot->graph(0)->setData(time, voltage);
    customPlot->replot();
}

void MainWidget::onSimulateCheckBoxChanged(int state) {
    if (state == Qt::Checked) {
        if (checkPortTimerId != ~0)
            killTimer(checkPortTimerId);
        comboBoxSerialPort->setEnabled(false);
        pushButtonConnect->setEnabled(true);
    } else if (state == Qt::Unchecked) {
        checkPorts();
    }
}

void MainWidget::onSerialFail(QString text) {
    QMessageBox::critical(this, "Error", text);
    onConnectButtonClick();
}

void MainWidget::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    checkPorts();

    initPlot();

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, COMPANY, PROGRAM);
    if (settings.contains(GEOMETRY))
        restoreGeometry(settings.value(GEOMETRY).toByteArray());
    if (settings.contains(SPLITTER_1))
        splitter->restoreState(settings.value(SPLITTER_1).toByteArray());
}

void MainWidget::closeEvent(QCloseEvent *event) {
    QWidget::closeEvent(event);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, COMPANY, PROGRAM);
    settings.setValue(GEOMETRY, saveGeometry());
    settings.setValue(SPLITTER_1, splitter->saveState());
}

void MainWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape)
        close();
    QWidget::keyPressEvent(event);
}

void MainWidget::checkPorts() {
    auto availablePorts = QSerialPortInfo::availablePorts();
    comboBoxSerialPort->clear();
    for (auto &availablePort: availablePorts)
        comboBoxSerialPort->addItem(availablePort.portName(), availablePort.systemLocation());

    const bool availablePortsEmpty = availablePorts.empty();
    pushButtonConnect->setEnabled(!availablePortsEmpty);
    comboBoxSerialPort->setEnabled(!availablePortsEmpty);

    if (availablePortsEmpty)
        checkPortTimerId = startTimer(1000);
    else if (checkPortTimerId != ~0)
        killTimer(checkPortTimerId);
}

void MainWidget::initPlot() const {
    QVector<double> x(100);
    QVector<double> y(100);

    double xmin = -4.0, xmax = 4.0;

    for (size_t i = 0; i < 100; ++i)
        y[i] = sin(x[i] = xmin + (xmax - xmin) * (i / 100.0));

    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    customPlot->xAxis->setRange(xmin, xmax);
    customPlot->yAxis->setRange(-2, 2);
    customPlot->xAxis->setLabel("Time, sec");
    customPlot->yAxis->setLabel("Voltage, V");
}

void MainWidget::timerEvent(QTimerEvent *event) {
    QObject::timerEvent(event);

    checkPorts();
}
