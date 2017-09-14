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

	~SimulatorSerialPortParser() override = default;

	void setUpdateTimeMs(int updateTime) override;

	bool read() override;

	QVector<VoltagePoint> getVoltages() const override;

	void close() override;

private:
	QVector<VoltagePoint> voltages;

	int delay = 0;
	double timestamp = 0.0;
	QMutex mutex;
	QWaitCondition cond;
};
