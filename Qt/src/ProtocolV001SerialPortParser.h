//
// Created by pavel on 11.09.17.
//

#pragma once


#include <QtSerialPort/QSerialPort>
#include "SerialPortParser.h"

class ProtocolV001SerialPortParser: public SerialPortParser
{
public:
	explicit ProtocolV001SerialPortParser(const QString& portName);

	~ProtocolV001SerialPortParser() override = default;

	bool read() override;

	double getTime() const override;

	double getVoltage() const override;

	void close() override;

protected:
	QSerialPort port;
	double time = 0, voltage = 0;
};
