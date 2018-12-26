#include <iostream>
#include <curses.h>
#include <unistd.h>
#include <menu.h>
#include <cdk.h>
#include <ctime>
#include <string>
#include "Todolist.h"
#include "Cal.h"
#include <locale.h>
#include "VeventRepeat.h"
#include "VeventNoRepeat.h"

#define MAXBUFSIZE 1024
static CDKOBJS *all_objects[3];
using namespace std;
#define NumElements(a) ((sizeof a)/(sizeof a[0]))
#define VECTORLENGTH 23
#define VECTORDEFAULTSIZE 100
#define VEVENTLENGTH 55
Cal vevent;
//char todolistitem[VECTORDEFAULTSIZE][VECTORLENGTH];
//char Veventlistitem[VECTORDEFAULTSIZE][VEVENTLENGTH];
static int changedayCB(EObjectType cdktype GCC_UNUSED,
                       void *object,
                       void *clientData GCC_UNUSED,
                       chtype key GCC_UNUSED);

static int addtodolistCB(EObjectType cdktype GCC_UNUSED,
                         void *object,
                         void *clientData GCC_UNUSED,
                         chtype key GCC_UNUSED);

static int addVeventCB(EObjectType cdktype GCC_UNUSED,
                       void *object,
                       void *clientData GCC_UNUSED,
                       chtype key GCC_UNUSED);

static int deleteVeventCB(EObjectType cdktype GCC_UNUSED,
                          void *object,
                          void *clientData GCC_UNUSED,
                          chtype key GCC_UNUSED);

static int deletetodolistCB(EObjectType cdktype GCC_UNUSED,
                            void *object,
                            void *clientData GCC_UNUSED,
                            chtype key GCC_UNUSED);

static int edittodolistCB(EObjectType cdktype GCC_UNUSED,
                          void *object,
                          void *clientData GCC_UNUSED,
                          chtype key GCC_UNUSED);

static int editVeventCB(EObjectType cdktype GCC_UNUSED,
                        void *object,
                        void *clientData GCC_UNUSED,
                        chtype key GCC_UNUSED);

static int popeventdetailCB(EObjectType cdktype GCC_UNUSED,
                            void *object,
                            void *clientData GCC_UNUSED,
                            chtype key GCC_UNUSED);

static int ImportIcsCB(EObjectType cdktype GCC_UNUSED,
                       void *object,
                       void *clientData GCC_UNUSED,
                       chtype key GCC_UNUSED);

static int ImportXlsCB(EObjectType cdktype GCC_UNUSED,
                       void *object,
                       void *clientData GCC_UNUSED,
                       chtype key GCC_UNUSED);

static int OutputIcsCB(EObjectType cdktype GCC_UNUSED,
                       void *object,
                       void *clientData GCC_UNUSED,
                       chtype key GCC_UNUSED);

static int CalHelpCB(EObjectType cdktype GCC_UNUSED,
                     void *object,
                     void *clientData GCC_UNUSED,
                     chtype key GCC_UNUSED);

static int VevnetHelpCB(EObjectType cdktype GCC_UNUSED,
                        void *object,
                        void *clientData GCC_UNUSED,
                        chtype key GCC_UNUSED);

static int TodolisthelpCB(EObjectType cdktype GCC_UNUSED,
                          void *object,
                          void *clientData GCC_UNUSED,
                          chtype key GCC_UNUSED);

std::string popentry(const char *title, const char *label);

std::string poptime(const char *title, const char *label);

std::string popentry(const char *title, const char *label, const char *val);

std::string poptime(const char *title, const char *label, const char *val);

