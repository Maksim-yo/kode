#include <list>
#include <math.h>
#include <functional>
#include <QList>

#include "ui/SorterFilter.hpp"
#include "ui/UIHandler.hpp"
#include "utils/utils.hpp"
#include "utils/error.hpp"
namespace {

    bool contains(const QList<UI::Group>& groups, QString name){

        auto it = std::find_if(groups.begin(), groups.end(),
                               [&name](UI::Group group) -> bool { return group.name == name;});
        return it != groups.end();
    }


}

namespace UI {

QList<Group> SorterFilter::groupByName(QList<ObjectItem*>& data){

        std::sort(data.begin(), data.end(),
              [](const ObjectItem* a, const ObjectItem* b) -> bool { return a->name() < b->name();});

        QList<Group> groups;
        for(int i = 0; i < data.size();i++){

            char16_t key = static_cast<char16_t>(data[i]->name().toStdU16String()[0]);
            if (!Utils::isCyrillica(key))
                key = u'#';

            std::u16string char_str;
            char_str.push_back(key);
            QString qt_str = QString::fromStdU16String(char_str);

            if (groups.isEmpty())
                groups.emplace_back(qt_str, i, i);

            else if (groups.last().name != qt_str)
                groups.emplace_back(qt_str, i, i);
            Group& _group = groups.last();
            _group.end++;

        }
        return groups;

    }

    QList<Group> SorterFilter::groupByDistance(QList<ObjectItem*>& data)
    {
        std::array<int,3> dist{100,1000,10000};

        std::sort(data.begin(), data.end(),
              [](const ObjectItem* a, const ObjectItem* b) -> bool { return a->x() < b->x();});

        QList<Group> groups(4);

        auto isCorrrectForDistance = [](double num, int dist) -> bool{
            num = std::abs(num);
            double intpart;
            double fractpart = modf(num, &intpart);
            if (static_cast<int>(intpart)  < dist)
                return true;
            if (fractpart == static_cast<double>(0) && static_cast<int>(intpart) == dist)
                return true;
            return false;
        };

        for(int i = 0; i < data.size();i++){

            DistanceSort sort_type = DistanceSort::ToFarAway;
            for (int j = 0; j < dist.size(); j++){

                if (isCorrrectForDistance(data[i]->x(), dist[j]) && isCorrrectForDistance(data[i]->y(), dist[j])) {
                    sort_type = static_cast<DistanceSort>(j);
                    break;
                }

            }

            std::u16string key = convertDistance(sort_type);
            QString qt_str = QString::fromStdU16String(key);

            if (groups[convertDistance(key)] == Group())
                groups[convertDistance(key)] = {qt_str, i,i};
            else
                groups[convertDistance(key)].end++;

        }
        QList<Group> sorted_groups(4);
        for(const auto& grp: groups)
            sorted_groups[convertDistance(grp.name.toStdU16String())] = grp;

        return sorted_groups;
    }

    QList<Group> SorterFilter::groupByType(QList<ObjectItem*>& data, int n)
    {
        std::sort(data.begin(), data.end(),
              [](const ObjectItem* a, const ObjectItem* b) -> bool { return a->type() < b->type();});

        QList<Group> groups;
        std::array<int, 5> object_count{0,0,0,0,0};
        for(int i = 0; i < data.size();i++){

            std::u16string key = data[i]->type().toStdU16String();
            QString qt_str = QString::fromStdU16String(key);
            object_count[convertOjbectType(data[i]->type().toStdString())]++;

            if (groups.isEmpty())
                groups.emplace_back(qt_str, i, i);

            if (groups.last().name != qt_str)
                groups.emplace_back(qt_str, i, i);

            Group& _group = groups.last();
            _group.end++;
        }

        for(const auto& elm: groups){
            int start = elm.start;
            int end = elm.end;
            std::sort(data.data() + start , data.data() + end, [](const ObjectItem* a, const ObjectItem* b) -> bool { return a->name() < b->name();});

        }
        std::sort(groups.begin(), groups.end(),
                  [](const Group a,const Group b) -> bool { return a.end - a.start < b.end - b.start;});

        auto _it = std::find_if(groups.begin(), groups.end(), [&n](const Group& group){return (group.end - group.start) <= n;});

        if (_it == groups.end())
            return groups;

        std::u16string key = u"Разное";
        QString str = QString::fromStdU16String(key);

        QList<Group> temp;
        QList<ObjectItem*> new_data;

        Group another_group;
        another_group.name = str;
        bool isAnother = false;
        for(auto it = groups.begin(); it != groups.end(); it++ ){

            if (it == _it){
                another_group.start = new_data.size();
                another_group.end = new_data.size();
                isAnother = true;
            }

            if (isAnother){
                another_group.end += it->end - it->start;
                new_data.append(data.sliced(it->start, it->end - it->start));

            }
            else {
                int new_start = new_data.size();
                new_data.append(data.sliced(it->start, it->end - it->start));
                it->start = new_start;
                it->end = new_data.size() - new_start;
            }
        }
        data = new_data;
        groups.clear();
        if (_it != groups.begin())
            groups = {groups.begin(), _it - 1};

        groups.append(another_group);
        int start = another_group.start;
        int end = another_group.end;
        std::sort(data.data() + start , data.data() + end, [](const ObjectItem* a, const ObjectItem* b) -> bool { return a->name() < b->name();});

        return groups;

    }

