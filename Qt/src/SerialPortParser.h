//
// Created by pavel on 11.09.17.
//

#ifndef UGLIFESS_SERIALPORTPARSER_H
#define UGLIFESS_SERIALPORTPARSER_H


#include <QtCore/QByteArray>

class SerialPortParser
{
public:
	virtual ~SerialPortParser() = default;
	virtual bool read() = 0;
	virtual double getTime() const = 0;
	virtual double getVoltage() const = 0;
};


#endif //UGLIFESS_SERIALPORTPARSER_H
