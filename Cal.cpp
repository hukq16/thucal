//
// Created by hukq1 on 2018/11/8.
//

#include "Cal.h"


void Cal::ImportIcs(std::string FilePath) {
    std::ifstream file(FilePath, std::ios::in);
    if (!file) {
        std::cerr << "file not exist!" << std::endl;
        return;
    }
    file.seekg(0, std::ios::end);
    long long int n = file.tellg();
    char *s = new char[n + 1];
    file.seekg(0, std::ios::beg);
    file.read(s, n);
    s[n] = '\0';
    file.close();
    char *p = std::strstr(s, "BEGIN:VEVENT");
    std::queue<char *> pvevent;
    *(p - 1) = '\0';
    pvevent.push(p);
    p += 12;
    while (p = std::strstr(p, "BEGIN:VEVENT")) {
        *(p - 1) = '\0';
        pvevent.push(p);
        p += 12;
    };

    while (!pvevent.empty()) {
        p = pvevent.front();
        if (*p != 'B')
            break;
        char *pfind = std::strstr(p, "FREQ");
        if (pfind) {
            VeventRepeat *f_vec = new VeventRepeat(p);
            Caltime itime = f_vec->DTSTART.STIME;
            for (int i = 0; i < f_vec->RRULE.COUNT; ++i) {
                ical.insert(std::pair<Caltime, Vevent *>(itime, f_vec));
                itime.AddDay(f_vec->RRULE.FREQ);
            }
        } else {
            VeventNoRepeat *f_vec = new VeventNoRepeat(p);
            ical.insert(std::pair<Caltime, Vevent *>(f_vec->DTSTART.STIME, f_vec));
        }
        pvevent.pop();
    }
}
