//
// Created by hukq1 on 2018/11/16.
//

#ifndef THUCAL_TODOLIST_H
#define THUCAL_TODOLIST_H

#include <vector>
#include <string>
#include <cstdlib>

class TodoThing {
private:
    std::vector<std::string> v;
public:
    void AddTodoThing(std::string s);

    void ChangeTodoThing(int n, std::string s);

    std::string DeleteTodoThing(int n);

    int GetSize();

    std::string GetTodoThing(int n);
};


#endif //THUCAL_TODOLIST_H
