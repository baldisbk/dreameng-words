#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "settings.h"
#include "appstate.h"
#include "pagestate.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);  
    QCoreApplication::setOrganizationName("Vadim Balakhanov");
    QCoreApplication::setOrganizationDomain("baldis.org");
    QCoreApplication::setApplicationName("DreamENG WordLearn");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<Settings>("Settings", 1, 0, "Settings");
    qmlRegisterType<AppState>("AppState", 1, 0, "AppState");
    qmlRegisterType<PageState>("PageState", 1, 0, "PageState");
    qmlRegisterType<WordState>("WordState", 1, 0, "WordState");
    qmlRegisterType<StatState>("StatState", 1, 0, "StatState");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
