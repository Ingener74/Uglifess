//
// Created by pavel on 11.09.17.
//

#include <QtCore/QDateTime>
#include <QtCore/QVector>
#include <QtCore/QDebug>

#include "SerialPortThread.h"
#include "SimulatorSerialPortParser.h"
#include "RealSerialPortParser.h"

const int NUMBER_OF_MEASUREMENT = 200;

SerialPortThread::SerialPortThread(QObject* parent, const QString& portName, bool simulate, int delay) : QThread(parent) {
    if (simulate) {
        serialPortParser = std::make_unique<SimulatorSerialPortParser>(delay);
    } else {
        serialPortParser = std::make_unique<RealSerialPortParser>(portName,
                                                                          RealSerialPortParser::Version::RegexComplex,
                                                                          delay);
    }
    work = 1;
}

void SerialPortThread::setUpdateTimeMs(int updateTime) {
	serialPortParser->setUpdateTimeMs(updateTime);
}

void SerialPortThread::run() {
    try {
        QVector<double> time;
        QVector<double> voltages;

        while (work) {
            if (serialPortParser->read()) {
                auto newVoltages = serialPortParser->getVoltages();

                for (auto &voltage: newVoltages) {
                    time.push_back(voltage.timestamp);
                    voltages.push_back(voltage.voltage);
                }

                while (time.size() > NUMBER_OF_MEASUREMENT)
                    time.pop_front();
                while (voltages.size() > NUMBER_OF_MEASUREMENT)
                    voltages.pop_front();

                emit onDataReady(time, voltages);
            }
        }
    } catch (std::exception const &e) {
        qWarning() << e.what();
        emit onFail(e.what());
    }
}

SerialPortThread::~SerialPortThread() {
    work = 0;
    serialPortParser->close();
    if (isRunning())
        wait();
}
