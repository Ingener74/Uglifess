//
// Created by pavel on 11.09.17.
//

#include "SimulatorSerialPortParser.h"

const int SIMULATED_DELAY = 10;

SimulatorSerialPortParser::SimulatorSerialPortParser()
{}

bool SimulatorSerialPortParser::read()
{
	QMutexLocker locker(&mutex);
	cond.wait(&mutex, SIMULATED_DELAY);
	voltages.clear();
	voltages.push_back({0.010, (qrand() % 1000) * 0.001 - 0.5});
	return true;
}

void SimulatorSerialPortParser::close()
{
	cond.wakeOne();
}

QVector<VoltagePoint> SimulatorSerialPortParser::getVoltages() const {
	return voltages;
}
