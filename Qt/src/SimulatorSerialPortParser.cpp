//
// Created by pavel on 11.09.17.
//

#include "SimulatorSerialPortParser.h"

SimulatorSerialPortParser::SimulatorSerialPortParser()
{}

bool SimulatorSerialPortParser::read()
{
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
