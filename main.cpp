#include <iostream>
#include <cursesw.h>
#include <unistd.h>
#include <menu.h>
#include <cdk.h>
#include <ctime>
#include <string>
#include "Todolist.h"
#include "Cal.h"
#include <locale.h>

static CDKOBJS *all_objects[3];
using namespace std;
#define NumElements(a) ((sizeof a)/(sizeof a[0]))
#define VECTORLENGTH 23
#define VECTORDEFAULTSIZE 100
#define VEVENTLENGTH 55
Cal vevent;

//char todolistitem[VECTORDEFAULTSIZE][VECTORLENGTH];
//char Veventlistitem[VECTORDEFAULTSIZE][VEVENTLENGTH];
static int dialogHelpCB(EObjectType cdktype GCC_UNUSED,
                        void *object,
                        void *clientData GCC_UNUSED,
                        chtype key GCC_UNUSED);

static void rebind_esc(CDKOBJS *obj) {
    bindCDKObject(ObjTypeOf (obj), obj, KEY_F (1), getcCDKBind, (void *) KEY_ESC);
}

CDKSCREEN *cdkscreen = NULL;
CDKSCROLL *Todolistwidget;
CDKSCROLL *Veventwidget;

int main(int argc, char const *argv[]) {

    vevent.ImportIcs("./thucal-2018-2019-秋.ics");
    int CalWinstartx, CalWinstarty, CalWinwidth, CalWinheight;
    int TodoWinstartx, TodoWinstarty, TodoWinwidth, TodoWinheight;
    int VeventWinstartx, VeventWinstarty, VeventWinwidth, VeventWinheight;
    int HelpWinstartx, HelpWinstarty, HelpWinwidth, HelpWinheight;
    CalWinheight = 12;
    CalWinwidth = 24;
    CalWinstarty = 0;
    CalWinstartx = (COLS - CalWinwidth);

    TodoWinheight = LINES - CalWinheight - 3;
    TodoWinwidth = CalWinwidth - 1;
    TodoWinstarty = CalWinheight;
    TodoWinstartx = (COLS - TodoWinwidth);

    VeventWinheight = LINES - 3;
    VeventWinwidth = COLS - CalWinwidth - 1;
    VeventWinstarty = 0;
    VeventWinstartx = 0;

    HelpWinstartx = 0;
    HelpWinstarty = LINES - 3;
    HelpWinwidth = COLS;
    HelpWinheight = 3;

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

    Todolist todolist;
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

    CDKCALENDAR *CalWidget = newCDKCalendar(cdkscreen, RIGHT, CalWinstarty,
                                            "Calendar", day, month, year,
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
                                "Appointments",
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
                                  "Todolist",
                                  (CDK_CSTRING2) todolistitem, i,
                                  FALSE,
                                  A_REVERSE,
                                  TRUE,
                                  FALSE);
    all_objects[2] = ObjPtr(Todolistwidget);
    bindCDKObject(vCALENDAR, CalWidget, KEY_ENTER, dialogHelpCB, Veventlistitem);

    refreshCDKScreen(cdkscreen);
    traverseCDKScreen(cdkscreen);
    mesg[0] = "Done";
    mesg[1] = "";
    mesg[2] = "<C>Press any key to continue.";
    popupLabel(cdkscreen, (CDK_CSTRING2) mesg, 3);
    _destroyCDKObject(all_objects[2]);
    destroyCDKScreen(cdkscreen);
    endCDK();
    return 0;
}


static int dialogHelpCB(EObjectType cdktype GCC_UNUSED,
                        void *object,
                        void *clientData GCC_UNUSED,
                        chtype key GCC_UNUSED) {
    eraseCDKScroll(Veventwidget);
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
    setCDKScroll(Veventwidget, (CDK_CSTRING2) Veventlistitem, j, FALSE, A_REVERSE, TRUE);
    drawCDKScroll(Veventwidget, TRUE);
    return (FALSE);
}