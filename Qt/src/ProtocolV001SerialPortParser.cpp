//
// Created by pavel on 11.09.17.
//

#include <iostream>
#include <QtCore/QDebug>
#include "ProtocolV001SerialPortParser.h"

ProtocolV001SerialPortParser::ProtocolV001SerialPortParser(const QString& portName) {
	qDebug() << portName;
	port.setPortName(portName);
	if (port.open(QIODevice::OpenModeFlag::ReadOnly)) {
		port.setBaudRate(QSerialPort::Baud115200);
		port.setDataBits(QSerialPort::Data8);
		port.setParity(QSerialPort::NoParity);
		port.setStopBits(QSerialPort::OneStop);
		port.setFlowControl(QSerialPort::NoFlowControl);
	}
}

bool ProtocolV001SerialPortParser::read() {
	if (!port.isOpen())
		return false;

	if (port.waitForReadyRead()) {
		auto data = port.readAll();
		qDebug() << data;

		bool ok = false;
//		time = data.toLong(&ok) / 1000.0;
//		voltage = 3.3 * (data.toInt(&ok) / 4096.0);

		auto dataString = QString(data);
		auto parts = dataString.split(" ");
		if (parts.size() != 2) {
			return false;
		}

		time = parts.at(0).toDouble(&ok) / 1000.0;
		voltage = 3.3 * (parts.at(1).toDouble(&ok) / 4096.0);

		return true;
	}
	return false;
}

double ProtocolV001SerialPortParser::getTime() const {
	return 0;
}

double ProtocolV001SerialPortParser::getVoltage() const {
	return 0;
}

void ProtocolV001SerialPortParser::close()
{
	port.close();
}
