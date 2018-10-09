#include <QApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>

#include "settings.h"
#include "appstate.h"
#include "pagestate.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Vadim Balakhanov");
    QCoreApplication::setOrganizationDomain("baldis.org");
    QCoreApplication::setApplicationName("DreamENG WordLearn");

    QApplication app(argc, argv);

    QTranslator translator;
    QString fn = QString(":/lang_%1.qm").arg(QLocale::system().name());
    qDebug() << fn << translator.load(fn);
    app.installTranslator(&translator);

    QQmlApplicationEngine engine;
    qmlRegisterType<Settings>("Settings", 1, 0, "Settings");
    qmlRegisterType<AppState>("AppState", 1, 0, "AppState");
    qmlRegisterType<PageState>("PageState", 1, 0, "PageState");
    qmlRegisterType<WordState>("WordState", 1, 0, "WordState");
    qmlRegisterType<HeadState>("HeadState", 1, 0, "HeadState");
    qmlRegisterType<StatState>("StatState", 1, 0, "StatState");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
