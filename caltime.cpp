//
// Created by hukq1 on 2018/11/17.
//

#include "caltime.h"


bool caltime::operator<(const caltime &d) const {
    if (year < d.year)
        return true;
    if (year == d.year && month < d.month)
        return true;
    if (month == d.month && monthday < d.monthday)
        return true;
    if (monthday == d.monthday && hour < d.hour)
        return true;
    if (hour == d.hour && minute < d.minute)
        return true;
    return minute == d.minute && second < d.second;
}

caltime::caltime(int _year, int _month, int _monthday, int _hour, int _minute, int _second) {
    int monthnumber[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        monthnumber[1] = 29;
    }
    year = _year;
    month = _month;
    monthday = _monthday;
    hour = _hour;
    minute = _minute;
    second = _second;
    int helpyear = 0;
    int helpmonth = 0;
    if (month == 1 || month == 2) {
        helpyear = year - 1;
        helpmonth = 12 + month;
    } else {
        helpyear = year;
        helpmonth = month;
    }
    int nY = helpyear / 100;
    int nC = helpyear % 100;
    weekday = (nY + nY / 4 + nC / 4 - 2 * nC + 26 * (helpmonth + 1) / 10 + monthday - 1) % 7;
    yearday = 0;
    for (int i = 0; i < month - 1; ++i) {
        yearday += monthnumber[i];
    }
    yearday += monthday;
}

void caltime::SetTime(int _year, int _month, int _monthday, int _hour, int _minute, int _second) {
    int monthnumber[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        monthnumber[1] = 29;
    }
    year = _year;
    month = _month;
    monthday = _monthday;
    hour = _hour;
    minute = _minute;
    second = _second;
    int helpyear = 0;
    int helpmonth = 0;
    if (month == 1 || month == 2) {
        helpyear = year - 1;
        helpmonth = 12 + month;
    } else {
        helpyear = year;
        helpmonth = month;
    }
    int nY = helpyear / 100;
    int nC = helpyear % 100;
    weekday = (nY + nY / 4 + nC / 4 - 2 * nC + 26 * (helpmonth + 1) / 10 + monthday - 1) % 7;
    yearday = 0;
    for (int i = 0; i < month - 1; ++i) {
        yearday += monthnumber[i];
    }
    yearday += monthday;
}





