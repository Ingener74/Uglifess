//
// Created by pavel on 11.09.17.
//

#include <QtCore/QSettings>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtGui/QKeyEvent>
#include <iostream>

#include "MainWidget.h"

/*
 * If qDebug not working answer here https://stackoverflow.com/questions/34355549/qdebug-doesnt-print-anything
 */

const char *const COMPANY = "ShnaiderPavel";

const char *const PROGRAM = "Uglifess";

const char *const GEOMETRY = "geometry";

const char *const SPLITTER_1 = "splitter1";

const char *const Y_MIN = "YMin";

const char *const Y_MAX = "YMax";

const int NUMBER_OF_POINTS = 100;

const char* const DELAY = "DELAY";

MainWidget::MainWidget(QWidget *parent, const Qt::WindowFlags &f) : QWidget(parent, f), Ui::MainWidget() {
    setupUi(this);

    auto plotLayout = new QVBoxLayout();
    customPlot = new QCustomPlot(this);
    plotLayout->addWidget(customPlot);
    groupBoxPlot->setLayout(plotLayout);

    connect(pushButtonConnect, SIGNAL(clicked()), SLOT(onConnectButtonClick()));
    connect(pushButtonUpdate, SIGNAL(clicked()), SLOT(onUpdateClick()));
    connect(checkBoxSimulate, SIGNAL(stateChanged(int)), SLOT(onSimulateCheckBoxChanged(int)));
    connect(checkBoxAutoRanges, SIGNAL(stateChanged(int)), SLOT(onAutoRangesChanged(int)));
    connect(doubleSpinBoxUMin, SIGNAL(valueChanged(double)), SLOT(onRangesChanged(double)));
    connect(doubleSpinBoxUMax, SIGNAL(valueChanged(double)), SLOT(onRangesChanged(double)));
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
                                                              checkBoxSimulate->isChecked(),
                                                              spinBoxUpdateDelay->value());

        connect(serialPortThread.get(),
                SIGNAL(onDataReady(DoubleVector, DoubleVector)),
                SLOT(onDataReady(DoubleVector, DoubleVector)));

        connect(serialPortThread.get(), SIGNAL(onFail(QString)), SLOT(onSerialFail(QString)));

        serialPortThread->start();

        pushButtonConnect->setText("Disconnect");
    }
}

void MainWidget::onUpdateClick() {
    serialPortThread->setUpdateTimeMs(spinBoxUpdateDelay->value());
}

void MainWidget::onDataReady(QVector<double> time, QVector<double> voltage) {
    if (time.empty() || voltage.empty() || time.size() != voltage.size())
        return;

    x = time;
    y = voltage;

    replot();
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

void MainWidget::onAutoRangesChanged(int state) {
    doubleSpinBoxUMax->setEnabled(state == Qt::Unchecked);
    doubleSpinBoxUMin->setEnabled(state == Qt::Unchecked);
    replot();
}

void MainWidget::onSerialFail(QString text) {
    QMessageBox::critical(this, "Error", text);
    onConnectButtonClick();
}

void MainWidget::onRangesChanged(double) {
    replot();
}

void MainWidget::addLogMessage(const QString &text) {
    textEditLog->append(text);
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
    if (settings.contains(Y_MIN))
        doubleSpinBoxUMin->setValue(settings.value(Y_MIN).toDouble());
    if (settings.contains(Y_MAX))
        doubleSpinBoxUMax->setValue(settings.value(Y_MAX).toDouble());
	if (settings.contains(DELAY))
		spinBoxUpdateDelay->setValue(settings.value(DELAY).toInt());
}

void MainWidget::closeEvent(QCloseEvent *event) {
    QWidget::closeEvent(event);

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, COMPANY, PROGRAM);
    settings.setValue(GEOMETRY, saveGeometry());
    settings.setValue(SPLITTER_1, splitter->saveState());
    settings.setValue(Y_MIN, doubleSpinBoxUMin->value());
    settings.setValue(Y_MAX, doubleSpinBoxUMax->value());
	settings.setValue(DELAY, spinBoxUpdateDelay->value());
}

void MainWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape)
        close();
    QWidget::keyPressEvent(event);
}

void MainWidget::timerEvent(QTimerEvent *event) {
    QObject::timerEvent(event);

    checkPorts();
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
    else if (checkPortTimerId != ~0) {
        killTimer(checkPortTimerId);
        checkPortTimerId = ~0;
    }
}

void MainWidget::initPlot() {
    double xmin = -4.0, xmax = 4.0;

    x.resize(NUMBER_OF_POINTS);
    y.resize(NUMBER_OF_POINTS);

    for (size_t i = 0; i < NUMBER_OF_POINTS; ++i)
        y[i] = sin(x[i] = xmin + (xmax - xmin) * (i / 100.0));

    customPlot->addGraph();

    replot();
}

void MainWidget::replot() {
    customPlot->graph(0)->setData(x, y);
    customPlot->xAxis->setRange(x.front(), x.back());

    if (checkBoxAutoRanges->checkState() == Qt::Checked) {
        const double ymin = *std::min_element(y.begin(), y.end());
        const double ymax = *std::max_element(y.begin(), y.end());
        doubleSpinBoxUMin->setValue(ymin);
        doubleSpinBoxUMax->setValue(ymax);
        customPlot->yAxis->setRange(ymin, ymax);

    } else {
        customPlot->yAxis->setRange(doubleSpinBoxUMin->value(), doubleSpinBoxUMax->value());
    }

    customPlot->xAxis->setLabel("Time, msec");
    customPlot->yAxis->setLabel("Voltage, V");
    customPlot->replot();
}
