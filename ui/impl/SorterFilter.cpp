#include <list>

#include "ui/SorterFilter.hpp"
#include "utils/utils.hpp"
#include <math.h>

namespace UI {


std::map<std::u16string, std::list<object>> SorterFilter::groupByName(std::list<object> data){

        std::map<std::u16string, std::list<object>> groups;
        for(auto&& elm : data){

            char16_t key = static_cast<char16_t>(elm.name[0]);
            if (!Utils::isCyrillica(key))
                key = u'#';

            std::u16string char_str;
            char_str.push_back(key);
            if (!groups.contains(char_str))
                groups[char_str] = {};

            std::list<object>& _data = groups[char_str];
            _data.emplace_front(elm);
            // O(n)
            _data.sort([](const object& a, const object& b){ return a.name < b.name;});
        }
        return groups;

    }

    std::map<std::u16string, std::list<object> > SorterFilter::groupByDistance(std::list<object> data)
    {
        std::array<int,3> dist{100,1000,10000};
        std::map<std::u16string, std::list<object>> groups;

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

        for(auto&& elm : data){

            DistanceSort sort_type = DistanceSort::ToFarAway;
            for (int i = 0; i < dist.size(); i++){

                if (!isCorrrectForDistance(elm.x, dist[i]) && !isCorrrectForDistance(elm.y, dist[i]))
                    continue;
                sort_type = static_cast<DistanceSort>(i);
            }

            std::u16string key = convertDistance(sort_type);
            if (!groups.contains(key))
                groups[key] = {};

            std::list<object>& _data = groups[key];
            _data.emplace_front(elm);
            // O(n)
            _data.sort([](const object& a, const object& b){ return a.x > b.x;});

        }
        return groups;
    }

    std::map<std::u16string, std::list<object> > SorterFilter::groupByType(std::list<object> data, int n)
    {
        std::map<std::u16string, std::list<object>> groups;
        std::array<int, 5> object_count{0,0,0,0,0};
        for(auto&& elm : data){

            std::u16string key;
            object_count[elm.type]++;
            Utils::fromUTF8(std::string(convertObjectType(elm.type)), key);
            if (!groups.contains(key))
                groups[key] = {};

            std::list<object>& data = groups[key];
            data.emplace_front(elm);
            // O(n)
            data.sort([](const object& a, const object& b){ return a.name < b.name;});

        }
        int object_type_size = 5;
        std::u16string key = u"Разное";
        for (int i = 0; i < object_type_size; i++){

            if (object_count[i] > n)
                continue;
            if (!groups.contains(key))
                groups[key] = {};
            std::u16string cur_key;
            Utils::fromUTF8(std::string(convertObjectType(static_cast<objectType>(i))), cur_key);
            std::list<object>& _data_elm = groups[cur_key];
            std::list<object>& _data = groups[key];
            std::transform(_data_elm.begin(), _data_elm.end(), std::back_inserter(_data), [](object obj){return obj;} );
            groups.erase(cur_key);
        }
        if (groups.contains(key)) {
            std::list<object>& _data =groups[key];
            _data.sort([](const object& a, const object& b){ return a.name < b.name;});
        }
        return groups;

    }

    std::map<std::u16string, std::list<object>> SorterFilter::groupByTime(std::list<object> data){

        auto cur_time = Utils::getCurrentTime();
        std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(cur_time)};
        auto[year_cur, month_cur, day_cur] = Utils::get_year_month_day(ymd);
        auto weekday_cur = Utils::getWeekDay(cur_time);
        std::map<std::u16string, std::list<object>> groups;

        for(auto&& elm : data){

            auto elm_time = Utils::convertMilisecsToSecs(elm.time);
            std::chrono::year_month_day ymd_elm{std::chrono::floor<std::chrono::days>(elm_time)};
            auto[year, month, day] = Utils::get_year_month_day(ymd_elm);
            auto weekday = Utils::getWeekDay(elm_time);
            TimeCreation time_type = getTimeCreation(day_cur,month_cur, year_cur, weekday_cur.c_encoding(), day, month, year, weekday.c_encoding());
            std::u16string key = convertTimeCreation(time_type);
            if (!groups.contains(key))
                groups[key] = {};

            std::list<object>& data = groups[key];
            data.emplace_front(elm);
            // O(n)
            data.sort([](const object& a, const object& b){ return a.time > b.time;});
        }
        return groups;
    }

    std::map<std::u16string, std::list<object> > SorterFilter::sort(std::list<object> data, SortType type, int n)
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
            std::map<std::u16string, std::list<object> > res;
            res.emplace(u"", data);
            return res;
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
