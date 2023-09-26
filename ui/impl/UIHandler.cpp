#include <iostream>
#include <QVector>
#include <QVariantMap>
#include <QUrl>

#include "ui/UIHandler.hpp"
#include "ui/ObjectItem.hpp"
#include "ui/FileHandler.hpp"
#include "ui/SorterFilter.hpp"

#include "utils/error.hpp"
#include "utils/utils.hpp"

namespace {

    std::string view_converter(std::u16string_view str_view){

        std::u16string str = {str_view.begin(), str_view.end()};
        return Utils::toUTF8(str);
    }
}

namespace UI {

    UIHandler::~UIHandler() = default;
    UIHandler::UIHandler(ObjectItemList* model, std::unique_ptr<SorterFilter> filter) : model(model), filter(std::move(filter)) {

        connect(this, &UIHandler::addSaved,  this, &UIHandler::onAddSaved);
        connect(this, &UIHandler::readFile, this, &UIHandler::onReadFile);
        connect(this, &UIHandler::sortMethodChanged, this, &UIHandler::onSortMethodChanged);
    }
    void UIHandler::onAddSaved(QVariantMap data){

        double x = data.value("x").toDouble();
        double y = data.value("y").toDouble();
        QString type = data.value("type").toString();
        QString name = data.value("name").toString();
        double time = data.value("time_creation").toDouble();
        UI::ObjectItem* itm = new UI::ObjectItem(name, x, y, type, time);

        model->addItem(itm);
        object obj = converter(itm);
        std::stringstream str;
        str << obj;
        file->write(std::move(str));

    }

    void UIHandler::onReadFile(QString path)
    {

        QString resolvePath =  QUrl(path).isLocalFile() == true ? QUrl(path).toLocalFile() : path;
        file = createFileHanlder(resolvePath.toStdString());
        std::stringstream data = file->read();
        int line = 1;
        std::list<object> objects;
        std::string str;
        qDebug() << "Reading file " << resolvePath;

        while (std::getline(data,str)){

            try {
                std::u16string _str;
                Utils::fromUTF8(str, _str);
                objects.emplace_back(readObject(_str));
            }

            catch (const BaseError& err) {

                qDebug() << "Couldn't read object at line: " << line << "." << err.what();
            }
            line++;
        }
        QList<ObjectItem*> _list{converter(objects)};
        model->setList(_list);
    }

    void UIHandler::onSortMethodChanged(QString name, int n)
    {
        if (name == "По имени")
            updateModel(SortType::Name, n);
        else if (name == "По расстоянию")
            updateModel(SortType::Distance, n);
        else if (name == "По времени создания")
            updateModel(SortType::Time, n);
        else if (name == "По типу")
            updateModel(SortType::Type, n);
        else
            updateModel(SortType::None, n);
    }

    object UIHandler::readObject(std::u16string str)
    {
        object _obj;
        auto str_view = std::u16string_view(str);
        std::vector<std::u16string_view> data = Utils::split(str_view, std::u16string_view(u" "));
        if (data.size() != 5)
            throw IncorrectObjectRepresantion("Incorrect object format: " + Utils::toUTF8(str));

        try {

            _obj.name = {data[0].begin(), data[0].end()};
            _obj.x = std::stod(view_converter((data[1])));
            _obj.y = std::stod(view_converter(data[2]));
            _obj.type = convertOjbectType(view_converter(data[3]));
            _obj.time = std::stod(view_converter(data[4]));
        }
        catch(const std::runtime_error& err ) {
            throw IncorrectObjectRepresantion("Incorrect object format: " + Utils::toUTF8(str));
        }
        return _obj;

    }


    QList<ObjectItem *> UIHandler::converter(std::list<object> data)
    {
        QList<ObjectItem *> items;
        for (const auto& obj: data) {

            items.push_back(new ObjectItem(Utils::toUTF8(obj.name).c_str(), obj.x, obj.y, convertObjectType(obj.type).data(), obj.time));
        }

        return items;
    }

    object UIHandler::converter(ObjectItem *obj)
    {
        return {obj->name().toStdU16String(), convertOjbectType(obj->type().toStdString()),obj->x(),obj->y(),obj->timeCreation()};
    }

    void UIHandler::updateModel(int type, int n)
    {
        auto func = std::bind(&SorterFilter::sort, filter.get(),  std::placeholders::_1, static_cast<SortType>(type), n);
        model->sort(func);
    }
    std::ostream& operator<<(std::ostream& os, const object& t){

        os << Utils::toUTF8(t.name);
        os << " ";
        os << t.x;
        os << " ";
        os << t.y;
        os << " ";
        os << convertObjectType(t.type);
        os << " ";
        os << std::to_string(t.time);
        return os;
    }

    objectType convertOjbectType(std::string_view str){

        if (str == "Человек")
            return objectType::Human;
        else if(str == "Здание")
            return objectType::Bulding;
        else if (str == "Машина")
            return objectType::Car;
        else if (str == "Дерево")
            return objectType::Tree;
        else if (str == "...")
            return objectType::Dots;
        else
            throw UnsupportedObjectType();
    }

    std::string_view convertObjectType(objectType type){

        switch (type){

        case objectType::Human:
            return "Человек";
        case objectType::Car:
            return "Машина";
        case objectType::Bulding:
            return "Здание";
        case objectType::Tree:
            return  "Дерево";
        case objectType::Dots:
            return  "...";

        }
    }

}
