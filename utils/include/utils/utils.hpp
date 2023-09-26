#ifndef G_HPP
#define G_HPP
#include <string>
#include <vector>
#include <chrono>
#include <string>
#include <locale>
#include <codecvt>
#include <concepts>
namespace Utils {


    template <class, template <class, class> class>
    struct is_instance : public std::false_type {};

    template <class T, template <class, class> class U>
    struct is_instance<U<T, std::char_traits<T>>, U> : public std::true_type {};

    template <class T, template <class, class> class U>
    concept StringView = is_instance<T, U>::value;

    template<class T>
    requires(StringView<T, std::basic_string_view>)
    std::vector<T> split(T str, T separator){

        std::vector<T> res;
        typename T::size_type strBegin = 0;
        strBegin = str.find_first_not_of(separator, strBegin);
        while (strBegin !=  T::npos){

            auto strEnd = str.find_first_of(separator, strBegin + 1);
            if (strEnd == T::npos){

                res.push_back(str.substr(strBegin, str.size() - strBegin));
                break;
            }

            res.push_back(str.substr(strBegin, strEnd - strBegin));
            strBegin = strEnd + 1;
        }

        return res;
    }

    std::chrono::time_point<std::chrono::local_t, std::chrono::nanoseconds>
    getCurrentTime();

    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>
    convertMilisecsToSecs(long double time);
    std::tuple<int,int,int> get_year_month_day(std::chrono::year_month_day ymd);
    bool isCyrillica(char16_t chr);
    std::chrono::weekday getWeekDay(auto time){
        return std::chrono::weekday{std::chrono::floor<std::chrono::days>(time)};
    }

    template <typename T>
    std::string toUTF8(const std::basic_string<T, std::char_traits<T>, std::allocator<T>>& source)
    {
        std::string result;

        std::wstring_convert<std::codecvt_utf8_utf16<T>, T> convertor;
        result = convertor.to_bytes(source);

        return result;
    }

    template <typename T>
    void fromUTF8(const std::string& source, std::basic_string<T, std::char_traits<T>, std::allocator<T>>& result)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<T>, T> convertor;
        result = convertor.from_bytes(source);
    }

}
#endif // G_HPP
