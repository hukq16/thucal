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

    bool InsertTodo(int i, std::string s);

    std::multimap<int, std::string> v;

    void ImportTodo(std::string FilePath);

    void OutputTodo(std::string FilePath);
};


#endif //THUCAL_TODOLIST_H