std::string popdate(const char *title, const char *label);
CDKSCREEN *cdkscreen = NULL;
CDKSCROLL *Todolistwidget;
CDKSCROLL *Veventwidget;
CDKCALENDAR *CalWidget;
Todolist todolist;
int main(int argc, char const *argv[]) {

    vevent.ImportIcs("database.ics");
    VeventNoRepeat *f_vec = new VeventNoRepeat;
    f_vec->SUMMARY = "test";
    f_vec->LOCATION = "skyworks";
    f_vec->STATUS = "CONFIRMED";
    f_vec->DESCRIPTION = "test app";
    f_vec->TRANSP = "OPAQUE";
    f_vec->UID = "1.36142813447@thucal";
    f_vec->DTEND.VALUE = "Asia/Shanghai";
    f_vec->DTEND.STIME.SetTime(2018, 12, 25, 16, 20, 0);
    f_vec->DTSTART.VALUE = "Asia/Shanghai";
    f_vec->DTSTART.STIME.SetTime(2018, 12, 25, 17, 20, 0);
    f_vec->repeat = false;
    vevent.ical.insert(pair<Caltime, Vevent *>(f_vec->DTSTART.STIME, f_vec));
    int CalWinstartx, CalWinstarty, CalWinwidth, CalWinheight;
    int TodoWinstartx, TodoWinstarty, TodoWinwidth, TodoWinheight;
    int VeventWinstartx, VeventWinstarty, VeventWinwidth, VeventWinheight;
    int HelpWinstartx, HelpWinstarty, HelpWinwidth, HelpWinheight;
    CalWinheight = 12;
    CalWinwidth = 24;
    CalWinstarty = 0;
    CalWinstartx = (COLS - CalWinwidth);

    TodoWinheight = LINES - CalWinheight;
    TodoWinwidth = CalWinwidth - 1;
    TodoWinstarty = CalWinheight;
    TodoWinstartx = (COLS - TodoWinwidth);

    VeventWinheight = LINES;
    VeventWinwidth = COLS - CalWinwidth - 1;
    VeventWinstarty = 0;
    VeventWinstartx = 0;

    setlocale(LC_ALL, "");

    struct tm *dateInfo;
    time_t clck, retVal;
    int day;
    int month;
    int year;
    time(&clck);
    dateInfo = gmtime(&clck);

    day = dateInfo->tm_mday;
    month = dateInfo->tm_mon + 1;
    year = dateInfo->tm_year + 1900;


    char **todolistitem = new char *[VECTORDEFAULTSIZE];
    for (int i = 0; i < VECTORDEFAULTSIZE; ++i) {
        todolistitem[i] = new char[VECTORLENGTH];
    }

    char **Veventlistitem = new char *[VECTORDEFAULTSIZE];
    for (int i = 0; i < VECTORDEFAULTSIZE; ++i) {
        Veventlistitem[i] = new char[VEVENTLENGTH];
    }

    todolist.InsertTodo(2, "jsdaffk");
    todolist.InsertTodo(5, "dfasdfsadfsad");
    todolist.InsertTodo(7, "s");
    int i = 0;

    cdkscreen = initCDKScreen(NULL);
    initCDKColor();
    for (auto &it:todolist.v) {
        std::stringstream sstream;
        sstream << it.first << " " << it.second << std::endl;
        sstream.getline(todolistitem[i], VECTORLENGTH);
        i++;
    }
    const char *mesg[3];
    const char *mesg2[3];
    mesg2[0] = "Press h for help";
    mesg2[1] = "";
    mesg2[2] = "<C>Press any key to continue.";
    popupLabel(cdkscreen, (CDK_CSTRING2) mesg2, 3);

    Caltime today(year, month, day, 0, 0, 0);

    auto mbegin = vevent.ical.lower_bound(today);
    Caltime tomorrow(year, month, day, 23, 59, 59);
    auto mend = vevent.ical.upper_bound(tomorrow);
    int j = 0;
    for (auto it = mbegin; it != mend; ++it) {
        std::stringstream sstream;
        if (it->second->repeat) {
            auto *item = (VeventRepeat *) it->second;
            char summary[100];
            strcpy(summary, item->SUMMARY.c_str());
            sprintf(Veventlistitem[j], "%02d:%02d:%02d->%02d:%02d:%02d  %s", item->DTSTART.STIME.hour,
                    item->DTSTART.STIME.minute, item->DTSTART.STIME.second, item->DTEND.STIME.hour,
                    item->DTEND.STIME.minute, item->DTEND.STIME.second, summary);
        } else {
            auto *item = (VeventNoRepeat *) it->second;
            char summary[100];
            strcpy(summary, item->SUMMARY.c_str());
            sprintf(Veventlistitem[j], "%02d:%02d:%02d->%02d:%02d:%02d  %s", item->DTSTART.STIME.hour,
                    item->DTSTART.STIME.minute, item->DTSTART.STIME.second, item->DTEND.STIME.hour,
                    item->DTEND.STIME.minute, item->DTEND.STIME.second, summary);
        }
        j++;
    }

    CalWidget = newCDKCalendar(cdkscreen, RIGHT, CalWinstarty,
                               "<C>Calendar", day, month, year,
                                            COLOR_PAIR (16) | A_BOLD,
                                            COLOR_PAIR (24) | A_BOLD,
                                            COLOR_PAIR (32) | A_BOLD,
                                            COLOR_PAIR (40) | A_REVERSE,
                               TRUE,
                               FALSE);
    all_objects[0] = ObjPtr(CalWidget);
    Veventwidget = newCDKScroll(cdkscreen,
                                LEFT,
                                VeventWinstarty,
                                RIGHT,
                                VeventWinheight,
                                VeventWinwidth,
                                "<C>Appointments",
                                (CDK_CSTRING2) Veventlistitem, j,
                                FALSE,
                                A_REVERSE,
                                TRUE,
                                FALSE);
    all_objects[1] = ObjPtr(Veventwidget);

    Todolistwidget = newCDKScroll(cdkscreen,
                                  RIGHT,
                                  TodoWinstarty,
                                  RIGHT,
                                  TodoWinheight,
                                  TodoWinwidth,
                                  "<C>Todolist",
                                  (CDK_CSTRING2) todolistitem, i,
                                  FALSE,
                                  A_REVERSE,
                                  TRUE,
                                  FALSE);
    all_objects[2] = ObjPtr(Todolistwidget);
    bindCDKObject(vCALENDAR, CalWidget, KEY_ENTER, changedayCB, Veventlistitem);
    bindCDKObject(vSCROLL, Todolistwidget, 'a', addtodolistCB, todolistitem);
    bindCDKObject(vSCROLL, Veventwidget, 'a', addVeventCB, Veventlistitem);
    bindCDKObject(vSCROLL, Veventwidget, 'd', deleteVeventCB, Veventlistitem);
    bindCDKObject(vSCROLL, Todolistwidget, 'd', deletetodolistCB, todolistitem);
    bindCDKObject(vSCROLL, Todolistwidget, 'e', edittodolistCB, todolistitem);
    bindCDKObject(vSCROLL, Veventwidget, 'e', editVeventCB, Veventlistitem);
    bindCDKObject(vSCROLL, Veventwidget, KEY_ENTER, popeventdetailCB, Veventlistitem);

    bindCDKObject(vCALENDAR, CalWidget, 'h', CalHelpCB, 0);
    bindCDKObject(vSCROLL, Todolistwidget, 'h', TodolisthelpCB, 0);
    bindCDKObject(vSCROLL, Veventwidget, 'h', VevnetHelpCB, 0);

    bindCDKObject(vCALENDAR, CalWidget, 'i', ImportIcsCB, Veventlistitem);
    bindCDKObject(vSCROLL, Todolistwidget, 'i', ImportIcsCB, Veventlistitem);
    bindCDKObject(vSCROLL, Veventwidget, 'i', ImportIcsCB, Veventlistitem);

    bindCDKObject(vCALENDAR, CalWidget, 'x', ImportXlsCB, Veventlistitem);
    bindCDKObject(vSCROLL, Todolistwidget, 'x', ImportXlsCB, Veventlistitem);
    bindCDKObject(vSCROLL, Veventwidget, 'x', ImportXlsCB, Veventlistitem);

    bindCDKObject(vCALENDAR, CalWidget, 'o', OutputIcsCB, 0);
    bindCDKObject(vSCROLL, Todolistwidget, 'o', OutputIcsCB, 0);
    bindCDKObject(vSCROLL, Veventwidget, 'o', OutputIcsCB, 0);

    refreshCDKScreen(cdkscreen);
    traverseCDKScreen(cdkscreen);
    mesg[0] = "Done";
    mesg[1] = "";
    mesg[2] = "<C>Press any key to continue.";
    popupLabel(cdkscreen, (CDK_CSTRING2) mesg, 3);
    _destroyCDKObject(all_objects[2]);
    destroyCDKScreen(cdkscreen);
    endCDK();
    vevent.OutputIcs("database.ics");
    return 0;
}


static int changedayCB(EObjectType cdktype GCC_UNUSED,
                       void *object,
                       void *clientData GCC_UNUSED,
                       chtype key GCC_UNUSED) {

    char **Veventlistitem = (char **) (clientData);
    CDKCALENDAR *calendar = (CDKCALENDAR *) object;
    const char *mesg[5];

    Caltime today(calendar->year, calendar->month, calendar->day, 0, 0, 0);

    auto mbegin = vevent.ical.lower_bound(today);
    Caltime tomorrow(calendar->year, calendar->month, calendar->day, 23, 59, 59);
    auto mend = vevent.ical.upper_bound(tomorrow);
    int j = 0;
    for (auto it = mbegin; it != mend; ++it) {
        std::stringstream sstream;
        if (it->second->repeat) {
            auto *item = (VeventRepeat *) it->second;
            char summary[100];
            strcpy(summary, item->SUMMARY.c_str());
            summary[strlen(item->SUMMARY.c_str())] = '\0';
            sprintf(Veventlistitem[j], "%02d:%02d:%02d->%02d:%02d:%02d  %s", item->DTSTART.STIME.hour,
                    item->DTSTART.STIME.minute, item->DTSTART.STIME.second, item->DTEND.STIME.hour,
                    item->DTEND.STIME.minute, item->DTEND.STIME.second, summary);

        } else {
            auto *item = (VeventNoRepeat *) it->second;
            char summary[100];
            strcpy(summary, item->SUMMARY.c_str());
            summary[strlen(item->SUMMARY.c_str())] = '\0';
            sprintf(Veventlistitem[j], "%02d:%02d:%02d->%02d:%02d:%02d  %s", item->DTSTART.STIME.hour,
                    item->DTSTART.STIME.minute, item->DTSTART.STIME.second, item->DTEND.STIME.hour,
                    item->DTEND.STIME.minute, item->DTEND.STIME.second, summary);
        }
        j++;
    }
    eraseCDKScroll(Veventwidget);
    setCDKScroll(Veventwidget, (CDK_CSTRING2) Veventlistitem, j, FALSE, A_REVERSE, TRUE);
    drawCDKScroll(Veventwidget, TRUE);
    return (FALSE);
}


