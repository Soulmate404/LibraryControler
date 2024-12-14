//
// Created by 14403 on 2024/12/3.
//

#ifndef LIBRARYCONTROLLER_BOOKBORROWSERVICE_H
#define LIBRARYCONTROLLER_BOOKBORROWSERVICE_H

// 添加字符串缓冲区大小定义
#define MAX_RESULT_LENGTH 4096

// 修改函数声明，添加结果字符串参数
int selectByID(int id, char* result);
int selectByName(char* name, char* result);
int addBorrow(int userid, char* name, int bookid, char* time);
int deleteBorrow(int userid, int bookid);

#endif //LIBRARYCONTROLLER_BOOKBORROWSERVICE_H
