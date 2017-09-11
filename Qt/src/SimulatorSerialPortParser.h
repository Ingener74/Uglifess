//
// Created by pavel on 11.09.17.
//

#ifndef UGLIFESS_SIMULATORSERIALPORTPARSER_H
#define UGLIFESS_SIMULATORSERIALPORTPARSER_H


#include "SerialPortParser.h"

class SimulatorSerialPortParser: public SerialPortParser
{
public:
	SimulatorSerialPortParser();

	virtual ~SimulatorSerialPortParser() = default;

	void setData(const QByteArray& data) override;

	double getTime() const override;

	double getVoltage() const override;

private:
	double millis = 0.0;
	double voltage = 2.4;
};


#endif //UGLIFESS_SIMULATORSERIALPORTPARSER_H
