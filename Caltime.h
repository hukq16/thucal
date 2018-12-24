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

    void SetTime(int _year, int _month, int _monthday, int _hour, int _minute, int _second);

    void SetTime(char *time);

    bool AddDay(int count);

    bool PrvDay(int count);
    void print() const;

    void print();

    bool AddDay(std::string freq);

    char **GetDateItem(Caltime d);

    char **GetDateItem(tm d);
    int second;
    int minute;
    int hour;
    int monthday;
    int month;
    int year;
    int weekday;
    int yearday;
    int isdst = 0;
    char **datenumber;
};


#endif //THUCAL_CALTIME_H
