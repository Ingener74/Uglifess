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
	millis += 0.010;
	voltage += (qrand() % 1000) * 0.001 - 0.5;
	return true;
}

double SimulatorSerialPortParser::getTime() const
{
	return millis;
}

double SimulatorSerialPortParser::getVoltage() const
{
	return voltage;
}

void SimulatorSerialPortParser::close()
{
	cond.wakeOne();
}
