#include <iostream>
#include <curses.h>
#include <unistd.h>
#include <cdk.h>
#include <ctime>
#include <string>
#include "Todolist.h"
#include "Cal.h"
#include "VeventRepeat.h"
#include "VeventNoRepeat.h"

using namespace std;

#define MAXBUFSIZE 1024
#define VECTORLENGTH 256
#define VECTORDEFAULTSIZE 100
#define VEVENTLENGTH 256

/**
 * 将vevent显示的日程改为选中日期的日程
 */
static int changedayCB(EObjectType cdktype GCC_UNUSED,
                       void *object,
                       void *clientData GCC_UNUSED,
                       chtype key GCC_UNUSED);

/**
 * 增加todolist项目并刷新
 */
static int addtodolistCB(EObjectType cdktype GCC_UNUSED,
                         void *object,
                         void *clientData GCC_UNUSED,
                         chtype key GCC_UNUSED);

/**
 * 增加vevnet项目并刷新
 */
static int addVeventCB(EObjectType cdktype GCC_UNUSED,
                       void *object,
                       void *clientData GCC_UNUSED,
                       chtype key GCC_UNUSED);

/**
 * 删除选中的vevnet项目并刷新
 */
static int deleteVeventCB(EObjectType cdktype GCC_UNUSED,
                          void *object,
                          void *clientData GCC_UNUSED,
                          chtype key GCC_UNUSED);

/**
 * 删除选中的todolsit项目并刷新
 */
static int deletetodolistCB(EObjectType cdktype GCC_UNUSED,
                            void *object,
                            void *clientData GCC_UNUSED,
                            chtype key GCC_UNUSED);

/**
 * 编辑选中的todolsit项目并刷新
 */
static int edittodolistCB(EObjectType cdktype GCC_UNUSED,
                          void *object,
                          void *clientData GCC_UNUSED,
                          chtype key GCC_UNUSED);

/**
 * 编辑选中的vevnet项目并刷新
 */
static int editVeventCB(EObjectType cdktype GCC_UNUSED,
                        void *object,
                        void *clientData GCC_UNUSED,
                        chtype key GCC_UNUSED);

/**
 * 显示选中的vevnet项目的详细信息
 */
static int popeventdetailCB(EObjectType cdktype GCC_UNUSED,
                            void *object,
                            void *clientData GCC_UNUSED,
                            chtype key GCC_UNUSED);

/**
 * 导入ics文件
 */
static int ImportIcsCB(EObjectType cdktype GCC_UNUSED,
                       void *object,
                       void *clientData GCC_UNUSED,
                       chtype key GCC_UNUSED);

/**
 * 导入xls文件
 */
static int ImportXlsCB(EObjectType cdktype GCC_UNUSED,
                       void *object,
                       void *clientData GCC_UNUSED,
                       chtype key GCC_UNUSED);

/**
 * 导出ics文件
 */
static int OutputIcsCB(EObjectType cdktype GCC_UNUSED,
                       void *object,
                       void *clientData GCC_UNUSED,
                       chtype key GCC_UNUSED);

/**
 * 显示日历帮助
 */
static int CalHelpCB(EObjectType cdktype GCC_UNUSED,
                     void *object,
                     void *clientData GCC_UNUSED,
                     chtype key GCC_UNUSED);

/**
 * 显示Vevnet帮助
 */
static int VevnetHelpCB(EObjectType cdktype GCC_UNUSED,
                        void *object,
                        void *clientData GCC_UNUSED,
                        chtype key GCC_UNUSED);

/**
 * 显示Todolist帮助
 */
static int TodolisthelpCB(EObjectType cdktype GCC_UNUSED,
                          void *object,
                          void *clientData GCC_UNUSED,
                          chtype key GCC_UNUSED);

/**
 * 弹出输入框，输入字符串数据
 * @param  title 标题
 * @param  label 标签
 * @return       输入的数据
 */
std::string popentry(const char *title, const char *label);

/**
 * 弹出输入框，输入时间数据
 * @param  title 标题
 * @param  label 标签
 * @return       输入的时间
 */
