#include <iostream>
#include <QtWidgets/QApplication>
#include "MainWidget.h"

int main(int argc, char* argv[])
{
	QApplication application(argc, argv);

	qRegisterMetaType<DoubleVector>("DoubleVector");

	MainWidget mainWidget;
	mainWidget.show();

	return application.exec();
}