static int addtodolistCB(EObjectType cdktype GCC_UNUSED,
                         void *object,
                         void *clientData GCC_UNUSED,
                         chtype key GCC_UNUSED) {

    char **todolistitem = (char **) clientData;
    const char *sumtitle = "Enter todo things.";
    const char *sumlabel = "</U/5>summary:<!U!5>";
    char *summary;
    char temp[256];
    CDKSCROLL *todolistwidget = (CDKSCROLL *) object;
    CDKENTRY *directory = 0;

    directory = newCDKEntry(cdkscreen, CENTER, CENTER, sumtitle, sumlabel, A_NORMAL, '_', vMIXED, 40, 0, 256, TRUE,
                            FALSE);
    summary = activateCDKEntry(directory, 0);
    std::string summ = summary;
    destroyCDKEntry(directory);
    eraseCDKScreen(cdkscreen);
    refreshCDKScreen(cdkscreen);


    CDKSCALE *importance = 0;
    const char *imptitle = "<C>Select a value";
    const char *implabel = "</5>Current value";
    int iimport;
    importance = newCDKScale(cdkscreen, CENTER, CENTER, imptitle, implabel, A_NORMAL, 5, 1, 1, 9, 1, 2, TRUE, FALSE);
    iimport = activateCDKScale(importance, 0);
    destroyCDKScale(importance);
    eraseCDKScreen(cdkscreen);
    refreshCDKScreen(cdkscreen);
    todolist.InsertTodo(iimport, summ);
    int i = 0;
    for (auto &it:todolist.v) {
        std::stringstream sstream;
        sstream << it.first << " " << it.second << std::endl;
        sstream.getline(todolistitem[i], VECTORLENGTH);
        i++;
    }
    eraseCDKScroll(Todolistwidget);
    setCDKScroll(Todolistwidget, (CDK_CSTRING2) todolistitem, i, FALSE, A_REVERSE, TRUE);
    drawCDKScroll(Todolistwidget, TRUE);
    return (FALSE);
}


static int addVeventCB(EObjectType cdktype GCC_UNUSED,
                       void *object,
                       void *clientData GCC_UNUSED,
                       chtype key GCC_UNUSED) {

    char **Veventlistitem = (char **) clientData;
    const char *sumtitle = "Enter event summary.";
    const char *sumlabel = "</U/5>summary:<!U!5>";
    std::string summary = popentry(sumtitle, sumlabel);

    const char *destitle = "Enter event description.";
    const char *deslabel = "</U/5>description:<!U!5>";
    std::string description = popentry(destitle, deslabel);

    const char *loctitle = "Enter event location.";
    const char *loclabel = "</U/5>location:<!U!5>";
    std::string location = popentry(loctitle, loclabel);


    const char *startimetitle = "Enter start time.";
    const char *startimelabel = "</U/5>start time:<!U!5>";
    std::string starttime = poptime(startimetitle, startimelabel);


    const char *endtimetitle = "Enter end time.";
    const char *endtimelabel = "</U/5>end time:<!U!5>";
    std::string endtime = poptime(endtimetitle, endtimelabel);


    const char *message[2];
    message[0] = "<C></U>Is the event repeat?";
    const char *buttons[] =
            {"</B/24>No", "</B/24>Yes"};
    int choice = popupDialog(cdkscreen, (CDK_CSTRING2) message, 1, (CDK_CSTRING2) buttons, 2);

    if (choice == 1) {

        std::string freq[] = {"DAILY", "WEEKLY", "MONTHLY", "YEARLY"};
        const char *repeatmessage[2];
        repeatmessage[0] = "<C></U>choose repeat kind";
        const char *repeatbuttons[] =
                {"</B/24>DAILY", "</B/24>WEEKLY", "</B/24>MONTHLY", "</B/24>YEARLY"};
        int repeatchoice = popupDialog(cdkscreen, (CDK_CSTRING2) repeatmessage, 1, (CDK_CSTRING2) repeatbuttons, 4);

        CDKSCALE *repeatscale = 0;
        const char *reptitle = "<C>Select repeat count";
        const char *replabel = "</5>repeat count";
        int repeatcount;
        repeatscale = newCDKScale(cdkscreen, CENTER, CENTER, reptitle, replabel, A_NORMAL, 5, 1, 1, 999, 1, 5, TRUE,
                                  FALSE);
        repeatcount = activateCDKScale(repeatscale, 0);
        destroyCDKScale(repeatscale);
        eraseCDKScreen(cdkscreen);
        refreshCDKScreen(cdkscreen);

        VeventRepeat *f_vec = new VeventRepeat;
        f_vec->SUMMARY = summary;
        f_vec->repeat = true;
        f_vec->SEQUENCE = 0;
        f_vec->STATUS = "CONFIRMED";
        f_vec->DESCRIPTION = description;
        f_vec->TRANSP = "OPAQUE";
        f_vec->RRULE.FREQ = freq[repeatchoice];
        f_vec->DTEND.TZID = "Asia/Shanghai";
        f_vec->DTSTART.TZID = "Asia/Shanghai";
        f_vec->LOCATION = location;
        char *charstarttime = new char[20];
        strcpy(charstarttime, starttime.c_str());
        f_vec->DTSTART.STIME.SetTime(charstarttime);
        char *charendtime = new char[20];
        strcpy(charendtime, starttime.c_str());
        f_vec->DTEND.STIME.SetTime(charendtime);
        std::stringstream ss2;
        ss2 << time(0) << '.' << "34214723058046557546435" << "@thucal";
        ss2 >> f_vec->UID;
        f_vec->RRULE.COUNT = repeatcount;
        Caltime itime = f_vec->DTSTART.STIME;
        for (int i = 0; i < f_vec->RRULE.COUNT; ++i) {
            vevent.ical.insert(std::pair<Caltime, Vevent *>(itime, f_vec));
            itime.AddDay(f_vec->RRULE.FREQ);
        }
    } else {
        VeventNoRepeat *f_vec = new VeventNoRepeat;
        f_vec->SUMMARY = summary;
        f_vec->repeat = false;
        f_vec->SEQUENCE = 0;
        f_vec->STATUS = "CONFIRMED";
        f_vec->DESCRIPTION = description;
        f_vec->TRANSP = "OPAQUE";
        f_vec->DTEND.VALUE = "Asia/Shanghai";
        f_vec->DTSTART.VALUE = "Asia/Shanghai";
        f_vec->LOCATION = location;
        char *charstarttime = new char[20];
        strcpy(charstarttime, starttime.c_str());
        f_vec->DTSTART.STIME.SetTime(charstarttime);
        char *charendtime = new char[20];
        strcpy(charendtime, starttime.c_str());
        f_vec->DTEND.STIME.SetTime(charendtime);
        std::stringstream ss2;
        ss2 << time(0) << '.' << "34214723058046557546435" << "@thucal";
        ss2 >> f_vec->UID;
        vevent.ical.insert(std::pair<Caltime, Vevent *>(f_vec->DTSTART.STIME, f_vec));
    }

    Caltime today(CalWidget->year, CalWidget->month, CalWidget->day, 0, 0, 0);

    auto mbegin = vevent.ical.lower_bound(today);
    Caltime tomorrow(CalWidget->year, CalWidget->month, CalWidget->day, 23, 59, 59);
    auto mend = vevent.ical.upper_bound(tomorrow);
    int j = 0;
    for (auto it = mbegin; it != mend; ++it) {
        std::stringstream sstream;
        if (it->second->repeat) {
            auto *item = (VeventRepeat *) it->second;
            char summary[100];
            strcpy(summary, item->SUMMARY.c_str());
            sprintf(Veventlistitem[j], "%02d:%02d:%02d->%02d:%02d:%02d  %s", item->DTSTART.STIME.hour,
                    item->DTSTART.STIME.minute, item->DTSTART.STIME.second, item->DTEND.STIME.hour,
                    item->DTEND.STIME.minute, item->DTEND.STIME.second, summary);
        } else {
            auto *item = (VeventNoRepeat *) it->second;
            char summary[100];
            strcpy(summary, item->SUMMARY.c_str());
            sprintf(Veventlistitem[j], "%02d:%02d:%02d->%02d:%02d:%02d  %s", item->DTSTART.STIME.hour,
                    item->DTSTART.STIME.minute, item->DTSTART.STIME.second, item->DTEND.STIME.hour,
                    item->DTEND.STIME.minute, item->DTEND.STIME.second, summary);
        }
        j++;
    }
    eraseCDKScroll(Veventwidget);
    setCDKScroll(Veventwidget, (CDK_CSTRING2) Veventlistitem, j, FALSE, A_REVERSE, TRUE);
    drawCDKScroll(Veventwidget, TRUE);
    return (FALSE);
}


