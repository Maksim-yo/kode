#ifndef UIHANDLER_H
#define UIHANDLER_H

#include <memory>
#include <QObject>
class FileHandler;

namespace UI {


    class ObjectItem;
    class ObjectItemList;
    class SorterFilter;

    enum objectType{

        Human,
        Car,
        Bulding,
        Tree,
        Dots,

    };


    std::string_view convertObjectType(objectType type);

    objectType convertOjbectType(std::string_view str);


    struct object {

        std::u16string name;
        objectType type;
        double x;
        double y;
        double time;

        friend std::ostream& operator<<(std::ostream& os, const object & t);

    };

    class UIHandler: public QObject {

      Q_OBJECT
    public :
        UIHandler(ObjectItemList* model, std::unique_ptr<SorterFilter> filter);
        ~UIHandler();
    public slots:
        void onAddSaved(QVariantMap data);
        void onReadFile(QString path);
        void onSortMethodChanged(QString, int n);
    signals:
        void addSaved(QVariantMap data);
        void sortMethodChanged(QString, int n = -1);
        void readFile(QString path);
    private:
        object readObject(std::u16string str);
        QList<ObjectItem*> converter(std::list<object> data);
        object converter(ObjectItem* obj);
        QList<ObjectItem*> convertToList(QMap<QString, QList<ObjectItem*>> data);
        void updateModel(int type, int n = 0);
        std::unique_ptr<FileHandler> file;
        std::unique_ptr<SorterFilter> filter;
        ObjectItemList* model;
    };
}
#endif // UIHANDLER_H
