#include "utils/utils.hpp"
#include <codecvt>

namespace Utils {



    std::chrono::time_point<std::chrono::local_t, std::chrono::nanoseconds>
    getCurrentTime()
    {
        auto const time = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
        return time;
    }


    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>
    convertMilisecsToSecs(long double time){

        using namespace std::chrono_literals;
        auto ms = time * 1s;
        return std::chrono::system_clock::time_point{std::chrono::duration_cast<std::chrono::milliseconds>(ms)};

    }

    // incorrect working
    bool isCyrillica(char16_t chr)
    {
        std::u16string a = u"а";
        std::u16string z = u"я";
        std::u16string A = u"А";
        std::u16string Z = u"Я";
        return (chr >= static_cast<char16_t>(a[0]) && chr <= static_cast<char16_t>(z[0])) || (chr >= static_cast<char16_t>(A[0]) && chr <= static_cast<char16_t>(Z[0]));
    }


    std::tuple<int,int,int> get_year_month_day(std::chrono::year_month_day ymd){

        int year = static_cast<int>(ymd.year());
        int month = static_cast<unsigned>(ymd.month());
        int day = static_cast<unsigned>(ymd.day());
        return {year, month, day};
    }


}
