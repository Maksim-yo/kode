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
        emit saved(itm);
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

                std::cout << "Couldn't read object at line: " << line << "." << err.what();
            }
            line++;
        }
        QMap<QString, QList<ObjectItem*>> _map;
        _map.insert("", converter(objects));
        currentSort = SortType::None;
        model->setList(_map);
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
//        std::string_view a = "gf sgdfg ";
//        std::string_view b = " ";
//        std::vector<std::string_view> ad = Utils::split(a, b);

        auto str_view = std::u16string_view(str);
        std::vector<std::u16string_view> data = Utils::split(str_view, std::u16string_view(u" "));
        if (data.size() != 5)
            throw IncorrectObjectRepresantion("Incorrect object format: " + Utils::toUTF8(str));

        try {

            _obj.name = data[0];
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

    QMap<QString, QList<ObjectItem *> > UIHandler::converter(std::map<std::u16string, std::list<object> > data)
    {
        QMap<QString, QList<ObjectItem *> > converted_data;
        for(const auto& elm : data){

            QList<ObjectItem*> items;
            for (const auto& obj: elm.second) {

                items.push_back(new ObjectItem(Utils::toUTF8(obj.name).c_str(), obj.x, obj.y, convertObjectType(obj.type).data(), obj.time));
            }
            QString group_name = Utils::toUTF8(elm.first).c_str();
            bool isExist = converted_data.contains(group_name);
            if (!isExist)
                converted_data.insert(group_name, items);
            else
                converted_data[group_name].append(items);
        }

        return converted_data;

    }

    std::list<object> UIHandler::converter(QList<ObjectItem *> data)
    {
        std::list<object> res;
        for(const auto& obj : data){
            object _obj;
            Utils::fromUTF8(obj->name().toStdString(), _obj.name);
            _obj.x = obj->x();
            _obj.y = obj->y();
            _obj.type = convertOjbectType(obj->type().toStdString());
            _obj.time  = obj->timeCreation();
            res.emplace_back(_obj);
        }
        return res;
    }

    QList<ObjectItem *> UIHandler::converter(std::list<object> data)
    {
        QList<ObjectItem *> items;
        for (const auto& obj: data) {

            items.push_back(new ObjectItem(Utils::toUTF8(obj.name).c_str(), obj.x, obj.y, convertObjectType(obj.type).data(), obj.time));
        }

        return items;
    }

    QList<ObjectItem *> UIHandler::convertToList(QMap<QString, QList<ObjectItem *> > data)
    {
        QList<ObjectItem *> res;
        for(const auto& key : data.keys()){
            std::list<object> items;

            for (const auto& obj: data[key]) {

                res.push_back(obj);
            }
        }
        return res;
    }

    std::map<std::u16string, std::list<object>> converter(QMap<QString, QList<ObjectItem *> >  data){

        std::map<std::u16string, std::list<object>> converted_data;
        for(const auto& key : data.keys()){
            std::list<object> items;

            for (const auto& obj: data[key]) {
                object _obj;
                Utils::fromUTF8(obj->name().toStdString(), _obj.name);
                _obj.x = obj->x();
                _obj.y = obj->y();
                _obj.type = convertOjbectType(obj->type().toStdString());
                _obj.time  = obj->timeCreation();
                items.emplace_back(_obj);
            }

            std::u16string group_name;
            Utils::fromUTF8(key.toStdString(), group_name);
            bool isExist = converted_data.contains(group_name);
            if (!isExist)
                converted_data[group_name] =  items;

        }
        return converted_data;
    }
    void UIHandler::updateModel(SortType type, int n)
    {
        QList<ObjectItem*> data = convertToList(model->getList());
        std::map<std::u16string, std::list<object>> sorted_data = filter->sort(converter(data), type,n);
        auto converted_data = converter(sorted_data);
        model->setList(converted_data);
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
