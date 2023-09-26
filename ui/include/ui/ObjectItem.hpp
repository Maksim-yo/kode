#ifndef OBJECTITEM_HPP
#define OBJECTITEM_HPP

#include <QObject>
#include <QAbstractListModel>
namespace UI {


    enum SortType {

        Distance,
        Name,
        Time,
        Type,
        None,
    };

    struct Group {

        QString name{};
        int start{0};
        int end{0};

        friend bool operator==(const Group& a, const Group& b){

            return a.name == b.name && a.start == b.start && a.end == b.end;
        }
    };

    class ObjectItem;
    class SorterFilter;

    using function_sorter = std::function<QList<Group>(QList<ObjectItem*>& data)>;

    class ObjectItem : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ name CONSTANT)
        Q_PROPERTY(QString type READ type CONSTANT)
        Q_PROPERTY(double time_creation READ timeCreation CONSTANT)
        Q_PROPERTY(double x READ x CONSTANT)
        Q_PROPERTY(double y READ y CONSTANT)

    public:
        ObjectItem(QString name, double x, double y, QString type, double time_creation, QObject *parent = nullptr);
        QString name() const {return _name;}
        QString type() const {return _type;}
        double timeCreation() const {return _time_creation;}
        double x() const {return _x;}
        double y(){return _y;}

        void setName(QString name){_name = name;}
        void setType(QString type){_type = type;}
        void setTimeCreation(double time){_time_creation = time;}
        void setX(double x){_x = x;}
        void setY(double y){_y = y;}

    private:
        QString _name;
        double _x;
        double _y;
        QString _type;
        double _time_creation;

    };


    class ObjectItemList : public  QAbstractListModel
    {


        Q_OBJECT
        Q_ENUMS(MusicRoles)


    public:


        enum MusicRoles {
            GroupNameRole = Qt::UserRole + 1,
            GroupDataRole,
        };

        ObjectItemList();
        ~ObjectItemList();
        int rowCount(const QModelIndex & parent = QModelIndex()) const override;
        QVariant data(const QModelIndex & index, int role) const override;
        bool setData(const QModelIndex &index, const QVariant &value, int role) override;
        void addItem(ObjectItem* item);
        void updateGroups(QList<Group> data);
        void setGroups(QList<Group> groups);
        void setList(QList<ObjectItem*> data);
        QHash<int, QByteArray> roleNames() const override;
        void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
        void sort(function_sorter func, int column = 0);

    signals:
        // USED IN QML as onModelUpdate()
        void modelUpdate();

    private:
        QList<Group> groups;
        QList<ObjectItem*> objects;
        function_sorter sorter;

    };


}
#endif // OBJECTITEM_HPP