std::string poptime(const char *title, const char *label);

/**
 * 弹出输入框，输入字符串数据
 * @param  title 标题
 * @param  label 标签
 * @param  val   需要填充进输入框的数据
 * @return       输入的数据
 */
std::string popentry(const char *title, const char *label, const char *val);

/**
 * 弹出输入框，输入时间数据
 * @param  title 标题
 * @param  label 标签
 * @param  val   需要填充进输入框的数据
 * @return       输入的数据
 */
std::string poptime(const char *title, const char *label, const char *val);

/**
 * 弹出输入框，输入日期数据
 * @param  title 标题
 * @param  label 标签
 * @return       输入的数据
 */
std::string popdate(const char *title, const char *label);

/**
 * 弹出输入框，选择一个1-9的数
 * @param  title 标题
 * @param  label 标签
 * @param  max   最大值
 * @return       选择的数字
 */
int popscale(const char *title, const char *label, int max = 9);

/**
 * 判断是否为闰年
 * @param  year 年份
 * @return      是否为闰年
 */
bool IsLeapYear(int year);

/**
 * 判断时间是否合法
 * @param  time 时间
 * @return      是否合法
 */
bool IsLegal(char *time);

/**
 * 更新vevent窗口的数据
 * @param  year           年份
 * @param  month          月份
 * @param  day            日期
 * @param  Veventlistitem 数据列表
 * @return                数据的个数
 */
int UpdateVeventItem(int year, int month, int day, char *const *Veventlistitem);

/**
 * 刷新vevent列表
 * @param Veventlistitem 数据列表
 * @param calendar       日历小部件的指针
 */
void RefreshVevent(char *const *Veventlistitem, const CDKCALENDAR *calendar);

/**
 * 刷新todolist列表
 * @param todolistitem todolist小部件的指针
 */
void RefreshTodolist(char *const *todolistitem);

/**
 * 删除vevnet数据
 * @param deleteitem 删除的条目
 */
void DeleteVeventItem(
        const multimap<Caltime, Vevent *, std::less<Caltime>, std::allocator<std::pair<Caltime, Vevent *>>>::iterator &deleteitem);

Cal vevent;
Todolist todolist;
CDKSCREEN *cdkscreen = NULL;
CDKSCROLL *Todolistwidget;
CDKSCROLL *Veventwidget;
CDKCALENDAR *CalWidget;


