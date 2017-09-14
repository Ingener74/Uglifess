//
// Created by pavel on 11.09.17.
//

#include "SimulatorSerialPortParser.h"

SimulatorSerialPortParser::SimulatorSerialPortParser()
{}

void SimulatorSerialPortParser::setUpdateTimeMs(int updateTime)
{
	delay = updateTime;
}

bool SimulatorSerialPortParser::read()
{
	QMutexLocker locker(&mutex);
	cond.wait(&mutex, delay);
	voltages.clear();
	voltages.push_back({timestamp += 0.033, 2.25 + (qrand() % 100) * 0.001 - 0.5});
	return true;
}

void SimulatorSerialPortParser::close()
{
	cond.wakeOne();
}

QVector<VoltagePoint> SimulatorSerialPortParser::getVoltages() const {
	return voltages;
}
