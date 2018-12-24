//
// Created by hukq1 on 2018/11/17.
//

#include <cstring>
#include <iostream>
#include "Caltime.h"


bool Caltime::operator<(const Caltime &d) const {
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

Caltime::Caltime(int _year, int _month, int _monthday, int _hour, int _minute, int _second) {
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
    datenumber = new char *[50];
    for (int j = 0; j < 50; ++j) {
        datenumber[j] = new char[3];
    }
}

void Caltime::SetTime(int _year, int _month, int _monthday, int _hour, int _minute, int _second) {
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

void Caltime::SetTime(char *time) {
    int len = static_cast<int>(std::strlen(time));
    if (len == 15) {
        int _year = (time[0] - '0') * 1000 + (time[1] - '0') * 100 + (time[2] - '0') * 10 + (time[3] - '0');
        int _month = (time[4] - '0') * 10 + (time[5] - '0');
        int _monthday = (time[6] - '0') * 10 + (time[7] - '0');
        int _hour = (time[9] - '0') * 10 + (time[10] - '0');
        int _minute = (time[11] - '0') * 10 + (time[12] - '0');
        int _second = (time[13] - '0') * 10 + (time[14] - '0');
        SetTime(_year, _month, _monthday, _hour, _minute, _second);
    } else if (len == 8) {
        int _year = (time[0] - '0') * 1000 + (time[1] - '0') * 100 + (time[2] - '0') * 10 + (time[3] - '0');
        int _month = (time[4] - '0') * 10 + (time[5] - '0');
        int _monthday = (time[6] - '0') * 10 + (time[7] - '0');
        int _hour = 8;
        int _minute = 0;
        int _second = 0;
        SetTime(_year, _month, _monthday, _hour, _minute, _second);
    } else {
        SetTime(1970, 1, 1, 8, 0, 0);
    }
}

bool Caltime::AddDay(int count) {
    bool state = true;
    int monthnumber[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        monthnumber[1] = 29;
    }
    monthday += count;
    while (monthday > monthnumber[month - 1]) {
        state = false;
        monthday -= monthnumber[month - 1];
        month++;
        if (month > 12) {
            year++;
            month = 1;
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                monthnumber[1] = 29;
            }
        }
    }
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
    return state;
}

bool Caltime::AddDay(std::string freq) {
    int monthnumber[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        monthnumber[1] = 29;
    }
    if (freq == "DAILY") {
        AddDay(1);
    } else if (freq == "WEEKLY") {
        AddDay(7);
    } else if (freq == "MONTHLY") {
        if (monthday <= monthnumber[month]) {
            AddDay(monthnumber[month - 1]);
        } else {
            month++;
            return false;
        }
    } else if (freq == "YEARLY") {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            AddDay(366);
        } else {
            AddDay(365);
        }
    }
    return true;
}

void Caltime::print() const {
    std::cout << this->year << " " << this->month << " " << this->monthday << " " << this->hour << " " << this->minute
              << " " << this->second << std::endl;
}

void Caltime::print() {
    std::cout << this->year << " " << this->month << " " << this->monthday << " " << this->hour << " " << this->minute
              << " " << this->second << std::endl;
}

char **Caltime::GetDateItem(Caltime d) {
    int monthnumber[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((d.year % 4 == 0 && d.year % 100 != 0) || (d.year % 400 == 0)) {
        monthnumber[1] = 29;
    }
    Caltime firstinmonth(d.year, d.month, 1);
    int i;
    sprintf(datenumber[0], "Mo");
    sprintf(datenumber[1], "Tu");
    sprintf(datenumber[2], "We");
    sprintf(datenumber[3], "Th");
    sprintf(datenumber[4], "Fr");
    sprintf(datenumber[5], "Sa");
    sprintf(datenumber[6], "Su");
    for (i = 7; i < firstinmonth.weekday + 7; ++i) {
        strcpy(datenumber[i], "  ");
    }
    for (int j = 1; j <= monthnumber[d.month - 1]; j++, i++) {
        sprintf(datenumber[i], "%2d", j);
    }
    for (; i < 49; i++) {
        strcpy(datenumber[i], "  ");
    }
    datenumber[i] = NULL;
    return datenumber;
}

bool Caltime::PrvDay(int count) {
    bool state = true;
    int monthnumber[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        monthnumber[1] = 29;
    }
    monthday -= count;
    while (monthday < 0) {
        state = false;
        month--;
        if (month <= 0) {
            year--;
            month = 12;
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                monthnumber[1] = 29;
            }
        }
        monthday += monthnumber[month - 1];

    }
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
    return state;
}