    QList<Group> SorterFilter::groupByTime(QList<ObjectItem*>& data){

        auto cur_time = Utils::getCurrentTime();
        std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(cur_time)};
        auto[year_cur, month_cur, day_cur] = Utils::get_year_month_day(ymd);
        auto weekday_cur = Utils::getWeekDay(cur_time);
        std::sort(data.begin(), data.end(),
              [](const ObjectItem* a, const ObjectItem* b) -> bool { return a->timeCreation() < b->timeCreation();});

        QList<Group> groups;
        for(int i = 0; i < data.size();i++){

            auto elm_time = Utils::convertMilisecsToSecs(data[i]->timeCreation());
            std::chrono::year_month_day ymd_elm{std::chrono::floor<std::chrono::days>(elm_time)};
            auto[year, month, day] = Utils::get_year_month_day(ymd_elm);
            auto weekday = Utils::getWeekDay(elm_time);
            TimeCreation time_type = getTimeCreation(day_cur,month_cur, year_cur, weekday_cur.c_encoding(), day, month, year, weekday.c_encoding());
            std::u16string key = convertTimeCreation(time_type);
            QString qt_str = QString::fromStdU16String(key);

            if (groups.isEmpty())
                groups.emplace_back(qt_str, i, i);

            if (groups.last().name != qt_str)
                groups.emplace_back(qt_str, i, i);

            Group& _group = groups.last();
            _group.end++;
        }

        QList<Group> sorted_groups(6);
        for(const auto& grp: groups)
            sorted_groups[convertTimeCreation(grp.name.toStdU16String())] = grp;

        return sorted_groups;
    }

    QList<Group>  SorterFilter::sort(QList<ObjectItem*>& data, SortType type, int n)
    {

        switch (type){
        case UI::SortType::Distance:
            return groupByDistance(data);
        case UI::SortType::Name:
            return groupByName(data);
        case UI::SortType::Time:
            return groupByTime(data);
        case UI::SortType::Type:
            return groupByType(data, n);
        case UI::SortType::None: {
            QList<Group> groups;
            groups.emplace_back("", 0, data.size());
            return groups;
            }
        }
    }

    std::u16string SorterFilter::convertTimeCreation(TimeCreation time){

        switch(time){
        case SorterFilter::Today:
            return u"Сегодня";
        case SorterFilter::Yesterday:
            return u"Вчера";
        case SorterFilter::InThisWeek:
            return u"На этой недели";
        case SorterFilter::InThisMonth:
            return u"В этом месяце";
        case SorterFilter::InThisYear:
            return u"В этом году";
        case SorterFilter::Before:
            return u"Ранее";
        }
    }

    SorterFilter::TimeCreation SorterFilter::convertTimeCreation(std::u16string time)
    {

        if (time == u"Сегодня")
            return SorterFilter::Today;
        if (time == u"Вчера")
            return SorterFilter::Yesterday;
        if (time == u"На этой недели")
            return SorterFilter::InThisWeek;
        if (time == u"В этом месяце")
            return SorterFilter::InThisMonth;
        if (time == u"В этом году")
            return SorterFilter::InThisYear;
        if (time == u"Ранее")
            return SorterFilter::Before;

        throw BaseError("unsupported enum string");
    }

    std::u16string SorterFilter::convertDistance(DistanceSort time)
    {
        switch(time){

        case UI::SorterFilter::To100:
            return u"До 100 ед.";
        case UI::SorterFilter::To1000:
            return u"До 1000 ед.";
        case UI::SorterFilter::To10000:
            return u"До 10000 ед.";
        case UI::SorterFilter::ToFarAway:
            return u"Слишком далеко.";

        }
    }

    SorterFilter::DistanceSort SorterFilter::convertDistance(std::u16string dist)
    {
            if (dist == u"До 100 ед.")
                return UI::SorterFilter::To100;
            if (dist == u"До 1000 ед.")
                return UI::SorterFilter::To1000;
            if (dist == u"До 10000 ед.")
                return UI::SorterFilter::To10000;
            if (dist == u"Слишком далеко.")
                return UI::SorterFilter::ToFarAway;
            throw BaseError("unsupported enum string");

    }

    SorterFilter::TimeCreation SorterFilter::getTimeCreation(int day_cur, int month_cur, int year_cur, int day_week_cur, int day,int month, int year, int day_week){

        if (day_cur == day)
            return TimeCreation::Today;
        else if (day_cur == day + 1)
            return TimeCreation::Yesterday;
        else if (day_week_cur > day_week)
            return TimeCreation::InThisWeek;
        else if (month_cur == month)
            return TimeCreation::InThisMonth;
        else if (year_cur == year)
            return TimeCreation::InThisYear;
        else
            return TimeCreation::Before;
    }
}
