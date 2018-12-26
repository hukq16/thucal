//
// Created by hukq1 on 2018/11/16.
//

#ifndef THUCAL_TODOLIST_H
#define THUCAL_TODOLIST_H

#include <map>
#include <string>

class Todolist {
public:
    /**
     * 初始化
     */
    Todolist();

    /**
     * 插入todo项目
     * @param  i 优先级
     * @param  s 描述
     * @return   是否插入成功
     */
    bool InsertTodo(int i, std::string s);

    /**
     * 导入todolist存储文件
     * @param FilePath 文件路径
     */
    void ImportTodo(std::string FilePath);

    /**
     * 导出todolist存储文件
     * @param FilePath 文件路径
     */
    void OutputTodo(std::string FilePath);

    std::multimap<int, std::string> v;
};


#endif //THUCAL_TODOLIST_H
