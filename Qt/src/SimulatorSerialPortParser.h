//
// Created by pavel on 11.09.17.
//

#pragma once

#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtCore/QVector>

#include "SerialPortParser.h"

class SimulatorSerialPortParser: public SerialPortParser
{
public:
	SimulatorSerialPortParser();

	virtual ~SimulatorSerialPortParser() = default;

	bool read() override;

	QVector<VoltagePoint> getVoltages() const override;

	void close() override;

private:
	QVector<VoltagePoint> voltages;

	QMutex mutex;
	QWaitCondition cond;
};
