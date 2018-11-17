//
// Created by hukq1 on 2018/11/16.
//

#ifndef THUCAL_TODOLIST_H
#define THUCAL_TODOLIST_H

#include <map>
#include <string>

class Todolist {
public:
    Todolist();

    void IntoMode();

    void LeaveMode();

    bool InsertTodo(int i, std::string s);

    bool DeleteNowTodo();

    bool ChangeNowTodo(std::string s);

    std::string GetNowTodoValue();

    int GetNowTodoKey();

    void NextTodo();

    void PrevTodo();

    void GotoFisrtTodo();

    void GotoLastTodo();

    bool isend();

    bool isbegin();

//private:
    std::multimap<int, std::string> v;
    std::multimap<int, std::string>::iterator p;
    int mode = 0;
};


#endif //THUCAL_TODOLIST_H
