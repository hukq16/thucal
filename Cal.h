//
// Created by hukq1 on 2018/11/8.
//

#ifndef THUCAL_CAL_H
#define THUCAL_CAL_H


#include <map>
#include "Vevent.h"

class Cal {
public:
    void ImportIcs(std::string FilePath);

    void ImportExcel(std::string FilePath, int year, int month, int day);

    void OutputIcs(std::string FilePath);

    void InsertItem(std::pair<Caltime, Vevent *> item);

    std::multimap<Caltime, Vevent *>::iterator FindLowerBound(Caltime dtime);

    std::multimap<Caltime, Vevent *>::iterator FindUpperBound(Caltime dtime);

    std::pair<std::multimap<Caltime, Vevent *>::iterator, std::multimap<Caltime, Vevent *>::iterator>
    FindEqualRange(Caltime dtime);

    void Erase(std::multimap<Caltime, Vevent *>::iterator item);

private:
    std::multimap<Caltime, Vevent *> ical;
};


#endif //THUCAL_CAL_H