int main(int argc, char const *argv[]) {
    //导入数据库
    vevent.ImportIcs("database.ics");
    todolist.ImportTodo("todolist.txt");

    //设置窗口大小
    int CalWinstartx, CalWinstarty, CalWinwidth, CalWinheight;
    int TodoWinstartx, TodoWinstarty, TodoWinwidth, TodoWinheight;
    int VeventWinstartx, VeventWinstarty, VeventWinwidth, VeventWinheight;

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

    //获取时间
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

    //新建todoilist和vevent列表
    char **todolistitem = new char *[VECTORDEFAULTSIZE];
    for (int i = 0; i < VECTORDEFAULTSIZE; ++i) {
        todolistitem[i] = new char[VECTORLENGTH];
    }

    char **Veventlistitem = new char *[VECTORDEFAULTSIZE];
    for (int i = 0; i < VECTORDEFAULTSIZE; ++i) {
        Veventlistitem[i] = new char[VEVENTLENGTH];
    }

    //初始化屏幕
    cdkscreen = initCDKScreen(NULL);
    initCDKColor();
    //提取todolist条目
    int i = 0;
    for (auto &it:todolist.v) {
        std::stringstream sstream;
        sstream << it.first << " " << it.second << std::endl;
        sstream.getline(todolistitem[i], VECTORLENGTH);
        i++;
    }
    //没有条目的补救
    if (i == 0) {
        sprintf(todolistitem[0], " ");
        i = 1;
    }
    //提取vevent条目
    int j = UpdateVeventItem(day, month, year, Veventlistitem);
    //没有条目的补救
    if (j == 0) {
        sprintf(Veventlistitem[0], " ");
        j = 1;
    }
    //新建日历小部件
    CalWidget = newCDKCalendar(cdkscreen, RIGHT, CalWinstarty,
                               "<C>Calendar", day, month, year,
                               COLOR_PAIR (16) | A_BOLD,
                               COLOR_PAIR (24) | A_BOLD,
                               COLOR_PAIR (32) | A_BOLD,
                               COLOR_PAIR (40) | A_REVERSE,
                               TRUE,
                               FALSE);
    //新建vevent小部件
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
    //新建todolist小部件
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
    //绑定按键
    bindCDKObject(vSCROLL, Todolistwidget, 'a', addtodolistCB, todolistitem);
    bindCDKObject(vSCROLL, Veventwidget, 'a', addVeventCB, Veventlistitem);

    bindCDKObject(vSCROLL, Veventwidget, 'd', deleteVeventCB, Veventlistitem);
    bindCDKObject(vSCROLL, Todolistwidget, 'd', deletetodolistCB, todolistitem);

    bindCDKObject(vSCROLL, Todolistwidget, 'e', edittodolistCB, todolistitem);
    bindCDKObject(vSCROLL, Veventwidget, 'e', editVeventCB, Veventlistitem);

    bindCDKObject(vCALENDAR, CalWidget, KEY_ENTER, changedayCB, Veventlistitem);
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

    //开始的提示信息
    const char *mesg[3];
    mesg[0] = "Press h for help";
    mesg[1] = "";
    mesg[2] = "<C>Press any key to continue.";
    popupLabel(cdkscreen, (CDK_CSTRING2) mesg, 3);
    refreshCDKScreen(cdkscreen);

    //刷新屏幕，把控制权交给用户
    RefreshVevent(Veventlistitem, CalWidget);
    traverseCDKScreen(cdkscreen);

    //结束信息
    mesg[0] = "Done";
    mesg[1] = "";
    mesg[2] = "<C>Press any key to continue.";
    popupLabel(cdkscreen, (CDK_CSTRING2) mesg, 3);

    //结束处理
    destroyCDKScroll(Veventwidget);
    destroyCDKScroll(Todolistwidget);
    destroyCDKCalendar(CalWidget);
    destroyCDKScreen(cdkscreen);
    endCDK();
    //导出数据库
    todolist.OutputTodo("todolist.txt");
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
    RefreshVevent(Veventlistitem, calendar);
    return (FALSE);
}


