#include "ui/ObjectItem.hpp"

namespace UI {

    ObjectItem::ObjectItem(QString name, double x, double y, QString type, double time_creation, QObject *parent)
        : _name(name),  _x(x), _y(y), _type(type), _time_creation(time_creation), QObject(parent)
    {

    }

    QHash<int, QByteArray> ObjectItemList::roleNames() const {
        QHash<int, QByteArray> roles;
        roles[GroupNameRole] = "group_name";
        roles[GroupDataRole] = "group_data";
        return roles;
    }

    int ObjectItemList::rowCount(const QModelIndex & parent) const {
        if (parent.isValid())
            return 0;
        return objectList.count();
    }

    QVariant ObjectItemList::data(const QModelIndex &index, int role) const
    {
        if (index.row() < 0 || index.row() >= objectList.count())
            return QVariant();
        auto start =  objectList.begin();
        int c = 0;
        for(; start != objectList.end(); start++, c++) {
            if (c == index.row())
                break;
        }
        QString group_name = start.key();
        QList<UI::ObjectItem*> items = start.value();
        if (role == GroupNameRole)
            return group_name;
        if (role == GroupDataRole) {
            QVariantList res;
            std::transform(items.begin(), items.end(), std::back_inserter(res), [](ObjectItem* itm){return QVariant::fromValue(itm);});
            return QVariant::fromValue(res);
        }

    }

    void ObjectItemList::addItem(QString group_name, ObjectItem* item)
    {
        beginInsertRows(QModelIndex(), objectList.size(), objectList.size());
        bool isExist = objectList.contains(group_name);
        if (!isExist)
            objectList.insert(group_name, {item});
        else
            objectList[group_name].append(item);
        endInsertRows();
    }

    void ObjectItemList::setList(QMap<QString, QList<ObjectItem*>> items)
    {
        beginResetModel();
        objectList = items;
        endResetModel();

    }

    QMap<QString, QList<ObjectItem*>> ObjectItemList::getList() const
    {
        return objectList;
    }

    bool ObjectItemList::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (!hasIndex(index.row(), index.column(), index.parent()) || !value.isValid())
            return false;

    }

}
