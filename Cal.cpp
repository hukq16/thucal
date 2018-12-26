//
// Created by hukq1 on 2018/11/8.
//

#include "Cal.h"
#include <libxl.h>

#include "VeventNoRepeat.h"
#include "VeventRepeat.h"

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

void Cal::ImportExcel(std::string FilePath, int year, int month, int day) {
    int starthour[] = {8, 9, 13, 15, 17, 19};
    int startminute[] = {0, 50, 30, 20, 5, 20};
    int endhour[] = {9, 12, 15, 16, 18, 21};
    int endminute[] = {35, 15, 5, 55, 40, 45};
    std::default_random_engine e(day);
    std::uniform_int_distribution<unsigned> u(100000000, 900000000);
    unsigned seed = u(e);
    libxl::Book *book = xlCreateBook();
    int i = 0;
    if (book->load(FilePath.c_str())) {
        libxl::Sheet *sheet = book->getSheet(0);
        if (sheet) {
            for (int row = 2; row <= 7; ++row) {
                for (int col = 1; col <= 7; ++col) {
                    libxl::CellType cellType = sheet->cellType(row, col);
//                    std::cout << "(" << row << ", " << col << ") = ";
                    if (sheet->isFormula(row, col)) {
                        const char *s = (sheet->readFormula(row, col));
                    } else {
                        switch (cellType) {
                            case libxl::CELLTYPE_STRING: {
                                const char *s = (sheet->readStr(row, col));
                                if (s) {
                                    char *pts = const_cast<char *>(s);
                                    char *p;
                                    VeventRepeat *f_vec = new VeventRepeat();
                                    p = strtok_r(pts, "(", &pts);
                                    std::string sname = p;
                                    std::string sdescription = pts;
                                    sdescription = sdescription.substr(0, sdescription.length() - 1);
                                    std::string stime = sdescription.substr(sdescription.find("；") + 3);
                                    stime = stime.substr(stime.find("；") + 3);
                                    std::string slocation = stime.substr(stime.find("；") + 3);
                                    stime = stime.substr(0, stime.find("；"));
                                    f_vec->SUMMARY = sname;
                                    f_vec->repeat = true;
                                    f_vec->SEQUENCE = 0;
                                    f_vec->STATUS = "CONFIRMED";
                                    f_vec->DESCRIPTION = sdescription;
                                    f_vec->TRANSP = "OPAQUE";
                                    f_vec->RRULE.FREQ = "WEEKLY";
                                    f_vec->DTEND.TZID = "Asia/Shanghai";
                                    f_vec->DTSTART.TZID = "Asia/Shanghai";
                                    f_vec->LOCATION = slocation;
                                    f_vec->DTSTART.STIME.SetTime(year, month, day, starthour[row - 2],
                                                                 startminute[row - 2], 0);
                                    f_vec->DTEND.STIME.SetTime(year, month, day, endhour[row - 2], endminute[row - 2],
                                                               0);
                                    f_vec->DTSTART.STIME.AddDay(col - 1);
                                    f_vec->DTEND.STIME.AddDay(col - 1);
                                    std::stringstream ss2;
                                    ss2 << i << '.' << seed << "@thucal";
                                    ss2 >> f_vec->UID;
                                    if (stime == "全周") {
                                        f_vec->RRULE.COUNT = 16;
                                    } else {
                                        stime = stime.substr(stime.find('-') + 1);
                                        std::stringstream sstream;
                                        sstream << stime;
                                        sstream >> f_vec->RRULE.COUNT;
                                    }
                                    Caltime itime = f_vec->DTSTART.STIME;
                                    for (int i = 0; i < f_vec->RRULE.COUNT; ++i) {
                                        ical.insert(std::pair<Caltime, Vevent *>(itime, f_vec));
                                        itime.AddDay(f_vec->RRULE.FREQ);
                                    }
                                    i++;
                                }
                                break;
                            }
                            case libxl::CELLTYPE_BLANK:
                                break;
                            case libxl::CELLTYPE_ERROR:
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }
    }

    book->release();
}

void Cal::OutputIcs(std::string FilePath) {
    std::ofstream file(FilePath, std::ios::out);
    file << "BEGIN:VCALENDAR\n"
         << "PRODID:-//smilekzs//thucal//EN\n"
         << "VERSION:2.0\n"
         << "CALSCALE:GREGORIAN\n"
         << "METHOD:PUBLISH\n"
         << "X-WR-TIMEZONE:Asia/Shanghai\n"
         << "BEGIN:VTIMEZONE\n"
         << "TZID:Asia/Shanghai\n"
         << "X-LIC-LOCATION:Asia/Shanghai\n"
         << "BEGIN:STANDARD\n"
         << "TZOFFSETFROM:+0800\n"
         << "TZOFFSETTO:+0800\n"
         << "TZNAME:CST\n"
         << "DTSTART:19700101T000000\n"
         << "END:STANDARD\n"
         << "END:VTIMEZONE\n";
    for (auto in:ical) {
        if (in.second->repeat) {
            VeventRepeat *item = (VeventRepeat *) in.second;
            if (item->DTSTART.STIME == in.first) {
                file << "BEGIN:VEVENT\n";
                file << "SUMMARY:" << item->SUMMARY << "\n";
                file << "LOCATION:" << item->LOCATION << "\n";
                file << "DESCRIPTION:" << item->DESCRIPTION << "\n";
                file << "DTSTART;TZID=" << item->DTSTART.TZID << ":" << item->DTSTART.STIME.strintout() << "\n";
                file << "DTEND;TZID=" << item->DTSTART.TZID << ":" << item->DTEND.STIME.strintout() << "\n";
                file << "RRULE:FREQ=" << item->RRULE.FREQ << ";COUNT=" << item->RRULE.COUNT << "\n";
                file << "SEQUENCE:" << item->SEQUENCE << "\n";
                file << "UID:" << item->UID << "\n";
                file << "TRANSP:" << item->TRANSP << "\n";
                file << "STATUS:" << item->STATUS << "\n";
                file << "END:VEVENT\n";
            }

        } else {
            VeventNoRepeat *item = (VeventNoRepeat *) in.second;
            file << "BEGIN:VEVENT\n";
            file << "SUMMARY:" << item->SUMMARY << "\n";
            file << "LOCATION:" << item->LOCATION << "\n";
            file << "DESCRIPTION:" << item->DESCRIPTION << "\n";
            file << "DTSTART;TZID=" << item->DTSTART.VALUE << ":" << item->DTSTART.STIME.strintout() << "\n";
            file << "DTEND;TZID=" << item->DTEND.VALUE << ":" << item->DTEND.STIME.strintout() << "\n";
            file << "SEQUENCE:" << item->SEQUENCE << "\n";
            file << "UID:" << item->UID << "\n";
            file << "TRANSP:" << item->TRANSP << "\n";
            file << "STATUS:" << item->STATUS << "\n";
            file << "END:VEVENT\n";
        }
    }
    file << "END:VCALENDAR\n";
    file.close();
}
