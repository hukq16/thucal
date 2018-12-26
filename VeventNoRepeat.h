//
// Created by hukq1 on 2018/12/19.
//

#ifndef THUCAL_VEVENTNOREPEAT_H
#define THUCAL_VEVENTNOREPEAT_H

#include "Vevent.h"

struct dtime {
    std::string VALUE;
    Caltime STIME;
};

struct VeventNoRepeat : public Vevent {
    struct dtime DTSTART, DTEND;

    /**
     * 通过字符串初始化
     * @param s 字符串
     */
    VeventNoRepeat(char *s);

    /**
     * 默认初始化函数
     */
    VeventNoRepeat();
};


#endif //THUCAL_VEVENTNOREPEAT_H