static int addtodolistCB(EObjectType cdktype GCC_UNUSED,
                         void *object,
                         void *clientData GCC_UNUSED,
                         chtype key GCC_UNUSED) {
    CDKSCROLL *todolistwidget = (CDKSCROLL *) object;
    char **todolistitem = (char **) clientData;
    const char *sumtitle = "Enter todo things.";
    const char *sumlabel = "</U/5>summary:<!U!5>";
    std::string summary = popentry(sumtitle, sumlabel);

    const char *imptitle = "<C>Select a value";
    const char *implabel = "</5>Current value";
    int iimport = popscale(imptitle, implabel);
    todolist.InsertTodo(iimport, summary);

    RefreshTodolist(todolistitem);
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
            {"No", "Yes"};
    int choice = popupDialog(cdkscreen, (CDK_CSTRING2) message, 1, (CDK_CSTRING2) buttons, 2);

    if (choice == 1) {

        std::string freq[] = {"DAILY", "WEEKLY", "MONTHLY", "YEARLY"};
        const char *repeatmessage[2];
        repeatmessage[0] = "<C></U>choose repeat kind";
        const char *repeatbuttons[] =
                {"DAILY", "WEEKLY", "MONTHLY", "YEARLY"};
        int repeatchoice = popupDialog(cdkscreen, (CDK_CSTRING2) repeatmessage, 1, (CDK_CSTRING2) repeatbuttons, 4);


        const char *reptitle = "<C>Select repeat count";
        const char *replabel = "</5>repeat count";
        int repeatcount = popscale(reptitle, replabel, 999);

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
        strcpy(charendtime, endtime.c_str());
        f_vec->DTEND.STIME.SetTime(charendtime);
        std::stringstream ss2;
        ss2 << time(0) << '.' << "34214723058046557546435" << "@thucal";
        ss2 >> f_vec->UID;
        f_vec->RRULE.COUNT = repeatcount;
        Caltime itime = f_vec->DTSTART.STIME;
        for (int i = 0; i < f_vec->RRULE.COUNT; ++i) {
            vevent.InsertItem(std::pair<Caltime, Vevent *>(itime, f_vec));
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
        strcpy(charendtime, endtime.c_str());
        f_vec->DTEND.STIME.SetTime(charendtime);
        std::stringstream ss2;
        ss2 << time(0) << '.' << "34214723058046557546435" << "@thucal";
        ss2 >> f_vec->UID;
        vevent.InsertItem(std::pair<Caltime, Vevent *>(f_vec->DTSTART.STIME, f_vec));
    }

    RefreshVevent(Veventlistitem, CalWidget);
    return (FALSE);
}


static int deleteVeventCB(EObjectType cdktype GCC_UNUSED,
                          void *object,
                          void *clientData GCC_UNUSED,
                          chtype key GCC_UNUSED) {

    CDKSCROLL *veventwidget = (CDKSCROLL *) object;
    char **Veventlistitem = (char **) clientData;

    Caltime today(CalWidget->year, CalWidget->month, CalWidget->day, 0, 0, 0);
    auto deleteitem = vevent.FindLowerBound(today);
    for (int i = 0; i < veventwidget->currentItem; ++i) {
        deleteitem++;
    }
    const char *message[2];
    message[0] = "<C></U>really to delete?";
    const char *buttons[] =
            {"No", "Yes"};
    int choice = popupDialog(cdkscreen, (CDK_CSTRING2) message, 1, (CDK_CSTRING2) buttons, 2);
    if (choice == 1) {
        DeleteVeventItem(deleteitem);
    }

    RefreshVevent(Veventlistitem, CalWidget);
    return (FALSE);

}

static int deletetodolistCB(EObjectType cdktype GCC_UNUSED,
                            void *object,
                            void *clientData GCC_UNUSED,
                            chtype key GCC_UNUSED) {

    CDKSCROLL *todolistwidget = (CDKSCROLL *) object;
    char **todolistitem = (char **) clientData;

    auto deleteitem = todolist.v.begin();
    if (todolist.v.empty())
        return (FALSE);
    for (int i = 0; i < todolistwidget->currentItem; ++i) {
        deleteitem++;
    }
    const char *message[2];
    message[0] = "<C></U>really to delete?";
    const char *buttons[] =
            {"No", "Yes"};
    int choice = popupDialog(cdkscreen, (CDK_CSTRING2) message, 1, (CDK_CSTRING2) buttons, 2);
    if (choice == 1) {
        todolist.v.erase(deleteitem);
    }

    RefreshTodolist(todolistitem);
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

    RefreshTodolist(todolistitem);
    return (FALSE);
}

static int editVeventCB(EObjectType cdktype GCC_UNUSED,
                        void *object,
                        void *clientData GCC_UNUSED,
                        chtype key GCC_UNUSED) {

    CDKSCROLL *veventwidget = (CDKSCROLL *) object;
    char **Veventlistitem = (char **) clientData;
    Caltime today(CalWidget->year, CalWidget->month, CalWidget->day, 0, 0, 0);

    auto edititem = vevent.FindLowerBound(today);
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
        strcpy(starttemp, editptr->DTSTART.STIME.stringout(14).c_str());
        const char *startconsttemp = starttemp;
        const char *startimetitle = "Enter start time.";
        const char *startimelabel = "</U/5>start time:<!U!5>";
        std::string starttime = poptime(startimetitle, startimelabel, startconsttemp);

        char endtemp[256];
        strcpy(endtemp, editptr->DTEND.STIME.stringout(14).c_str());
        const char *endconsttemp = endtemp;
        const char *endtimetitle = "Enter end time.";
        const char *endtimelabel = "</U/5>end time:<!U!5>";
        std::string endtime = poptime(endtimetitle, endtimelabel, endconsttemp);


        std::string freq[] = {"DAILY", "WEEKLY", "MONTHLY", "YEARLY"};
        const char *repeatmessage[2];
        repeatmessage[0] = "<C></U>choose repeat kind";
        const char *repeatbuttons[] =
                {"DAILY", "WEEKLY", "MONTHLY", "YEARLY"};
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
            auto pair2 = vevent.FindEqualRange(deletetime);
            for (auto j = pair2.first; j != pair2.second; j++) {
                if (j->second == editptr) {
                    vevent.Erase(j);
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
        strcpy(charendtime, endtime.c_str());
        f_vec->DTEND.STIME.SetTime(charendtime);
        std::stringstream ss2;
        ss2 << time(0) << '.' << "34214723058046557546435" << "@thucal";
        ss2 >> f_vec->UID;
        f_vec->RRULE.COUNT = repeatcount;
        Caltime itime = f_vec->DTSTART.STIME;
        for (int i = 0; i < f_vec->RRULE.COUNT; ++i) {
            vevent.InsertItem(std::pair<Caltime, Vevent *>(itime, f_vec));
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
        strcpy(starttemp, editptr->DTSTART.STIME.stringout(14).c_str());
        const char *startconsttemp = starttemp;
        const char *startimetitle = "Enter start time.";
        const char *startimelabel = "</U/5>start time:<!U!5>";
        std::string starttime = poptime(startimetitle, startimelabel, startconsttemp);

        char endtemp[256];
        strcpy(endtemp, editptr->DTEND.STIME.stringout(14).c_str());
        const char *endconsttemp = starttemp;
        const char *endtimetitle = "Enter end time.";
        const char *endtimelabel = "</U/5>end time:<!U!5>";
        std::string endtime = poptime(endtimetitle, endtimelabel, endconsttemp);


        Caltime deletetime = editptr->DTSTART.STIME;
        auto pair2 = vevent.FindEqualRange(deletetime);
        for (auto j = pair2.first; j != pair2.second; j++) {
            if (j->second == editptr) {
                vevent.Erase(j);
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
        strcpy(charendtime, endtime.c_str());
        f_vec->DTEND.STIME.SetTime(charendtime);
        std::stringstream ss2;
        ss2 << time(0) << '.' << "34214723058046557546435" << "@thucal";
        ss2 >> f_vec->UID;
        vevent.InsertItem(std::pair<Caltime, Vevent *>(f_vec->DTSTART.STIME, f_vec));


    }

    RefreshVevent(Veventlistitem, CalWidget);


    return (FALSE);


}

static int popeventdetailCB(EObjectType cdktype GCC_UNUSED,
                            void *object,
                            void *clientData GCC_UNUSED,
                            chtype key GCC_UNUSED) {
    CDKSCROLL *veventwidget = (CDKSCROLL *) object;
    char **Veventlistitem = (char **) clientData;
    Caltime today(CalWidget->year, CalWidget->month, CalWidget->day, 0, 0, 0);

    auto pointevent = vevent.FindLowerBound(today);
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
        sprintf(msg[2], "START TIME:%s", pointptr->DTSTART.STIME.stringout(19).c_str());
        sprintf(msg[3], "END TIME:%s", pointptr->DTEND.STIME.stringout(19).c_str());
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
        sprintf(msg[2], "START TIME:%s", pointptr->DTSTART.STIME.stringout(19).c_str());
        sprintf(msg[3], "END TIME:%s", pointptr->DTEND.STIME.stringout(19).c_str());
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


    RefreshVevent(Veventlistitem, CalWidget);


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

    RefreshVevent(Veventlistitem, CalWidget);


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
    for (int i = 0; i < 18; ++i) {
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
    char **msg = new char *[17];
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
    sprintf(msg[12], "h                Show Todolist widget help.");
    sprintf(msg[13], "Tab              Toggle widgets.");
    sprintf(msg[14], "Ctrl+x           Quit.");
    sprintf(msg[15], " ");
    sprintf(msg[16], "<C>Press any key to continue.");
    popupLabel(cdkscreen, msg, 17);
}

int UpdateVeventItem(int year, int month, int day, char *const *Veventlistitem) {
    Caltime today(year, month, day, 0, 0, 0);

    auto mbegin = vevent.FindLowerBound(today);
    Caltime tomorrow(year, month, day, 23, 59, 59);
    auto mend = vevent.FindUpperBound(tomorrow);
    int j = 0;
    for (auto it = mbegin; it != mend; ++it) {
        stringstream sstream;
        if (it->second->repeat) {
            auto *item = (VeventRepeat *) it->second;
            char summary[100];
            strcpy(summary, item->SUMMARY.c_str());
            sprintf(Veventlistitem[j], "%02d:%02d:%02d->%02d:%02d:%02d  %s", item->DTSTART.STIME.GetHour(),
                    item->DTSTART.STIME.GetMinute(), item->DTSTART.STIME.GetSecond(), item->DTEND.STIME.GetHour(),
                    item->DTEND.STIME.GetMinute(), item->DTEND.STIME.GetSecond(), summary);
        } else {
            auto *item = (VeventNoRepeat *) it->second;
            char summary[100];
            strcpy(summary, item->SUMMARY.c_str());
            sprintf(Veventlistitem[j], "%02d:%02d:%02d->%02d:%02d:%02d  %s", item->DTEND.STIME.GetHour(),
                    item->DTEND.STIME.GetMinute(), item->DTEND.STIME.GetSecond(), item->DTEND.STIME.GetHour(),
                    item->DTEND.STIME.GetMinute(), item->DTEND.STIME.GetSecond(), summary);
        }
        j++;
    }
    return j;
}

void DeleteVeventItem(
        const multimap<Caltime, Vevent *, std::less<Caltime>, std::allocator<std::pair<Caltime, Vevent *>>>::iterator &deleteitem) {
    if (deleteitem->second->repeat) {
        VeventRepeat *deleterepeat = (VeventRepeat *) deleteitem->second;
        Caltime deletetime = deleterepeat->DTSTART.STIME;
        for (int i = 0; i < deleterepeat->RRULE.COUNT; ++i) {
            auto pair2 = vevent.FindEqualRange(deletetime);
            for (auto j = pair2.first; j != pair2.second; j++) {
                if (j->second == deleterepeat) {
                    vevent.Erase(j);
                    break;
                }
            }
            deletetime.AddDay(deleterepeat->RRULE.FREQ);
        }
        delete deleterepeat;
    } else {
        VeventNoRepeat *deletenorepeat = (VeventNoRepeat *) deleteitem->second;
        Caltime deletetime = deletenorepeat->DTSTART.STIME;
        auto pair2 = vevent.FindEqualRange(deletetime);
        for (auto j = pair2.first; j != pair2.second; j++) {
            if (j->second == deletenorepeat) {
                vevent.Erase(j);
                break;
            }
        }
        delete deletenorepeat;
    }
}


void RefreshVevent(char *const *Veventlistitem, const CDKCALENDAR *calendar) {
    int j = UpdateVeventItem(calendar->year, calendar->month, calendar->day, Veventlistitem);
    if (j == 0) {
        sprintf(Veventlistitem[0], " ");
        j = 1;
    }
    eraseCDKScroll(Veventwidget);
    setCDKScroll(Veventwidget, (CDK_CSTRING2) Veventlistitem, j, FALSE, A_REVERSE, TRUE);
    drawCDKScroll(Veventwidget, TRUE);
}

void RefreshTodolist(char *const *todolistitem) {
    int i = 0;
    for (auto &it:todolist.v) {
        stringstream sstream;
        sstream << it.first << " " << it.second << endl;
        sstream.getline(todolistitem[i], VECTORLENGTH);
        i++;
    }

    if (i == 0) {
        sprintf(todolistitem[0], " ");
        i = 1;
    }
    eraseCDKScroll(Todolistwidget);
    setCDKScroll(Todolistwidget, (CDK_CSTRING2) todolistitem, i, FALSE, A_REVERSE, TRUE);
    drawCDKScroll(Todolistwidget, TRUE);
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

std::string poptime(const char *title, const char *label) {
    char *charvalue;
    char temp[256];
    char *info;
    char *mixed;
    int state = 1;
    do {
        const char *Overlay = "____/__/__-__:__:__";
        const char *plate = "####/##/##-##:##:##";
        CDKTEMPLATE *cdktemplate = 0;

        cdktemplate = newCDKTemplate(cdkscreen, CENTER, CENTER, title, label, plate, Overlay, TRUE, FALSE);
        info = activateCDKTemplate(cdktemplate, 0);
        if (IsLegal(info)) {
            state = 0;
            std::string time = info;
            destroyCDKEntry(cdktemplate);
            eraseCDKScreen(cdkscreen);
            refreshCDKScreen(cdkscreen);
            return time;
        }
        destroyCDKEntry(cdktemplate);
        const char *mesg[3];

        mesg[0] = "time is not legal!";
        mesg[1] = "";
        mesg[2] = "<C>please enter new time.";

        popupLabel(cdkscreen, (CDK_CSTRING2) mesg, 3);
        eraseCDKScreen(cdkscreen);
        refreshCDKScreen(cdkscreen);
    } while (state);
}


std::string poptime(const char *title, const char *label, const char *val) {
    char *charvalue;
    char temp[256];
    char *info;
    char *mixed;
    int state = 1;
    do {
        const char *Overlay = "____/__/__-__:__:__";
        const char *plate = "####/##/##-##:##:##";
        CDKTEMPLATE *cdktemplate = 0;

        cdktemplate = newCDKTemplate(cdkscreen, CENTER, CENTER, title, label, plate, Overlay, TRUE, FALSE);
        setCDKTemplate(cdktemplate, val, TRUE);
        info = activateCDKTemplate(cdktemplate, 0);
        if (IsLegal(info)) {
            state = 0;
            std::string time = info;
            destroyCDKEntry(cdktemplate);
            eraseCDKScreen(cdkscreen);
            refreshCDKScreen(cdkscreen);
            return time;
        }
        destroyCDKEntry(cdktemplate);
        const char *mesg[3];

        mesg[0] = "time is not legal!";
        mesg[1] = "";
        mesg[2] = "<C>please enter new time.";

        popupLabel(cdkscreen, (CDK_CSTRING2) mesg, 3);
        eraseCDKScreen(cdkscreen);
        refreshCDKScreen(cdkscreen);
    } while (state);
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

int popscale(const char *title, const char *label, int max) {
    CDKSCALE *importance = 0;
    int iimport;
    importance = newCDKScale(cdkscreen, CENTER, CENTER, title, label, A_NORMAL, 5, 1, 1, max, 1, 2, TRUE, FALSE);
    iimport = activateCDKScale(importance, 0);
    destroyCDKScale(importance);
    eraseCDKScreen(cdkscreen);
    refreshCDKScreen(cdkscreen);
    return iimport;
}


bool IsLeapYear(int year) {
    if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
        return true;
    return false;
}

bool IsLegal(char *time) {
    int year;
    int mon;
    int day;
    int hour;
    int minute;
    int second;
    year = (time[0] - '0') * 1000 + (time[1] - '0') * 100 + (time[2] - '0') * 10 + (time[3] - '0');
    mon = (time[4] - '0') * 10 + (time[5] - '0');
    day = (time[6] - '0') * 10 + (time[7] - '0');
    hour = (time[8] - '0') * 10 + (time[9] - '0');
    minute = (time[10] - '0') * 10 + (time[11] - '0');
    second = (time[12] - '0') * 10 + (time[13] - '0');
    if (year < 0 || mon <= 0 || mon > 12 || day <= 0 || day > 31 || hour >= 24 || hour < 0 || minute >= 60 ||
        minute < 0 || second >= 60 || second < 0)
        return false;

    if (1 == mon || 3 == mon || 5 == mon || 7 == mon || 8 == mon || 10 == mon || 12 == mon) {
        return true;
    }
    if (IsLeapYear(year)) {
        return !(2 == mon && (28 == day || 30 == day || 31 == day));
    } else {
        return !(2 == mon && (29 == day || 30 == day || 31 == day));
    }
}