std::string popentry(const char *title, const char *label) {
    char *charvalue;
    char temp[256];

    CDKENTRY *entry = 0;

    entry = newCDKEntry(cdkscreen, CENTER, CENTER, title, label, A_NORMAL, '_', vMIXED, 40, 0, 256, TRUE, FALSE);
    charvalue = activateCDKEntry(entry, 0);
    std::string svalue = charvalue;
    destroyCDKEntry(entry);
    eraseCDKScreen(cdkscreen);
    refreshCDKScreen(cdkscreen);
    return svalue;
}

std::string poptime(const char *title, const char *label) {
    char *charvalue;
    char temp[256];
    char *info;
    char *mixed;
    const char *Overlay = "____/__/__-__:__:__";
    const char *plate = "####/##/##-##:##:##";
    CDKTEMPLATE *cdktemplate = 0;

    cdktemplate = newCDKTemplate(cdkscreen, CENTER, CENTER, title, label, plate, Overlay, TRUE, FALSE);
    info = activateCDKTemplate(cdktemplate, 0);
    std::string time = info;
    destroyCDKEntry(cdktemplate);
    eraseCDKScreen(cdkscreen);
    refreshCDKScreen(cdkscreen);
    return time;
}


static int deleteVeventCB(EObjectType cdktype GCC_UNUSED,
                          void *object,
                          void *clientData GCC_UNUSED,
                          chtype key GCC_UNUSED) {

    CDKSCROLL *veventwidget = (CDKSCROLL *) object;
    char **Veventlistitem = (char **) clientData;
    Caltime today(CalWidget->year, CalWidget->month, CalWidget->day, 0, 0, 0);

    auto deleteitem = vevent.ical.lower_bound(today);
    for (int i = 0; i < veventwidget->currentItem; ++i) {
        deleteitem++;
    }
    const char *message[2];
    message[0] = "<C></U>really to delete?";
    const char *buttons[] =
            {"</B/24>No", "</B/24>Yes"};
    int choice = popupDialog(cdkscreen, (CDK_CSTRING2) message, 1, (CDK_CSTRING2) buttons, 2);
    if (choice == 1) {
        if (deleteitem->second->repeat) {
            VeventRepeat *deleterepeat = (VeventRepeat *) deleteitem->second;
            Caltime deletetime = deleterepeat->DTSTART.STIME;
            for (int i = 0; i < deleterepeat->RRULE.COUNT; ++i) {
                auto pair2 = vevent.ical.equal_range(deletetime);
                for (auto j = pair2.first; j != pair2.second; j++) {
                    if (j->second == deleterepeat) {
                        vevent.ical.erase(j);
                        break;
                    }
                }
                deletetime.AddDay(deleterepeat->RRULE.FREQ);
            }
            delete deleterepeat;
        } else {
            VeventNoRepeat *deletenorepeat = (VeventNoRepeat *) deleteitem->second;
            Caltime deletetime = deletenorepeat->DTSTART.STIME;
            auto pair2 = vevent.ical.equal_range(deletetime);
            for (auto j = pair2.first; j != pair2.second; j++) {
                if (j->second == deletenorepeat) {
                    vevent.ical.erase(j);
                    break;
                }
            }
            delete deletenorepeat;
        }

    }


    auto mbegin = vevent.ical.lower_bound(today);
    Caltime tomorrow(CalWidget->year, CalWidget->month, CalWidget->day, 23, 59, 59);
    auto mend = vevent.ical.upper_bound(tomorrow);
    int j = 0;
    for (auto it = mbegin; it != mend; ++it) {
        std::stringstream sstream;
        if (it->second->repeat) {
            auto *item = (VeventRepeat *) it->second;
            char summary[100];
            strcpy(summary, item->SUMMARY.c_str());
            sprintf(Veventlistitem[j], "%02d:%02d:%02d->%02d:%02d:%02d  %s", item->DTSTART.STIME.hour,
                    item->DTSTART.STIME.minute, item->DTSTART.STIME.second, item->DTEND.STIME.hour,
                    item->DTEND.STIME.minute, item->DTEND.STIME.second, summary);
        } else {
            auto *item = (VeventNoRepeat *) it->second;
            char summary[100];
            strcpy(summary, item->SUMMARY.c_str());
            sprintf(Veventlistitem[j], "%02d:%02d:%02d->%02d:%02d:%02d  %s", item->DTSTART.STIME.hour,
                    item->DTSTART.STIME.minute, item->DTSTART.STIME.second, item->DTEND.STIME.hour,
                    item->DTEND.STIME.minute, item->DTEND.STIME.second, summary);
        }
        j++;
    }


    setCDKScroll(Veventwidget, (CDK_CSTRING2) Veventlistitem, j, FALSE, A_REVERSE, TRUE);
    eraseCDKScroll(Veventwidget);
    drawCDKScroll(Veventwidget, TRUE);
    return (FALSE);

}

