#ifndef OBJECTITEM_HPP
#define OBJECTITEM_HPP

#include <QObject>
#include <QAbstractListModel>

namespace UI {


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

        using QAbstractListModel::QAbstractListModel;

        enum MusicRoles {
            GroupNameRole = Qt::UserRole + 1,
            GroupDataRole,
        };

        ~ObjectItemList(){}
        int rowCount(const QModelIndex & parent = QModelIndex()) const override;
        QVariant data(const QModelIndex & index, int role) const override;
        void addItem(QString group_name, ObjectItem* item);
        bool setData(const QModelIndex &index, const QVariant &value, int role) override;
        void clear();
        void update(QVector<ObjectItem> data);
        void modifyData(std::function<void(ObjectItem& item)> function);
        void setList(QMap<QString, QList<ObjectItem*>> items);
        QMap<QString, QList<ObjectItem*>>  getList() const;
        QHash<int, QByteArray> roleNames() const override;
    signals:
        void nameClicked(int index);
        void nameChanged();
        void durationChanged();
        void artistChanged();
        void listenersChanged();
        void imageChanged();
        void modelChanged(QVector<UI::ObjectItem*> temp);
        // USED IN QML as onModelUpdate()
        void modelUpdate();

    private:
        QMap<QString, QList<ObjectItem*>> objectList;

    };
}
#endif // OBJECTITEM_HPP
