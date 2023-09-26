#ifndef SORTERFILTER_H
#define SORTERFILTER_H
#include <vector>
#include <string>
#include <unordered_map>
#include <list>
#include "ui/UIHandler.hpp"
#include <QSortFilterProxyModel>

namespace UI {

//    class MySortFilterProxyModel : public QSortFilterProxyModel
//    {
//        Q_OBJECT

//        Q_ENUMS(SortTypes)

//    public:
//        enum SortTypes {
//            Distance = Qt::UserRole + 1,
//            Name,
//            Time,
//            Type,
//            None,
//        };


//        MySortFilterProxyModel(QObject *parent = 0);

//    protected:
//        bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
//    };

    class SorterFilter {

    public:
        SorterFilter() = default;
        enum TimeCreation {

            Today,
            Yesterday,
            InThisWeek,
            InThisMonth,
            InThisYear,
            Before,

        };

        enum DistanceSort {

            To100,
            To1000,
            To10000,
            ToFarAway,

        };

        std::map<std::u16string, std::list<object>> groupByName(std::list<object> data);
        std::map<std::u16string, std::list<object>> groupByDistance(std::list<object> data);
        std::map<std::u16string, std::list<object>> groupByType(std::list<object> data, int n);
        std::map<std::u16string, std::list<object>> groupByTime(std::list<object> data);
        std::map<std::u16string, std::list<object>> sort(std::list<object> data, SortType type, int n = 0);

    private:
        std::u16string convertTimeCreation(TimeCreation time);
        std::u16string convertDistance(DistanceSort time);
        TimeCreation getTimeCreation(int day_cur, int month_cur, int year_cur, int day_week_cur, int day,int month, int year, int day_week);

    };
}
#endif // SORTERFILTER_H
