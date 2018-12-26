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

    /**
     * 初始化函数，设置时间
     * @param _year     年份，默认为0年
     * @param _month    月份，默认为0月
     * @param _monthday 一个月中的哪一天，默认为0日
     * @param _hour     小时，默认为8时
     * @param _minute   分钟，默认为0分
     * @param _second   秒，默认为0秒
     */
    Caltime(int _year = 0, int _month = 0, int _monthday = 0, int _hour = 8, int _minute = 0, int _second = 0);

    /**
     * 按时间比较Caltime的大小，如小，则返回true
     */
    bool operator<(const Caltime &d) const;

    /**
     * 判断时间是否相同
     */
    bool operator==(const Caltime &d) const;

    /**
     * 用于设置时间
     * @param _year     年份
     * @param _month    月份
     * @param _monthday 一个月中的哪一天
     * @param _hour     小时
     * @param _minute   分钟
     * @param _second   秒
     */
    void SetTime(int _year, int _month, int _monthday, int _hour, int _minute, int _second);

    /**
     * 多态，函数重载
     * 通过字符串设置时间
     * @param time 字符串，格式为"YYYYMMDDTHHMMSS"或"YYYYMMDD"或"YYYYMMDDHHMMSS"
     */
    void SetTime(char *time);

    /**
     * 增加特定天数
     * @param  count 天数，必须>=0
     * @return       是否增加成功
     */
    bool AddDay(int count);

    /**
     * 按照字符串的值增加特定天数
     * @param  freq 字符串，可以为"DAILY""WEEKLY""MONTHLY""YEARLY"
     * @return      是否增加成功
     */
    bool AddDay(std::string freq);

    /**
     * 按照特定格式输出字符串
     * @param  length 总字符串长度
     * @return        string字符串，格式为"YYYYMMDDTHHMMSS"或"YYYY/MM/DD-HH:MM:SS"或"YYYYMMDDHHMMSS"
     */
    std::string stringout(int length);

    /**
     * 返回年份
     * @return 年份
     */
    int GetYear();

    /**
     * 返回月份
     * @return 月份
     */
    int GetMonth();

    /**
     * 返回日期
     * @return 日期
     */
    int GetMonthDay();

    /**
     * 返回小时
     * @return 小时
     */
    int GetHour();

    /**
     * 返回分钟
     * @return 分钟
     */
    int GetMinute();

    /**
     * 返回秒
     * @return 秒
     */
    int GetSecond();

    /**
     * 设置年份
     * @param _year 年份
     */
    void SetYear(int _year);

    /**
     * 设置月份
     * @param _month 月份
     */
    void SetMonth(int _month);

    /**
     * 设置日期
     * @param _monthday 日期
     */
    void SetMonthDay(int _monthday);

    /**
     * 设置小时
     * @param _hour 小时
     */
    void SetHour(int _hour);

    /**
     * 设置分钟
     * @param _minute 分钟
     */
    void SetMinute(int _minute);

    /**
     * 设置秒
     * @param _second 秒
     */
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
