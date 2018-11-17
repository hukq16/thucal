#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <string>
#include "Todolist.h"
using namespace std;

int main(int argc, char const *argv[]) {
    Todolist todolist;
    todolist.IntoMode();
    todolist.InsertTodo(2, "do math homework");
    todolist.InsertTodo(5, "play game");
    todolist.InsertTodo(5, "add new feature");
    for (todolist.GotoFisrtTodo(); !todolist.isend(); todolist.NextTodo()) {
        cout << todolist.GetNowTodoKey() << "  " << todolist.GetNowTodoValue() << endl;
    }

    todolist.GotoFisrtTodo();
    todolist.NextTodo();
    todolist.ChangeNowTodo("ssss");

    todolist.NextTodo();
    todolist.DeleteNowTodo();

    for (todolist.GotoFisrtTodo(); !todolist.isend(); todolist.NextTodo()) {
        cout << todolist.GetNowTodoKey() << "  " << todolist.GetNowTodoValue() << endl;
    }
    return 0;

}

