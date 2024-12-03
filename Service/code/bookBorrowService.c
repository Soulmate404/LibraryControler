//
// Created by 14403 on 2024/12/3.
//

#include "bookBorrowService.h"
#include <stdio.h>
#include "../../Mapper/code/BookBorrowMapper.h"

int selectByID(int id) {
    MYSQL_ROW row = SelectByID(id);
    if (row == NULL) {
        return -1;
    }
    else{
        for (int i=0;i<4;i++) {
            printf("%s\t",row[i]);
        }
        return 0;
    }
}

int selectByName(char* name) {
    MYSQL_ROWS rows1= SelectByName(name);
    MYSQL_ROWS *rows = &rows1;
    if (rows->data == NULL) {
        return -1;
    }
    while (rows->next != NULL) {
        for (int i = 0; i < 4; i++) {
            printf("%s\t",rows->data[i]);
        }
        rows->next= rows->next->next;
    }
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