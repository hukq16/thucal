//
// Created by hukq1 on 2018/11/8.
//

#ifndef THUCAL_CAL_H
#define THUCAL_CAL_H


#include <map>
#include "Vevent.h"

class Cal {
public:
    /**
     * 导入ics文件
     * @param FilePath 文件路径
     */
    void ImportIcs(std::string FilePath);

    /**
     * 导入xls文件
     * @param FilePath 文件路径
     * @param year     开始年份
     * @param month    开始月份
     * @param day      开始日期
     */
    void ImportExcel(std::string FilePath, int year, int month, int day);

    /**
     * 导出ics文件
     * @param FilePath 文件路径
     */
    void OutputIcs(std::string FilePath);

    /**
     * 插入事件
     * @param item pair of 事件、指针
     */
    void InsertItem(std::pair<Caltime, Vevent *> item);

    /**
     * 查找第一个key大于或等于dtime时间的pair
     * @param  dtime 时间
     * @return       multimap的迭代器
     */
    std::multimap<Caltime, Vevent *>::iterator FindLowerBound(Caltime dtime);

    /**
     * 查找第一个key小于或等于dtime时间的pair
     * @param  dtime 时间
     * @return       multimap的迭代器
     */
    std::multimap<Caltime, Vevent *>::iterator FindUpperBound(Caltime dtime);

    /**
     * 查找key等于dtime的所有项
     * @param  dtime 时间
     * @return       两个multimap的迭代器组成的pair
     */
    std::pair<std::multimap<Caltime, Vevent *>::iterator, std::multimap<Caltime, Vevent *>::iterator>
    FindEqualRange(Caltime dtime);

    /**
     * 删除迭代器所指的项目
     * @param item 迭代器
     */
    void Erase(std::multimap<Caltime, Vevent *>::iterator item);

private:
    std::multimap<Caltime, Vevent *> ical;
};


#endif //THUCAL_CAL_H
