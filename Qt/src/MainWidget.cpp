//
// Created by pavel on 11.09.17.
//

#include <QtCore/QSettings>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtGui/QKeyEvent>
#include <iostream>

#include "MainWidget.h"

const char *const GEOMETRY = "geometry";

const char *const SPLITTER_1 = "splitter1";

MainWidget::MainWidget(QWidget *parent, const Qt::WindowFlags &f) : Ui::MainWidget() {
    setupUi(this);

    auto plotLayout = new QVBoxLayout();
    customPlot = new QCustomPlot(this);
    plotLayout->addWidget(customPlot);
    groupBoxPlot->setLayout(plotLayout);

    connect(pushButtonConnect, SIGNAL(clicked()), SLOT(onConnectButtonClick()));
}

void MainWidget::onConnectButtonClick() {
    serialPortThread = std::make_unique<SerialPortThread>(nullptr, checkBoxSimulate->isChecked());

    connect(serialPortThread.get(), SIGNAL(onDataReady(DoubleVector, DoubleVector)),
            SLOT(onDataReady(DoubleVector, DoubleVector)));

    serialPortThread->start();
}

void MainWidget::onDataReady(QVector<double> time, QVector<double> voltage) {
    double ymin = std::numeric_limits<double>::max();
    double ymax = std::numeric_limits<double>::min();
    for (auto &y: voltage) {
        ymin = std::min(ymin, y);
        ymax = std::max(ymax, y);
    }
    customPlot->xAxis->setRange(time.front(), time.back());
    customPlot->yAxis->setRange(ymin, ymax);
    customPlot->graph(0)->setData(time, voltage);
    customPlot->replot();
}

void MainWidget::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    checkPorts();

    initPlot();

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "ShnaiderPavel", "Uglifess");
    if (settings.contains(GEOMETRY))
        restoreGeometry(settings.value(GEOMETRY).toByteArray());
    if (settings.contains(SPLITTER_1))
        splitter->restoreState(settings.value(SPLITTER_1).toByteArray());
}

void MainWidget::closeEvent(QCloseEvent *event) {
    QWidget::closeEvent(event);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "ShnaiderPavel", "Uglifess");
    settings.setValue(GEOMETRY, saveGeometry());
    settings.setValue(SPLITTER_1, splitter->saveState());
}

void MainWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape)
        close();
    QWidget::keyPressEvent(event);
}

void MainWidget::checkPorts() const {
    auto availablePorts = QSerialPortInfo::availablePorts();
    comboBoxSerialPort->clear();
    for (auto &availablePort: availablePorts)
        comboBoxSerialPort->addItem(availablePort.portName());

    const bool availablePortsEmpty = availablePorts.empty();
    pushButtonConnect->setEnabled(!availablePortsEmpty);
    comboBoxSerialPort->setEnabled(!availablePortsEmpty);
}

void MainWidget::initPlot() const {
    QVector<double> x(100);
    QVector<double> y(100);

    double xmin = -4.0, xmax = 4.0;

    for (size_t i = 0; i < 100; ++i) {
        x[i] = xmin + (xmax - xmin) * (i / 100.0);
        y[i] = sin(x[i]);
    }

    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    customPlot->xAxis->setRange(xmin, xmax);
    customPlot->yAxis->setRange(-2, 2);
}
