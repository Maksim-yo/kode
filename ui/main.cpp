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

    void add(UI::ObjectItemList*& model){

        UI::ObjectItem* item = new UI::ObjectItem("Hello",13,14,"d", 412412);
        UI::ObjectItem* item2 = new UI::ObjectItem("fsdafads",13,14,"111",42342314);
        model->addItem("Today", item);
        model->addItem("Today", item);
        model->addItem("Gello", item2);
    }


int main(int argc, char *argv[])
{

    std::filesystem::path path;
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QQmlContext* context = engine.rootContext();
    UI::ObjectItemList* model = new UI::ObjectItemList();
    std::unique_ptr<UI::SorterFilter> filter = std::make_unique<UI::SorterFilter>();
    add(model);
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
    emit handler->readFile("C:/Users/lyzlo/Documents/testing/ex.txt");
    return app.exec();

    return 0;
}