static int deletetodolistCB(EObjectType cdktype GCC_UNUSED,
                            void *object,
                            void *clientData GCC_UNUSED,
                            chtype key GCC_UNUSED) {

    CDKSCROLL *todolistwidget = (CDKSCROLL *) object;
    char **todolistitem = (char **) clientData;

    auto deleteitem = todolist.v.begin();
    for (int i = 0; i < todolistwidget->currentItem; ++i) {
        deleteitem++;
    }
    const char *message[2];
    message[0] = "<C></U>really to delete?";
    const char *buttons[] =
            {"</B/24>No", "</B/24>Yes"};
    int choice = popupDialog(cdkscreen, (CDK_CSTRING2) message, 1, (CDK_CSTRING2) buttons, 2);
    if (choice == 1) {
        todolist.v.erase(deleteitem);
    }

    int i = 0;
    for (auto &it:todolist.v) {
        std::stringstream sstream;
        sstream << it.first << " " << it.second << std::endl;
        sstream.getline(todolistitem[i], VECTORLENGTH);
        i++;
    }
    eraseCDKScroll(Todolistwidget);
    setCDKScroll(Todolistwidget, (CDK_CSTRING2) todolistitem, i, FALSE, A_REVERSE, TRUE);
    drawCDKScroll(Todolistwidget, TRUE);
    return (FALSE);


}


static int edittodolistCB(EObjectType cdktype GCC_UNUSED,
                          void *object,
                          void *clientData GCC_UNUSED,
                          chtype key GCC_UNUSED) {

    CDKSCROLL *todolistwidget = (CDKSCROLL *) object;
    char **todolistitem = (char **) clientData;

    auto edititem = todolist.v.begin();
    for (int i = 0; i < todolistwidget->currentItem; ++i) {
        edititem++;
    }

    char summ[256];
    strcpy(summ, edititem->second.c_str());
    const char *summer = summ;
    int preimport = edititem->first;
    todolist.v.erase(edititem);


    const char *sumtitle = "Enter todo things.";
    const char *sumlabel = "</U/5>summary:<!U!5>";
    std::string summary = popentry(sumtitle, sumlabel, summer);


    CDKSCALE *importance = 0;
    const char *imptitle = "<C>Select a value";
    const char *implabel = "</5>Current value";
    int iimport;
    importance = newCDKScale(cdkscreen, CENTER, CENTER, imptitle, implabel, A_NORMAL, 5, preimport, 1, 9, 1, 2, TRUE,
                             FALSE);
    iimport = activateCDKScale(importance, 0);
    destroyCDKScale(importance);
    eraseCDKScreen(cdkscreen);
    refreshCDKScreen(cdkscreen);
    todolist.InsertTodo(iimport, summary);

    int i = 0;
    for (auto &it:todolist.v) {
        std::stringstream sstream;
        sstream << it.first << " " << it.second << std::endl;
        sstream.getline(todolistitem[i], VECTORLENGTH);
        i++;
    }
    eraseCDKScroll(Todolistwidget);
    setCDKScroll(Todolistwidget, (CDK_CSTRING2) todolistitem, i, FALSE, A_REVERSE, TRUE);
    drawCDKScroll(Todolistwidget, TRUE);
    return (FALSE);
}


std::string popentry(const char *title, const char *label, const char *val) {
    char *charvalue;
    char temp[256];

    CDKENTRY *entry = 0;

    entry = newCDKEntry(cdkscreen, CENTER, CENTER, title, label, A_NORMAL, '_', vMIXED, 40, 0, 256, TRUE, FALSE);
    setCDKEntry(entry, val, 0, 256, TRUE);
    charvalue = activateCDKEntry(entry, 0);
    std::string svalue = charvalue;
    destroyCDKEntry(entry);
    eraseCDKScreen(cdkscreen);
    refreshCDKScreen(cdkscreen);
    return svalue;
}


