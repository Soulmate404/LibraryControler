//
// Created by 14403 on 2024/12/3.
//

#include "adminService.h"
#include <stdio.h>
#include <string.h>
#include "../../Mapper/code/AdminMapper.h"
#include "../../Mapper/code/BookBorrowMapper.h"


int addBooks(int id,char* name,char* writer,int last_num,char* position) {
    if (CheckBook(id) != 0) {
        return AddBooks(id,name,writer,last_num,position);
    }else {
        return -1;
    }
}

int deleteBooks(int id) {
    if (CheckBook(id) == 0) {
        return DeleteBook(id);
    }else {
        return -1;
    }
}

int addUser(int id,char* name,char* pass_wd,int authority) {
    if (CheckUser(id) > 0 || CheckUser(id) == 0) {
        return -1;
    }else {
        return AddUser(id,name,pass_wd,authority);
    }
}

int deleteUser(int id) {
    if (CheckUser(id) > 0 || CheckUser(id) == 0) {
        return DeleteUser(id);
    }else {
        return -1;
    }
}

int selectUser(int id, char* result) {
    MYSQL_ROW row1 = SelectUser(id);
    
    if (row1 == NULL) {
        return -1;
    }
    
    // 清空结果字符串
    result[0] = '\0';
    
    // 添加表头
    strcat(result, "用户ID    用户名              密码                权限\r\n");
    strcat(result, "--------------------------------------------------------\r\n");
    
    // 格式化输出
    char temp[100];
    sprintf(temp, "%-10s", row1[0]); strcat(result, temp);
    sprintf(temp, "%-20s", row1[1]); strcat(result, temp);
    sprintf(temp, "%-20s", row1[2]); strcat(result, temp);
    sprintf(temp, "%s", row1[3]); strcat(result, temp);
    strcat(result, "\r\n");
    
    return 0;
}

int updateAuthority(int id,int authority) {
    if (CheckUser(id) > 0 || CheckUser(id) == 0) {
        if (authority != 0 && authority != 1) {
            return 2;
        }else {
            return UpdateUserAuthority(id,authority);
        }
    }else {
        return 1;
    }
}

int checkUserBorrow(int id, char* result) {
    result[0] = '\0';
    strcat(result, "用户ID    图书ID    借阅时间            状态\r\n");
    strcat(result, "------------------------------------------------\r\n");
    
    if (id != 0) {
        MYSQL_ROWS rows2 = CheckUserBorrow(id);
        MYSQL_ROWS *rows = &rows2;
        if (rows->data == NULL) {
            return -1;
        }
        
        while (rows) {
            for (int i = 0; i < 4; i++) {
                strcat(result, rows->data[i] ? rows->data[i] : "NULL");
                strcat(result, "\t");
            }
            strcat(result, "\n");
            rows = rows->next;
        }
        return 0;
    } else {
        // 处理查看所有借阅记录的情况
        MYSQL_ROWS rows2 = CheckAllBorrow();
        MYSQL_ROWS *rows = &rows2;
        if (rows->data == NULL) {
            return -1;
        }
        
        while (rows) {
            for (int i = 0; i < 4; i++) {
                strcat(result, rows->data[i] ? rows->data[i] : "NULL");
                strcat(result, "\t");
            }
            strcat(result, "\n");
            rows = rows->next;
        }
        return 0;
    }
}

int checkBooksBorrow(int id, char* result) {
    MYSQL_ROWS rows2 = CheckBooksBorrow(id);
    MYSQL_ROWS *rows = &rows2;
    
    if (rows->data == NULL) {
        return -1;
    }
    
    // 清空结果字符串
    result[0] = '\0';
    
    // 添加表头
    strcat(result, "图书ID    借阅用户ID    借阅时间            状态\r\n");
    strcat(result, "----------------------------------------------------\r\n");
    
    while (rows) {
        char temp[100];
        sprintf(temp, "%-10s", rows->data[0]); strcat(result, temp);
        sprintf(temp, "%-14s", rows->data[1]); strcat(result, temp);
        sprintf(temp, "%-20s", rows->data[2]); strcat(result, temp);
        sprintf(temp, "%s", rows->data[3]); strcat(result, temp);
        strcat(result, "\r\n");
        rows = rows->next;
    }
    return 0;
}

