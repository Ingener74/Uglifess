//
// Created by pavel on 11.09.17.
//

#pragma once

#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>

#include "SerialPortParser.h"

class SimulatorSerialPortParser: public SerialPortParser
{
public:
	SimulatorSerialPortParser();

	virtual ~SimulatorSerialPortParser() = default;

	bool read() override;

	double getTime() const override;

	double getVoltage() const override;

	void close() override;

private:
	double millis = 0.0;
	double voltage = 2.4;

	QMutex mutex;
	QWaitCondition cond;
};
