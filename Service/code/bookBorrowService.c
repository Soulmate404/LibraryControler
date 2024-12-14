//
// Created by 14403 on 2024/12/3.
//

#include "bookBorrowService.h"
#include <stdio.h>
#include "../../Mapper/code/BookBorrowMapper.h"

int selectByID(int id, char* result) {
    MYSQL_ROW row = SelectByID(id);
    
    if (row == NULL) {
        return -1;
    }

    // 清空结果字符串
    result[0] = '\0';
    
    // 添加表头，使用空格代替制表符以获得更好的对齐效果
    strcat(result, "图书ID    书名                作者                剩余数量    位置\r\n");
    strcat(result, "----------------------------------------------------------------\r\n");
    
    // 拼接结果，确保每列有固定宽度
    char temp[100];
    sprintf(temp, "%-10s", row[0]); strcat(result, temp);  // ID 列宽 10
    sprintf(temp, "%-20s", row[1]); strcat(result, temp);  // 书名列宽 20
    sprintf(temp, "%-20s", row[2]); strcat(result, temp);  // 作者列宽 20
    sprintf(temp, "%-12s", row[3]); strcat(result, temp);  // 剩余数量列宽 12
    sprintf(temp, "%s", row[4]); strcat(result, temp);     // 位置
    strcat(result, "\r\n");
    
    return 0;
}

int selectByName(char* name, char* result) {
    char *found = strchr(name, ';');
    if(found != NULL) {
        return -1;
    }
    
    MYSQL_RES *mysqlRes = SelectByName(name);
    if(mysql_num_rows(mysqlRes) == 0 || mysql_num_rows(mysqlRes) > 100) {
        mysql_free_result(mysqlRes);
        return -1;
    }
    
    // 清空结果字符串
    result[0] = '\0';
    
    // 添加表头
    strcat(result, "图书ID    书名                作者                剩余数量    位置\r\n");
    strcat(result, "----------------------------------------------------------------\r\n");
    
    MYSQL_ROW row;
    unsigned int filed = mysql_num_fields(mysqlRes);
    while((row = mysql_fetch_row(mysqlRes))) {
        char temp[100];
        sprintf(temp, "%-10s", row[0]); strcat(result, temp);
        sprintf(temp, "%-20s", row[1]); strcat(result, temp);
        sprintf(temp, "%-20s", row[2]); strcat(result, temp);
        sprintf(temp, "%-12s", row[3]); strcat(result, temp);
        sprintf(temp, "%s", row[4]); strcat(result, temp);
        strcat(result, "\r\n");
    }
    
    mysql_free_result(mysqlRes);
    return 0;
}

int addBorrow(int userid, char* name, int bookid, char* time) {
    // 首先检查图书是否存在
    if (CheckBook(bookid) != 0) {
        return 1;  // 图书不存在
    }
    
    // 检查图书是否有剩余
    int remaining = GetBookNum(bookid);
    if (remaining <= 0) {
        return 2;  // 图书已被借完
    }
    
    // 检查用户是否已经借阅过这本书
    if (CheckBorrow(userid, bookid) == 0) {
        return 3;  // 用户已经借阅过这本书
    }
    
    // 执行借书操作
    return AddBorrow(userid, name, bookid, time);
}

int deleteBorrow(int userid,int bookid) {
    if (CheckBorrow(userid,bookid) == 0) {
        return DeleteBorrow(userid,bookid);
    }else {
        return -1;
    }
}