#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include "cal.h"
using namespace std;

int main(int argc, char const *argv[]) {
//    cout << "hello world!" << endl;
    cal ss;
    initscr();
    ss.test();
    refresh();
    sleep(2);
    endwin();
    return 0;
}

