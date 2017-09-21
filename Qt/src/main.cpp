
#include <QtWidgets/QApplication>
#include "MainWidget.h"

MainWidget *mainWidget = nullptr;

void qtMessageHandler(QtMsgType, const QMessageLogContext &, const QString &message) {
    if (!mainWidget)
        return;

    mainWidget->addLogMessage(message);
}

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    try {
        qInstallMessageHandler(qtMessageHandler);

        qRegisterMetaType<DoubleVector>("DoubleVector");

        MainWidget mainWidget;
        ::mainWidget = &mainWidget;
        mainWidget.show();

        return application.exec();
    } catch(std::exception const& e) {
        QMessageBox::critical(nullptr, "Error", QString("Caught exception {}").arg(e.what()));
    }
}