//
// Created by pavel on 11.09.17.
//

#include <iostream>
#include <QtCore/QDebug>
#include "ProtocolV001SerialPortParser.h"

ProtocolV001SerialPortParser::ProtocolV001SerialPortParser(const QString &portName) {
    qDebug() << portName;
    port.setPortName(portName);
    if (port.open(QIODevice::OpenModeFlag::ReadOnly)) {
        port.setBaudRate(QSerialPort::Baud115200);
        port.setDataBits(QSerialPort::Data8);
        port.setParity(QSerialPort::NoParity);
        port.setStopBits(QSerialPort::OneStop);
        port.setFlowControl(QSerialPort::NoFlowControl);
        port.setDataTerminalReady(true);
    }
}

bool ProtocolV001SerialPortParser::read() {
    if (!port.isOpen())
        return false;

    voltagesPoints.clear();

    if (port.waitForReadyRead()) {
        auto data = port.readAll();

        temp += data;

        bool ok = false;

        auto dataString = QString(data);
        auto lines = dataString.split("\n");
        for (auto &line: lines) {
            auto parts = line.split(" ");
            if (parts.size() != 2) {
                continue;
            }
            voltagesPoints.push_back({parts.at(0).toDouble(&ok) / 1000.0, 3.3 * (parts.at(1).toDouble(&ok) / 4096.0)});
        }

        port.clear();

        return true;
    }
    return false;
}

void ProtocolV001SerialPortParser::close() {
    port.close();
}

QVector<VoltagePoint> ProtocolV001SerialPortParser::getVoltages() const {
    return voltagesPoints;
}
