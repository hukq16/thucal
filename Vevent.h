//
// Created by hukq1 on 2018/12/17.
//

#ifndef THUCAL_VEVENT_H
#define THUCAL_VEVENT_H

#include <string>
#include "Caltime.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <queue>

struct Vevent {
    std::string SUMMARY;
    std::string LOCATION;
    std::string DESCRIPTION;
    int SEQUENCE;
    std::string UID;
    std::string TRANSP;
    std::string STATUS;
    bool repeat;
};


#endif //THUCAL_VEVENT_H
