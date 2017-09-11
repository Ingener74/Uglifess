//
// Created by pavel on 11.09.17.
//

#include <QtCore/QDateTime>
#include <QtCore/QVector>
#include "SerialPortThread.h"
#include "SimulatorSerialPortParser.h"
#include "ProtocolV001SerialPortParser.h"

SerialPortThread::SerialPortThread(QObject* parent, bool simulate) : QThread(parent)
{
	if (simulate) {
		serialPortParser = std::make_unique<SimulatorSerialPortParser>();
	} else {
		serialPortParser = std::make_unique<ProtocolV001SerialPortParser>();
	}
	work = 1;
}

void SerialPortThread::run() {
	try {
		qsrand(static_cast<uint>(QDateTime::currentSecsSinceEpoch()));

		QVector<double> time;
		QVector<double> voltages;

		while(work) {
			QMutexLocker locker(&mutex);
			cond.wait(&mutex, 100);

			serialPortParser->setData({});

			time.append(serialPortParser->getTime());
			voltages.append(serialPortParser->getVoltage());

			if (time.size() > 200)
				time.pop_front();
			if (voltages.size() > 200)
				voltages.pop_front();

			emit onDataReady(time, voltages);
		}
	} catch (std::exception const& e) {
	}
}

SerialPortThread::~SerialPortThread()
{
	work = 0;
	cond.wakeOne();
	if (isRunning())
		wait();
}
