//
// Created by hukq1 on 2018/12/19.
//

#ifndef THUCAL_VEVENTNOREPEAT_H
#define THUCAL_VEVENTNOREPEAT_H

#include "Vevent.h"

class VeventNoRepeat : public Vevent {
public:
    struct time {
        std::string VALUE;
        Caltime STIME;
    } DTSTART, DTEND;

    VeventNoRepeat(char *s);

    VeventNoRepeat();
};


#endif //THUCAL_VEVENTNOREPEAT_H
