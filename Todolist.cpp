#include <utility>

//
// Created by hukq1 on 2018/11/16.
//

#include "Todolist.h"

Todolist::Todolist() {
}

bool Todolist::InsertTodo(int i, std::string s) {
    if (v.empty() && mode)
        p = v.begin();
    if (i <= 0 || i > 9)
        return false;
    v.insert(std::pair<int, std::string>(i, s));
    return true;
}

bool Todolist::DeleteNowTodo() {
    if (v.empty() || !mode)
        return false;
    p = v.erase(p);
    return true;
}

bool Todolist::ChangeNowTodo(std::string s) {
    if (v.empty() || !mode)
        return false;
    p->second = std::move(s);
    return true;
}

void Todolist::NextTodo() {
    if (p != v.end())
        p++;
}

void Todolist::PrevTodo() {
    if (p != v.begin())
        p--;
}

void Todolist::GotoFisrtTodo() {
    p = v.begin();
}

void Todolist::GotoLastTodo() {
    p = v.end();
}

std::string Todolist::GetNowTodoValue() {
    return p->second;
}

bool Todolist::isend() {
    return p == v.end();
}

bool Todolist::isbegin() {
    return p == v.begin();
}

int Todolist::GetNowTodoKey() {
    return p->first;
}

void Todolist::IntoMode() {
    mode = 1;
    p = v.begin();
}

void Todolist::LeaveMode() {
    mode = 0;
}
