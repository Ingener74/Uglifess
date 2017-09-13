//
// Created by pavel on 11.09.17.
//

#include <QtCore/QDateTime>
#include <QtCore/QVector>
#include <QtCore/QDebug>

#include "SerialPortThread.h"
#include "SimulatorSerialPortParser.h"
#include "ProtocolV001SerialPortParser.h"

const int NUMBER_OF_MEASUREMENT = 200;

SerialPortThread::SerialPortThread(QObject *parent, const QString &portName, bool simulate) : QThread(parent) {
    if (simulate) {
        serialPortParser = std::make_unique<SimulatorSerialPortParser>();
    } else {
        serialPortParser = std::make_unique<ProtocolV001SerialPortParser>(portName,
                                                                          ProtocolV001SerialPortParser::Version::RegexSimple);
    }
    work = 1;
}

void SerialPortThread::run() {
    try {
        qsrand(static_cast<uint>(QDateTime::currentSecsSinceEpoch()));

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
        qDebug() << e.what();
        emit onFail(e.what());
    }
}

SerialPortThread::~SerialPortThread() {
    work = 0;
    serialPortParser->close();
    if (isRunning())
        wait();
}