static int editVeventCB(EObjectType cdktype GCC_UNUSED,
                        void *object,
                        void *clientData GCC_UNUSED,
                        chtype key GCC_UNUSED) {

    CDKSCROLL *veventwidget = (CDKSCROLL *) object;
    char **Veventlistitem = (char **) clientData;
    Caltime today(CalWidget->year, CalWidget->month, CalWidget->day, 0, 0, 0);

    auto edititem = vevent.ical.lower_bound(today);
    for (int i = 0; i < veventwidget->currentItem; ++i) {
        edititem++;
    }

    if (edititem->second->repeat) {
        VeventRepeat *editptr = (VeventRepeat *) edititem->second;

        char summtemp[256];
        strcpy(summtemp, editptr->SUMMARY.c_str());
        const char *summconsttemp = summtemp;
        const char *sumtitle = "Enter event summary.";
        const char *sumlabel = "</U/5>summary:<!U!5>";
        std::string summary = popentry(sumtitle, sumlabel, summconsttemp);


        char desctemp[256];
        strcpy(desctemp, editptr->DESCRIPTION.c_str());
        const char *descconsttemp = desctemp;
        const char *destitle = "Enter event description.";
        const char *deslabel = "</U/5>description:<!U!5>";
        std::string description = popentry(destitle, deslabel, descconsttemp);

        char locatemp[256];
        strcpy(locatemp, editptr->LOCATION.c_str());
        const char *locaconsttemp = locatemp;
        const char *loctitle = "Enter event location.";
        const char *loclabel = "</U/5>location:<!U!5>";
        std::string location = popentry(loctitle, loclabel, locaconsttemp);

        char starttemp[256];
        strcpy(starttemp, editptr->DTSTART.STIME.strintout2().c_str());
        const char *startconsttemp = starttemp;
        const char *startimetitle = "Enter start time.";
        const char *startimelabel = "</U/5>start time:<!U!5>";
        std::string starttime = poptime(startimetitle, startimelabel, startconsttemp);

        char endtemp[256];
        strcpy(endtemp, editptr->DTEND.STIME.strintout2().c_str());
        const char *endconsttemp = endtemp;
        const char *endtimetitle = "Enter end time.";
        const char *endtimelabel = "</U/5>end time:<!U!5>";
        std::string endtime = poptime(endtimetitle, endtimelabel, endconsttemp);


        std::string freq[] = {"DAILY", "WEEKLY", "MONTHLY", "YEARLY"};
        const char *repeatmessage[2];
        repeatmessage[0] = "<C></U>choose repeat kind";
        const char *repeatbuttons[] =
                {"</B/24>DAILY", "</B/24>WEEKLY", "</B/24>MONTHLY", "</B/24>YEARLY"};
        int repeatchoice = popupDialog(cdkscreen, (CDK_CSTRING2) repeatmessage, 1, (CDK_CSTRING2) repeatbuttons, 4);

        CDKSCALE *repeatscale = 0;
        const char *reptitle = "<C>Select repeat count";
        const char *replabel = "</5>repeat count";
        int repeatcount;
        repeatscale = newCDKScale(cdkscreen, CENTER, CENTER, reptitle, replabel, A_NORMAL, 5, editptr->RRULE.COUNT, 1,
                                  999, 1, 5, TRUE, FALSE);
        repeatcount = activateCDKScale(repeatscale, 0);
        destroyCDKScale(repeatscale);
        eraseCDKScreen(cdkscreen);
        refreshCDKScreen(cdkscreen);

        Caltime deletetime = editptr->DTSTART.STIME;
        for (int i = 0; i < editptr->RRULE.COUNT; ++i) {
            auto pair2 = vevent.ical.equal_range(deletetime);
            for (auto j = pair2.first; j != pair2.second; j++) {
                if (j->second == editptr) {
                    vevent.ical.erase(j);
                    break;
                }
            }
            deletetime.AddDay(editptr->RRULE.FREQ);
        }
        delete editptr;


        VeventRepeat *f_vec = new VeventRepeat;
        f_vec->SUMMARY = summary;
        f_vec->repeat = true;
        f_vec->SEQUENCE = 0;
        f_vec->STATUS = "CONFIRMED";
        f_vec->DESCRIPTION = description;
        f_vec->TRANSP = "OPAQUE";
        f_vec->RRULE.FREQ = freq[repeatchoice];
        f_vec->DTEND.TZID = "Asia/Shanghai";
        f_vec->DTSTART.TZID = "Asia/Shanghai";
        f_vec->LOCATION = location;
        char *charstarttime = new char[20];
        strcpy(charstarttime, starttime.c_str());
        f_vec->DTSTART.STIME.SetTime(charstarttime);
        char *charendtime = new char[20];
        strcpy(charendtime, starttime.c_str());
        f_vec->DTEND.STIME.SetTime(charendtime);
        std::stringstream ss2;
        ss2 << time(0) << '.' << "34214723058046557546435" << "@thucal";
        ss2 >> f_vec->UID;
        f_vec->RRULE.COUNT = repeatcount;
        Caltime itime = f_vec->DTSTART.STIME;
        for (int i = 0; i < f_vec->RRULE.COUNT; ++i) {
            vevent.ical.insert(std::pair<Caltime, Vevent *>(itime, f_vec));
            itime.AddDay(f_vec->RRULE.FREQ);
        }


    } else {
        VeventNoRepeat *editptr = (VeventNoRepeat *) edititem->second;
        char summtemp[256];
        strcpy(summtemp, editptr->SUMMARY.c_str());
        const char *summconsttemp = summtemp;
        const char *sumtitle = "Enter event summary.";
        const char *sumlabel = "</U/5>summary:<!U!5>";
        std::string summary = popentry(sumtitle, sumlabel, summconsttemp);


        char desctemp[256];
        strcpy(desctemp, editptr->DESCRIPTION.c_str());
        const char *descconsttemp = desctemp;
        const char *destitle = "Enter event description.";
        const char *deslabel = "</U/5>description:<!U!5>";
        std::string description = popentry(destitle, deslabel, descconsttemp);

        char locatemp[256];
        strcpy(locatemp, editptr->LOCATION.c_str());
        const char *locaconsttemp = locatemp;
        const char *loctitle = "Enter event location.";
        const char *loclabel = "</U/5>location:<!U!5>";
        std::string location = popentry(loctitle, loclabel, locaconsttemp);

        char starttemp[256];
        strcpy(starttemp, editptr->DTSTART.STIME.strintout2().c_str());
        const char *startconsttemp = starttemp;
        const char *startimetitle = "Enter start time.";
        const char *startimelabel = "</U/5>start time:<!U!5>";
        std::string starttime = poptime(startimetitle, startimelabel, startconsttemp);

        char endtemp[256];
        strcpy(endtemp, editptr->DTEND.STIME.strintout2().c_str());
        const char *endconsttemp = starttemp;
        const char *endtimetitle = "Enter end time.";
        const char *endtimelabel = "</U/5>end time:<!U!5>";
        std::string endtime = poptime(endtimetitle, endtimelabel, endconsttemp);


        Caltime deletetime = editptr->DTSTART.STIME;
        auto pair2 = vevent.ical.equal_range(deletetime);
        for (auto j = pair2.first; j != pair2.second; j++) {
            if (j->second == editptr) {
                vevent.ical.erase(j);
                break;
            }
        }
        delete editptr;


        VeventNoRepeat *f_vec = new VeventNoRepeat;
        f_vec->SUMMARY = summary;
        f_vec->repeat = false;
        f_vec->SEQUENCE = 0;
        f_vec->STATUS = "CONFIRMED";
        f_vec->DESCRIPTION = description;
        f_vec->TRANSP = "OPAQUE";
        f_vec->DTEND.VALUE = "Asia/Shanghai";
        f_vec->DTSTART.VALUE = "Asia/Shanghai";
        f_vec->LOCATION = location;
        char *charstarttime = new char[20];
        strcpy(charstarttime, starttime.c_str());
        f_vec->DTSTART.STIME.SetTime(charstarttime);
        char *charendtime = new char[20];
        strcpy(charendtime, starttime.c_str());
        f_vec->DTEND.STIME.SetTime(charendtime);
        std::stringstream ss2;
        ss2 << time(0) << '.' << "34214723058046557546435" << "@thucal";
        ss2 >> f_vec->UID;
        vevent.ical.insert(std::pair<Caltime, Vevent *>(f_vec->DTSTART.STIME, f_vec));


    }


    auto mbegin = vevent.ical.lower_bound(today);
    Caltime tomorrow(CalWidget->year, CalWidget->month, CalWidget->day, 23, 59, 59);
    auto mend = vevent.ical.upper_bound(tomorrow);
    int j = 0;
    for (auto it = mbegin; it != mend; ++it) {
        std::stringstream sstream;
        if (it->second->repeat) {
            auto *item = (VeventRepeat *) it->second;
            char summary[100];
            strcpy(summary, item->SUMMARY.c_str());
            sprintf(Veventlistitem[j], "%02d:%02d:%02d->%02d:%02d:%02d  %s", item->DTSTART.STIME.hour,
                    item->DTSTART.STIME.minute, item->DTSTART.STIME.second, item->DTEND.STIME.hour,
                    item->DTEND.STIME.minute, item->DTEND.STIME.second, summary);
        } else {
            auto *item = (VeventNoRepeat *) it->second;
            char summary[100];
            strcpy(summary, item->SUMMARY.c_str());
            sprintf(Veventlistitem[j], "%02d:%02d:%02d->%02d:%02d:%02d  %s", item->DTSTART.STIME.hour,
                    item->DTSTART.STIME.minute, item->DTSTART.STIME.second, item->DTEND.STIME.hour,
                    item->DTEND.STIME.minute, item->DTEND.STIME.second, summary);
        }
        j++;
    }
    eraseCDKScroll(Veventwidget);
    setCDKScroll(Veventwidget, (CDK_CSTRING2) Veventlistitem, j, FALSE, A_REVERSE, TRUE);
    drawCDKScroll(Veventwidget, TRUE);
    return (FALSE);


}


