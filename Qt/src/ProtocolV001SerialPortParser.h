//
// Created by pavel on 11.09.17.
//

#pragma once

#include <QtSerialPort/QSerialPort>
#include <QtCore/QRegularExpression>
#include "SerialPortParser.h"

class ProtocolV001SerialPortParser: public SerialPortParser
{
public:
	enum class Version {
		Simple,
		RegexSimple,
		RegexComplex,
	};

	explicit ProtocolV001SerialPortParser(const QString& portName, Version);

	~ProtocolV001SerialPortParser() override = default;

	void setUpdateTimeMs(int updateTime) override;

	bool read() override;

	QVector<VoltagePoint> getVoltages() const override;

	void close() override;

protected:

	void parseV1(const QByteArray& data);
	void parseV2(const QByteArray& data);
	void parseV3(const QByteArray& data);

	Version version;

	QRegularExpression regularExpression;

	QSerialPort port;
	QByteArray temp;
	QVector<VoltagePoint> voltagesPoints;
};
