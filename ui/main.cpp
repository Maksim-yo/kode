#include <iostream>
#include <filesystem>
#include <list>
#include <unordered_map>
#include <chrono>
#include <cmath>
#include <fstream>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>


#include "utils/utils.hpp"
#include "ui/ObjectItem.hpp"
#include "ui/SorterFilter.hpp"
#include "ui/UIHandler.hpp"


int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QQmlContext* context = engine.rootContext();
    UI::ObjectItemList* model = new UI::ObjectItemList();
    std::unique_ptr<UI::SorterFilter> filter = std::make_unique<UI::SorterFilter>();
    const QUrl url(u"qrc:/kode/qml/main.qml"_qs);

    UI::UIHandler* handler = new UI::UIHandler(model, std::move(filter));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    context->setContextProperty("objectList", model);
    context->setContextProperty("handler", handler);

    engine.load(url);
    return app.exec();

    return 0;
}
