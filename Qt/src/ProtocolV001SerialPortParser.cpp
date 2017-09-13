//
// Created by pavel on 11.09.17.
//

#include <iostream>
#include <QtCore/QDebug>
#include "ProtocolV001SerialPortParser.h"

const int READ_WAIT_TIMEOUT = 500;

const double MILLISECONDS_IN_SEDOND = 1000.0;

const double VREF_VOLTAGE = 3.3;

const double MAX_ANALOG_READ_VALUE = 4096.0;

ProtocolV001SerialPortParser::ProtocolV001SerialPortParser(const QString &portName, Version version): version(version) {
	if (version == Version::RegexSimple)
		regularExpression = QRegularExpression("(?<T>\\d+) (?<U>\\d+)\r?\n?");

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

    if (port.waitForReadyRead(READ_WAIT_TIMEOUT)) {

        auto data = port.readAll();

        if (data.isEmpty())
            return false;

		switch (version) {
			case Version::Simple: parseV1(data); break;
			case Version::RegexSimple: parseV2(data); break;
			case Version::RegexComplex: parseV3(data); break;
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

void ProtocolV001SerialPortParser::parseV1(const QByteArray& data)
{
    qDebug() << ">" << data << "<";

    temp += data;

    int timeIdx = 0;
    int timeEndIdx = 0;
    int voltageIdx = 0;
    int voltageEndIdx = 0;

    while(true) {
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
                                  temp.mid(voltageIdx, voltageEndIdx - voltageIdx).toDouble(&ok) * VREF_VOLTAGE / MAX_ANALOG_READ_VALUE});

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

void ProtocolV001SerialPortParser::parseV2(const QByteArray& data)
{
	int end = -1;
	temp += data;
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

void ProtocolV001SerialPortParser::parseV3(const QByteArray& data)
{
	temp += data;
    QRegularExpression re("T(\\d+) U(\\d+)\r?\n?");
    auto match = re.match(temp);
	if (match.hasMatch()) {
		qDebug() << match;
		1;
	} else {
	}
}
