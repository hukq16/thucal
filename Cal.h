//
// Created by hukq1 on 2018/11/8.
//

#ifndef THUCAL_CAL_H
#define THUCAL_CAL_H

#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <map>
#include "Caltime.h"
#include "VeventNoRepeat.h"
#include "VeventRepeat.h"


class Cal {
public:
    void ImportIcs(std::string FilePath);

    std::multimap<Caltime, Vevent *> ical;
};


#endif //THUCAL_CAL_H
