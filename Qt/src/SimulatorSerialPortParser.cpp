//
// Created by pavel on 11.09.17.
//

#include "SimulatorSerialPortParser.h"

SimulatorSerialPortParser::SimulatorSerialPortParser()
{}

void SimulatorSerialPortParser::setData(const QByteArray& data)
{
	millis += 0.010;
	voltage += (qrand() % 1000) * 0.001 - 0.5;
}

double SimulatorSerialPortParser::getTime() const
{
	return millis;
}

double SimulatorSerialPortParser::getVoltage() const
{
	return voltage;
}
