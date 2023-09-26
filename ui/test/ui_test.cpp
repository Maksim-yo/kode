
#include <QTest>
#include <QObject>
#include <random>
#include <fstream>

#include "ui/UIHandler.hpp"
#include "utils/utils.hpp"
#include "ui/SorterFilter.hpp"

using namespace UI;
using namespace std::chrono;
std::default_random_engine e;

double get_random_float(double min, double max)
{
    return std::uniform_real_distribution<double>{min, max}(e);

}

int get_random_int(int min, int max){

    return std::uniform_int_distribution<int>{min,max}(e);

}

double generate_random_distance(){

    static std::array<int,4> distance{100, 1000,10000, 100000};
    int dist = distance[get_random_int(0,3)];
    double res1 = get_random_float(-dist, static_cast<double>(-dist/10));
    double res2 = get_random_float(static_cast<double>(dist/10), dist);
    bool res = get_random_int(0,1);
    return res ? res1 : res2;
}

double generate_random_day(){

    SorterFilter::TimeCreation time = static_cast<SorterFilter::TimeCreation>(get_random_int(0,5));
    auto time_cur = Utils::getCurrentTime();
    double secs  = time_cur.time_since_epoch() /  std::chrono::seconds(1);
    int secs_in_day = std::chrono::seconds(1h).count() * 24;
    int cur_day_time = (int)secs % secs_in_day;
    double prev_day = secs - cur_day_time - secs_in_day;
    qDebug() << prev_day;
    std::chrono::year_month_day ymd{std::chrono::floor<std::chrono::days>(time_cur)};
    auto[year_cur, month_cur, day_cur] = Utils::get_year_month_day(ymd);

    switch (time){
    case UI::SorterFilter::Today:
        return secs - get_random_int(0, cur_day_time);
    case UI::SorterFilter::Yesterday:
        return prev_day + get_random_int(0,secs_in_day);
    case UI::SorterFilter::InThisWeek:
        return prev_day - get_random_int(secs_in_day, secs_in_day * 7);
    case UI::SorterFilter::InThisMonth:
        return prev_day - get_random_int(secs_in_day * 7, secs_in_day * 30);
    case UI::SorterFilter::InThisYear:
       return prev_day - get_random_int(secs_in_day * 30, secs_in_day * 365);
    case UI::SorterFilter::Before:
       return prev_day - get_random_int(secs_in_day * 365, secs_in_day * 700);

    }
}


class UiTest: public QObject {


    Q_OBJECT

private slots:
    void generate_file() {

        e.seed(std::chrono::system_clock::now().time_since_epoch().count());
        std::list<std::string> type{"Человек","Машина","Здание","Дерево","..."};
        std::ifstream fread("C:/Users/lyzlo//Documents/testing/text.txt");
        std::list<std::string> words;
        if(fread.is_open()){
            std::string line;
            while (std::getline(fread, line)) {
                words.push_back({line.c_str()});
            }
        }


        std::ofstream fwrite(std::filesystem::current_path());
        if (!fwrite.is_open())
            return ;

        for (std::string& elm : words){
            object _obj;
            _obj.x = generate_random_distance();
            _obj.y = generate_random_distance();
            _obj.time = generate_random_day();
            _obj.time += get_random_float(0, 1);
            _obj.type = objectType(get_random_int(0, type.size()-1));
            Utils::fromUTF8(elm, _obj.name);
            fwrite << _obj;
            fwrite << "\n";
        };
    }
};

QTEST_MAIN(UiTest)
#include "ui_test.moc"
