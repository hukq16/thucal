#include <utility>

//
// Created by hukq1 on 2018/11/16.
//

#include "TodoThing.h"


void TodoThing::AddTodoThing(std::string s) {
    v.push_back(s);
}

void TodoThing::ChangeTodoThing(int n, std::string s) {
    if (n < 0 || n > v.size())
        std::abort();
    v[n] = s;
}

std::string TodoThing::DeleteTodoThing(int n) {
    if (n < 0 || n > v.size())
        std::abort();
    std::string value = v[n];
    v.erase(v.begin() + n);
    return value;
}

int TodoThing::GetSize() {
    return static_cast<int>(v.size());
}

std::string TodoThing::GetTodoThing(int n) {
    if (n < 0 || n > v.size())
        std::abort();
    return v[n];
}


