#include "ui/ObjectItem.hpp"
#include "ui/SorterFilter.hpp"
#include <QElapsedTimer>

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

    void ObjectItemList::sort(int column, Qt::SortOrder order)
    {
        emit layoutAboutToBeChanged();
        QList<Group> _groups = sorter(objects);
        updateGroups(_groups);
        emit layoutChanged();
    }

    void ObjectItemList::sort(function_sorter func, int column)
    {
        sorter = func;
        sort(column);
    }

    ObjectItemList::~ObjectItemList() = default;

    ObjectItemList::ObjectItemList()
    {
        // for optimization
        QList<Group> groups;
        for(int i = 0;i < 6; i++)
            groups.emplace_back(Group());
        setGroups(groups);
    }

    int ObjectItemList::rowCount(const QModelIndex & parent) const {
        if (parent.isValid())
            return 0;
        return groups.count();
    }

    QVariant ObjectItemList::data(const QModelIndex &index, int role) const
    {
        if (index.row() < 0 || index.row() >= groups.count())
            return QVariant();

        Group group = groups.at(index.row());

        if (role == GroupNameRole)
            return group.name;
        if (role == GroupDataRole) {
            QVariantList res;
            QList<ObjectItem*> sub_list = objects.mid(group.start, group.end - group.start);
            return QVariant::fromValue(sub_list);
        }
    }

    void ObjectItemList::addItem(ObjectItem* item)
    {
        emit layoutAboutToBeChanged();
        beginInsertRows(QModelIndex(),0,0);
        objects.push_back(item);
        endInsertRows();

        QList<Group> data = groups;

        auto comparison = [](const Group& a, const Group& b)
            {
                return a.end < b.end;
            };

        QList<Group>::iterator max = std::max_element(data.begin(), data.end(), comparison);
        max->end++;
        updateGroups(data);
        emit layoutChanged();
    }

    void ObjectItemList::updateGroups(QList<Group> data)
    {
        QElapsedTimer timer;
        timer.start();
        for(int i =0;i < groups.size(); i++){
            if (i < data.size() && data[i] == groups[i])
                continue;
            else if (i < data.size())
                groups[i] = data[i];
            else
                groups[i] = Group();

            dataChanged(index(i), index(i));

     }
    }

    void ObjectItemList::setGroups(QList<Group> _groups)
    {
        beginResetModel();
        groups = _groups;
        endResetModel();
    }

    void ObjectItemList::setList(QList<ObjectItem *> data)
    {
        QList<Group> group;
        bool isSorter = sorter ? true : false;
        group.emplace_back("", 0, data.size());
        beginResetModel();            
        objects = data;
        endResetModel();
        if (isSorter)
            sort(0);
        else
            updateGroups(group);
    }

    bool ObjectItemList::setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (!hasIndex(index.row(), index.column(), index.parent()) || !value.isValid())
            return false;
    }
}
