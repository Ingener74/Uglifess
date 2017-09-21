//
// Created by pavel on 11.09.17.
//

#include <iostream>
#include <QtCore/QDebug>
#include "RealSerialPortParser.h"

const int READ_WAIT_TIMEOUT = 500;

const double MILLISECONDS_IN_SEDOND = 1000.0;

const double VREF_VOLTAGE = 3.3;

const double MAX_ANALOG_READ_VALUE = 4096.0;

RealSerialPortParser::RealSerialPortParser(const QString& portName, Version version, int /*delay*/)
        : version(version) {
    if (version == Version::RegexSimple)
        regularExpression = QRegularExpression("(?<T>\\d+) (?<U>\\d+)\r?\n?");
    else if (version == Version::RegexComplex)
        regularExpression = QRegularExpression("(?<T>T\\d+) (?<U>U\\d+)\r\n");

    qDebug() << portName;
    port.setPortName(portName);
    if (port.open(QIODevice::OpenModeFlag::ReadWrite)) {
        port.setBaudRate(QSerialPort::Baud9600);
        port.setDataBits(QSerialPort::Data8);
        port.setParity(QSerialPort::NoParity);
        port.setStopBits(QSerialPort::OneStop);
        port.setFlowControl(QSerialPort::NoFlowControl);
        port.setDataTerminalReady(true);
    }
}

void RealSerialPortParser::setUpdateTimeMs(int updateTime) {
    if (!port.isOpen())
        return;
    port.write(QString("delay %1").arg(updateTime).toLocal8Bit());
    port.flush();
}

bool RealSerialPortParser::read() {
    if (!port.isOpen())
        return false;

    voltagesPoints.clear();

    port.clear();

    if (port.waitForReadyRead(READ_WAIT_TIMEOUT)) {

        auto data = port.readAll();

        if (data.isEmpty())
            return false;

        switch (version) {
            case Version::Simple:
                parseV1(data);
                break;
            case Version::RegexSimple:
                parseV2(data);
                break;
            case Version::RegexComplex:
                parseV3(data);
                break;
        }

        return true;
    }
    return false;
}

void RealSerialPortParser::close() {
    port.close();
}

QVector<VoltagePoint> RealSerialPortParser::getVoltages() const {
    return voltagesPoints;
}

void RealSerialPortParser::parseV1(const QByteArray &data) {
    qDebug() << ">" << data << "<";

    temp += data;

    int timeIdx = 0;
    int timeEndIdx = 0;
    int voltageIdx = 0;
    int voltageEndIdx = 0;

    while (true) {
        if (timeIdx == temp.size())
            break;

        timeEndIdx = temp.indexOf(" ", timeIdx);

        if (timeEndIdx == -1)
            break;

        voltageIdx = timeEndIdx + 1;
        voltageEndIdx = temp.indexOf("\r", voltageIdx);

        if (voltageEndIdx == -1)
            break;

        bool ok = false;

        auto t = temp.mid(timeIdx, timeEndIdx - timeIdx).toStdString();
        auto v = temp.mid(voltageIdx, voltageEndIdx - voltageIdx).toStdString();

        voltagesPoints.push_back({temp.mid(timeIdx, timeEndIdx - timeIdx).toDouble(&ok) / MILLISECONDS_IN_SEDOND,
                                  temp.mid(voltageIdx, voltageEndIdx - voltageIdx).toDouble(&ok) * VREF_VOLTAGE /
                                  MAX_ANALOG_READ_VALUE});

        timeIdx = voltageEndIdx;
        while (true) {
            if (timeIdx == temp.size())
                break;
            char c = temp.at(timeIdx);
            if (timeIdx == temp.size() || (c != '\r' && c != '\n'))
                break;
            timeIdx++;
        }
    }

    const int len = temp.size() - timeIdx;
    if (len == 0)
        temp.clear();
    else
        temp = temp.right(len);
}

void RealSerialPortParser::parseV2(const QByteArray &data) {
    int end = -1;
    temp += data;
    qDebug() << "#" << temp << "#";
    auto matchIt = regularExpression.globalMatch(temp);
    while (matchIt.hasNext()) {
        auto match = matchIt.next();
        qDebug() << match;
        if (match.hasMatch()) {
            double T = match.captured("T").toDouble();
            double U = match.captured("U").toDouble();
            voltagesPoints.push_back({T, U});
            end = match.capturedEnd(2);
        }
    }
    if (end != -1)
        temp = temp.right(temp.size() - end);
}

void RealSerialPortParser::parseV3(const QByteArray &data) {
    qDebug() << "#" << data << "#";
    auto matchIt = regularExpression.globalMatch(data);
    while (matchIt.hasNext()) {
        auto match = matchIt.next();
        if (match.hasMatch()) {
            const QString &TStr = match.captured("T");
            double T = TStr.right(TStr.size() - 1).toDouble() / MILLISECONDS_IN_SEDOND;
            const QString &UStr = match.captured("U");
            double U = UStr.right(UStr.size() - 1).toDouble() * VREF_VOLTAGE / MAX_ANALOG_READ_VALUE;
            voltagesPoints.push_back({T, U});
        }
    }
}
