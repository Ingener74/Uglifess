//
// Created by pavel on 11.09.17.
//

#include "SimulatorSerialPortParser.h"

const int SIMULATED_DELAY = 33;

SimulatorSerialPortParser::SimulatorSerialPortParser()
{}

bool SimulatorSerialPortParser::read()
{
	QMutexLocker locker(&mutex);
	cond.wait(&mutex, SIMULATED_DELAY);
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
