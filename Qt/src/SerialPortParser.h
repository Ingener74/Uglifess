//
// Created by pavel on 11.09.17.
//

#ifndef UGLIFESS_SERIALPORTPARSER_H
#define UGLIFESS_SERIALPORTPARSER_H


#include <QtCore/QByteArray>

struct VoltagePoint {
	double timestamp;
	double voltage;
};

class SerialPortParser
{
public:
	virtual ~SerialPortParser() = default;

	virtual void setUpdateTimeMs(int updateTime) = 0;

	virtual bool read() = 0;

	virtual QVector<VoltagePoint> getVoltages() const = 0;

	virtual void close() = 0;
};


#endif //UGLIFESS_SERIALPORTPARSER_H
