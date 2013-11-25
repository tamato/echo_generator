#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"

#include <QQmlContext>
#include "screenshot.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ScreenShot>("Capture", 1, 0, "ScreenCapture");

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/echo_generator/echo_main.qml"));
    viewer.showExpanded();
    return app.exec();
}
