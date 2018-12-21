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

class Vevent {
public:
    friend class Caltime;

//    friend std::istream & operator >> (std::istream &in, Vevent &obj);
    std::string SUMMARY;
    std::string LOCATION;
    std::string DESCRIPTION;
    int SEQUENCE;
    std::string UID;
    std::string TRANSP;
    std::string STATUS;
    bool repeat;
//    void clear();
};


#endif //THUCAL_VEVENT_H
