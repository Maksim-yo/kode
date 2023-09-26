#ifndef SORTERFILTER_H
#define SORTERFILTER_H
#include <vector>
#include <string>
#include <unordered_map>
#include <list>
#include "ui/ObjectItem.hpp"

namespace UI {

    class SorterFilter {

    public:
        SorterFilter() = default;
        ~SorterFilter() = default;
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

        QList<Group> groupByName(QList<ObjectItem*>& data);
        QList<Group> groupByDistance(QList<ObjectItem*>&  data);
        QList<Group> groupByType(QList<ObjectItem*>&  data, int n);
        QList<Group> groupByTime(QList<ObjectItem*>&  data);
        QList<Group> sort(QList<ObjectItem*>&  data, SortType type, int n = 0);

    private:
        std::u16string convertTimeCreation(TimeCreation time);
        TimeCreation convertTimeCreation(std::u16string);
        std::u16string convertDistance(DistanceSort time);
        DistanceSort convertDistance(std::u16string );
        TimeCreation getTimeCreation(int day_cur, int month_cur, int year_cur, int day_week_cur, int day,int month, int year, int day_week);

    };
}
#endif // SORTERFILTER_H
