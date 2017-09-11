//
// Created by pavel on 11.09.17.
//

#ifndef UGLIFESS_SERIALPORTTHREAD_H
#define UGLIFESS_SERIALPORTTHREAD_H

#include <QtCore/QThread>
#include <QtCore/QWaitCondition>
#include <QtCore/QMutex>
#include <memory>

using DoubleVector = QVector<double>;

class SerialPortParser;

class SerialPortThread: public QThread
{
	Q_OBJECT
public:
	SerialPortThread(QObject* parent = nullptr, bool simulate = false);

	virtual ~SerialPortThread();

signals:
	void onDataReady(DoubleVector, DoubleVector);

private:
	void run() override;

	std::unique_ptr<SerialPortParser> serialPortParser;

	QAtomicInt work;
	QMutex mutex;
	QWaitCondition cond;
};


#endif //UGLIFESS_SERIALPORTTHREAD_H
