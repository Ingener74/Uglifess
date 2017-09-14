//
// Created by pavel on 11.09.17.
//

#pragma once

#include <QtCore/QThread>
#include <QtCore/QWaitCondition>
#include <QtCore/QMutex>
#include <memory>

using DoubleVector = QVector<double>;

class SerialPortParser;

class SerialPortThread : public QThread {
Q_OBJECT
public:
    SerialPortThread(QObject *parent, const QString &portName, bool simulate);

    ~SerialPortThread() override;

public slots:
	void setUpdateTimeMs(int updateTime);

Q_SIGNALS:

    void onDataReady(DoubleVector, DoubleVector);

    void onFail(QString);

private:
    void run() override;

    std::unique_ptr<SerialPortParser> serialPortParser;
    QAtomicInt work;
};
