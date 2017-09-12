//
// Created by pavel on 11.09.17.
//

#pragma once


#include <QtSerialPort/QSerialPort>
#include "SerialPortParser.h"

class ProtocolV001SerialPortParser: public SerialPortParser
{
public:
	ProtocolV001SerialPortParser(const QString& portName);

private:
	bool read() override;

	double getTime() const override;

	double getVoltage() const override;

	QSerialPort port;
	double time = 0, voltage = 0;
};
