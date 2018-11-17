//
// Created by hukq1 on 2018/11/16.
//

#ifndef THUCAL_TODOLIST_H
#define THUCAL_TODOLIST_H

#include <map>
#include <string>

class Todolist {
private:
    std::map<int,> v;
public:

    void InsertTodo(int i, std::string s);

    std::string DeleteTodo(int i, std::string s);

    void ChangeTodo(int i, int n, std::string s);
};


#endif //THUCAL_TODOLIST_H
