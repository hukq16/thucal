#include <utility>

//
// Created by hukq1 on 2018/11/16.
//

#include "Todolist.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <memory.h>

Todolist::Todolist() {
}

bool Todolist::InsertTodo(int i, std::string s) {
    if (i <= 0 || i > 9)
        return false;
    v.insert(std::pair<int, std::string>(i, s));
    return true;
}

void Todolist::ImportTodo(std::string FilePath) {
    std::ifstream file(FilePath, std::ios::in);
    if (!file) {
        std::cerr << "file not exist!" << std::endl;
        return;
    }
    while (!file.eof()) {
        char *summary = new char[256];
        file.getline(summary, 256);
        if (strlen(summary) == 0)
            return;
        int importance = atoi(strtok_r(summary, " ", &summary));
        std::string summ = summary;
        InsertTodo(importance, summ);
    }
    file.close();
}

void Todolist::OutputTodo(std::string FilePath) {
    std::ofstream file(FilePath, std::ios::out);
    for (const auto &in :v) {
        file << in.first << " " << in.second << "\n";
    }
    file.close();
    std::ifstream ifile(FilePath, std::ios::in);
    ifile.seekg(0, std::ios::end);
    long long int n = ifile.tellg();
    ifile.close();
    truncate(FilePath.c_str(), n - 1);
}

