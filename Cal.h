//
// Created by hukq1 on 2018/11/8.
//

#ifndef THUCAL_CAL_H
#define THUCAL_CAL_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <map>
#include "Vevent.h"
#include <random>
class Cal {
public:
    void ImportIcs(std::string FilePath);

    void ImportExcel(std::string FilePath, int year, int month, int day);

    void OutputIcs(std::string FilePath);
    std::multimap<Caltime, Vevent *> ical;
};


#endif //THUCAL_CAL_H
