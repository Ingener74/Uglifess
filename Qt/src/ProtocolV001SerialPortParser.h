//
// Created by pavel on 11.09.17.
//

#ifndef UGLIFESS_PROTOCOLV001SERIALPORTPARSER_H
#define UGLIFESS_PROTOCOLV001SERIALPORTPARSER_H


#include "SerialPortParser.h"

class ProtocolV001SerialPortParser: public SerialPortParser
{
public:
	ProtocolV001SerialPortParser();

private:
	void setData(const QByteArray& data) override;

	double getTime() const override;

	double getVoltage() const override;
};


#endif //UGLIFESS_PROTOCOLV001SERIALPORTPARSER_H
