//
// Created by hukq1 on 2018/11/17.
//

#ifndef THUCAL_CALTIME_H
#define THUCAL_CALTIME_H


class Caltime {
public:
    Caltime(int _year, int _month, int _monthday, int _hour = 8, int _minute = 0, int _second = 0);

    bool operator<(const Caltime &d) const;

    void SetTime(int _year, int _month, int _monthday, int _hour, int _minute, int _second);

    int second;
    int minute;
    int hour;
    int monthday;
    int month;
    int year;
    int weekday;
    int yearday;
    int isdst = 0;
};


#endif //THUCAL_CALTIME_H
