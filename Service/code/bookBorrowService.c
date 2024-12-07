//
// Created by 14403 on 2024/12/3.
//

#include "bookBorrowService.h"
#include <stdio.h>
#include "../../Mapper/code/BookBorrowMapper.h"

int selectByID(int id) {
    printf("Entering selectByID with id: %d\n", id);  // 调试信息

    MYSQL_ROW row = SelectByID(id);
    char a[500];

    if (row == NULL) {
        printf("Exiting selectByID: row is NULL\n");  // 调试信息
        return -1;
    }

    a[0] = '\0';

    for (int i = 0; i < 4; i++) {
        if (row[i] != NULL) {
            if (i == 0) {
                strcpy(a, row[i]);
                strcat(a, "\t");
            } else if (i == 3) {
                strcat(a, row[i]);
            } else {
                strcat(a, row[i]);
                strcat(a, "\t");
            }
        }
    }

    printf("%s\n", a);

    printf("Exiting selectByID successfully\n");  // 调试信息
    return 0;
}


int selectByName(char* name) {
    char *found = strchr(name, ';');
    if(found!=NULL){
        return -1;
    }
    MYSQL_RES *mysqlRes= SelectByName(name);
    if(mysql_num_rows(mysqlRes) == 0|| mysql_num_rows(mysqlRes)>100){
        mysql_free_result(mysqlRes);
        return -1;
    }
    MYSQL_ROW row;
    unsigned int filed= mysql_num_fields(mysqlRes);
    while((row= mysql_fetch_row(mysqlRes))){
        for(int i=0;i<filed;i++){
            printf("%s\t",row[i]);
        }
        printf("\n");
    }
    mysql_free_result(mysqlRes);
    return 0;
}
int addBorrow(int userid,char* name,int bookid,char* time) {
    if (CheckBook(bookid) == 0) {
        if (GetBookNum(bookid) > 0) {
            return AddBorrow(userid,name,bookid,time);
        }else if (GetBookNum(bookid) == 0){
            return 2;
        }else if (GetBookNum(bookid) == -1) {
            return 1;
        }
    }else {
        return 1;
    }
}

int deleteBorrow(int userid,int bookid) {
    if (CheckBorrow(userid,bookid) == 0) {
        return DeleteBorrow(userid,bookid);
    }else {
        return -1;
    }
}