std::string poptime(const char *title, const char *label, const char *val) {
    char *charvalue;
    char temp[256];
    char *info;
    char *mixed;
    const char *Overlay = "____/__/__-__:__:__";
    const char *plate = "####/##/##-##:##:##";
    CDKTEMPLATE *cdktemplate = 0;

    cdktemplate = newCDKTemplate(cdkscreen, CENTER, CENTER, title, label, plate, Overlay, TRUE, FALSE);
    setCDKTemplate(cdktemplate, val, TRUE);
    info = activateCDKTemplate(cdktemplate, 0);
    std::string time = info;
    destroyCDKEntry(cdktemplate);
    eraseCDKScreen(cdkscreen);
    refreshCDKScreen(cdkscreen);
    return time;
}


static int popeventdetailCB(EObjectType cdktype GCC_UNUSED,
                            void *object,
                            void *clientData GCC_UNUSED,
                            chtype key GCC_UNUSED) {
    CDKSCROLL *veventwidget = (CDKSCROLL *) object;
    char **Veventlistitem = (char **) clientData;
    Caltime today(CalWidget->year, CalWidget->month, CalWidget->day, 0, 0, 0);

    auto pointevent = vevent.ical.lower_bound(today);
    for (int i = 0; i < veventwidget->currentItem; ++i) {
        pointevent++;
    }
    if (pointevent->second->repeat) {
        VeventRepeat *pointptr = (VeventRepeat *) pointevent->second;
        char **msg = new char *[10];
        for (int i = 0; i < 10; ++i) {
            msg[i] = new char[256];
        }
        sprintf(msg[0], "SUMMARY:%s", pointptr->SUMMARY.c_str());
        sprintf(msg[1], "DESCRIPTION:%s", pointptr->DESCRIPTION.c_str());
        sprintf(msg[2], "START TIME:%s", pointptr->DTSTART.STIME.strintout3().c_str());
        sprintf(msg[3], "END TIME:%s", pointptr->DTEND.STIME.strintout3().c_str());
        sprintf(msg[4], "LOCATION:%s", pointptr->LOCATION.c_str());
        sprintf(msg[5], "REPEAT KIND:%s", pointptr->RRULE.FREQ.c_str());
        sprintf(msg[6], "REPEAT COUNT:%d", pointptr->RRULE.COUNT);
        sprintf(msg[7], " ");
        sprintf(msg[8], "<C>Press any key to continue.");
        popupLabel(cdkscreen, msg, 9);

    } else {
        VeventNoRepeat *pointptr = (VeventNoRepeat *) pointevent->second;
        char **msg = new char *[10];
        for (int i = 0; i < 10; ++i) {
            msg[i] = new char[256];
        }
        sprintf(msg[0], "SUMMARY:%s", pointptr->SUMMARY.c_str());
        sprintf(msg[1], "DESCRIPTION:%s", pointptr->DESCRIPTION.c_str());
        sprintf(msg[2], "START TIME:%s", pointptr->DTSTART.STIME.strintout3().c_str());
        sprintf(msg[3], "END TIME:%s", pointptr->DTEND.STIME.strintout3().c_str());
        sprintf(msg[4], "LOCATION:%s", pointptr->LOCATION.c_str());
        sprintf(msg[5], " ");
        sprintf(msg[6], "<C>Press any key to continue.");
        popupLabel(cdkscreen, msg, 7);
    }
}


static int ImportIcsCB(EObjectType cdktype GCC_UNUSED,
                       void *object,
                       void *clientData GCC_UNUSED,
                       chtype key GCC_UNUSED) {
    char **Veventlistitem = (char **) clientData;
    char temp[256];
    getcwd(temp, MAXBUFSIZE);
    const char *consttemp = temp;
    const char *sumtitle = "Enter ICS file directory.";
    const char *sumlabel = "</U/5>directory:<!U!5>";
    std::string directory = popentry(sumtitle, sumlabel, consttemp);
    vevent.ImportIcs(directory);


    Caltime today(CalWidget->year, CalWidget->month, CalWidget->day, 0, 0, 0);
    auto mbegin = vevent.ical.lower_bound(today);
    Caltime tomorrow(CalWidget->year, CalWidget->month, CalWidget->day, 23, 59, 59);
    auto mend = vevent.ical.upper_bound(tomorrow);
    int j = 0;
    for (auto it = mbegin; it != mend; ++it) {
        std::stringstream sstream;
        if (it->second->repeat) {
            auto *item = (VeventRepeat *) it->second;
            char summary[100];
            strcpy(summary, item->SUMMARY.c_str());
            sprintf(Veventlistitem[j], "%02d:%02d:%02d->%02d:%02d:%02d  %s", item->DTSTART.STIME.hour,
                    item->DTSTART.STIME.minute, item->DTSTART.STIME.second, item->DTEND.STIME.hour,
                    item->DTEND.STIME.minute, item->DTEND.STIME.second, summary);
        } else {
            auto *item = (VeventNoRepeat *) it->second;
            char summary[100];
            strcpy(summary, item->SUMMARY.c_str());
            sprintf(Veventlistitem[j], "%02d:%02d:%02d->%02d:%02d:%02d  %s", item->DTSTART.STIME.hour,
                    item->DTSTART.STIME.minute, item->DTSTART.STIME.second, item->DTEND.STIME.hour,
                    item->DTEND.STIME.minute, item->DTEND.STIME.second, summary);
        }
        j++;
    }


    setCDKScroll(Veventwidget, (CDK_CSTRING2) Veventlistitem, j, FALSE, A_REVERSE, TRUE);
    eraseCDKScroll(Veventwidget);
    drawCDKScroll(Veventwidget, TRUE);


}

static int ImportXlsCB(EObjectType cdktype GCC_UNUSED,
                       void *object,
                       void *clientData GCC_UNUSED,
                       chtype key GCC_UNUSED) {
    char **Veventlistitem = (char **) clientData;
    char temp[256];
    getcwd(temp, MAXBUFSIZE);
    const char *consttemp = temp;
    const char *sumtitle = "Enter XLS file directory.";
    const char *sumlabel = "</U/5>directory:<!U!5>";
    std::string directory = popentry(sumtitle, sumlabel, consttemp);

    const char *timetitle = "Enter begin day.";
    const char *timelabel = "</U/5>begin day:<!U!5>";
    std::string stime = popdate(timetitle, timelabel);
    int year, month, day;
    char sdate[20];
    strcpy(sdate, stime.c_str());
    sscanf(sdate, "%d/%d/%d", &year, &month, &day);
    vevent.ImportExcel(directory, year, month, day);

    Caltime today(CalWidget->year, CalWidget->month, CalWidget->day, 0, 0, 0);
    auto mbegin = vevent.ical.lower_bound(today);
    Caltime tomorrow(CalWidget->year, CalWidget->month, CalWidget->day, 23, 59, 59);
    auto mend = vevent.ical.upper_bound(tomorrow);
    int j = 0;
    for (auto it = mbegin; it != mend; ++it) {
        std::stringstream sstream;
        if (it->second->repeat) {
            auto *item = (VeventRepeat *) it->second;
            char summary[100];
            strcpy(summary, item->SUMMARY.c_str());
            sprintf(Veventlistitem[j], "%02d:%02d:%02d->%02d:%02d:%02d  %s", item->DTSTART.STIME.hour,
                    item->DTSTART.STIME.minute, item->DTSTART.STIME.second, item->DTEND.STIME.hour,
                    item->DTEND.STIME.minute, item->DTEND.STIME.second, summary);
        } else {
            auto *item = (VeventNoRepeat *) it->second;
            char summary[100];
            strcpy(summary, item->SUMMARY.c_str());
            sprintf(Veventlistitem[j], "%02d:%02d:%02d->%02d:%02d:%02d  %s", item->DTSTART.STIME.hour,
                    item->DTSTART.STIME.minute, item->DTSTART.STIME.second, item->DTEND.STIME.hour,
                    item->DTEND.STIME.minute, item->DTEND.STIME.second, summary);
        }
        j++;
    }


    setCDKScroll(Veventwidget, (CDK_CSTRING2) Veventlistitem, j, FALSE, A_REVERSE, TRUE);
    eraseCDKScroll(Veventwidget);
    drawCDKScroll(Veventwidget, TRUE);


}

