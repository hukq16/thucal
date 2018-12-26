//
// Created by hukq1 on 2018/11/17.
//

#ifndef THUCAL_CALTIME_H
#define THUCAL_CALTIME_H


#include <string>
#include <cstring>
#include <sstream>
#include <ctime>

class Caltime {
public:
    Caltime(int _year = 0, int _month = 0, int _monthday = 0, int _hour = 8, int _minute = 0, int _second = 0);

    bool operator<(const Caltime &d) const;

    bool operator==(const Caltime &d) const;

    void SetTime(int _year, int _month, int _monthday, int _hour, int _minute, int _second);

    void SetTime(char *time);

    bool AddDay(int count);

    bool AddDay(std::string freq);

    std::string stringout(int length);

    int GetYear();

    int GetMonth();

    int GetMonthDay();

    int GetHour();

    int GetMinute();

    int GetSecond();

    void SetYear(int _year);

    void SetMonth(int _month);

    void SetMonthDay(int _monthday);

    void SetHour(int _hour);

    void SetMinute(int _minute);

    void SetSecond(int _second);

private:
    int second;
    int minute;
    int hour;
    int monthday;
    int month;
    int year;
    int weekday;
    int yearday;
};


#endif //THUCAL_CALTIME_H