static int OutputIcsCB(EObjectType cdktype GCC_UNUSED,
                       void *object,
                       void *clientData GCC_UNUSED,
                       chtype key GCC_UNUSED) {
    char temp[256];
    char name[] = "/default.ics";
    getcwd(temp, MAXBUFSIZE);
    strcat(temp, name);
    const char *consttemp = temp;
    const char *sumtitle = "Enter ICS file directory.";
    const char *sumlabel = "</U/5>directory:<!U!5>";
    std::string directory = popentry(sumtitle, sumlabel, consttemp);
    vevent.OutputIcs(directory);
}


static int CalHelpCB(EObjectType cdktype GCC_UNUSED,
                     void *object,
                     void *clientData GCC_UNUSED,
                     chtype key GCC_UNUSED) {
    char **msg = new char *[19];
    for (int i = 0; i < 19; ++i) {
        msg[i] = new char[256];
    }
    sprintf(msg[0], "Left Arrow       Moves the cursor to the previous day.");
    sprintf(msg[1], "Right Arrow      Moves the cursor to the next day.");
    sprintf(msg[2], "Up Arrow         Moves the cursor to the next week.");
    sprintf(msg[3], "Down Arrow       Moves the cursor to the previous week.");
    sprintf(msg[4], "n                Advances the calendar one month ahead.");
    sprintf(msg[5], "N                Advances the calendar six months ahead.");
    sprintf(msg[6], "p                Advances the calendar one month back.");
    sprintf(msg[7], "P                Advances the calendar six months back.");
    sprintf(msg[8], "-                Advances the calendar one year ahead.");
    sprintf(msg[9], "+                Advances the calendar one year back.");
    sprintf(msg[10], "i                Import .ics file.");
    sprintf(msg[11], "o                Output .ics file.");
    sprintf(msg[12], "x                Import .xls file.");
    sprintf(msg[13], "h                Show calendar widget help.");
    sprintf(msg[14], "Enter            Select correct date.");
    sprintf(msg[15], "Tab              Toggle widgets.");
    sprintf(msg[16], "Ctrl+x           Quit.");
    sprintf(msg[17], " ");
    sprintf(msg[18], "<C>Press any key to continue.");
    popupLabel(cdkscreen, msg, 19);
}


static int VevnetHelpCB(EObjectType cdktype GCC_UNUSED,
                        void *object,
                        void *clientData GCC_UNUSED,
                        chtype key GCC_UNUSED) {
    char **msg = new char *[18];
    for (int i = 0; i < 17; ++i) {
        msg[i] = new char[256];
    }
    sprintf(msg[0], "Left Arrow       Shift the list left one column.");
    sprintf(msg[1], "Right Arrow      Shift the list right one column.");
    sprintf(msg[2], "Up Arrow         Select the previous item in the list.");
    sprintf(msg[3], "Down Arrow       Select the next item in the list.");
    sprintf(msg[4], "g                Move to the first element in the list.");
    sprintf(msg[5], "G                Move to the last element in the list.");
    sprintf(msg[6], "a                Add new item in the list.");
    sprintf(msg[7], "d                Delete correct item in the list.");
    sprintf(msg[8], "e                Edit correct item in the list.");
    sprintf(msg[9], "i                Import .ics file.");
    sprintf(msg[10], "o                Output .ics file.");
    sprintf(msg[11], "x                Import .xls file.");
    sprintf(msg[12], "h                Show Appointments widget help.");
    sprintf(msg[13], "Enter            Show detail of correct item in the list.");
    sprintf(msg[14], "Tab              Toggle widgets.");
    sprintf(msg[15], "Ctrl+x           Quit.");
    sprintf(msg[16], " ");
    sprintf(msg[17], "<C>Press any key to continue.");
    popupLabel(cdkscreen, msg, 18);
}


static int TodolisthelpCB(EObjectType cdktype GCC_UNUSED,
                          void *object,
                          void *clientData GCC_UNUSED,
                          chtype key GCC_UNUSED) {
    char **msg = new char *[7];
    for (int i = 0; i < 16; ++i) {
        msg[i] = new char[256];
    }
    sprintf(msg[0], "Left Arrow       Shift the list left one column.");
    sprintf(msg[1], "Right Arrow      Shift the list right one column.");
    sprintf(msg[2], "Up Arrow         Select the previous item in the list.");
    sprintf(msg[3], "Down Arrow       Select the next item in the list.");
    sprintf(msg[4], "g                Move to the first element in the list.");
    sprintf(msg[5], "G                Move to the last element in the list.");
    sprintf(msg[6], "a                Add new item in the list.");
    sprintf(msg[7], "d                Delete correct item in the list.");
    sprintf(msg[8], "e                Edit correct item in the list.");
    sprintf(msg[9], "i                Import .ics file.");
    sprintf(msg[10], "o                Output .ics file.");
    sprintf(msg[11], "x                Import .xls file.");
    sprintf(msg[12], "h                Show Todolist widget help.");
    sprintf(msg[13], "Tab              Toggle widgets.");
    sprintf(msg[14], "Ctrl+x           Quit.");
    sprintf(msg[15], " ");
    sprintf(msg[16], "<C>Press any key to continue.");
    popupLabel(cdkscreen, msg, 17);
}

std::string popdate(const char *title, const char *label) {
    char *charvalue;
    char temp[256];
    char *info;
    char *mixed;
    const char *Overlay = "____/__/__";
    const char *plate = "####/##/##";
    CDKTEMPLATE *cdktemplate = 0;

    cdktemplate = newCDKTemplate(cdkscreen, CENTER, CENTER, title, label, plate, Overlay, TRUE, FALSE);
    info = activateCDKTemplate(cdktemplate, 0);
    mixed = mixCDKTemplate(cdktemplate);
    std::string time = mixed;
    destroyCDKEntry(cdktemplate);
    eraseCDKScreen(cdkscreen);
    refreshCDKScreen(cdkscreen);
    return time;
}