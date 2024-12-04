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

int selectUser(int id) {
    MYSQL_ROW row = SelectUser(id);
    if (row == NULL) {
        return -1;
    }else {
        for (int i = 0; i < 4; i ++) {
            printf("%s\t",row[i]);
        }
        return 0;
    }
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

int checkUserBorrow(int id) {
    if (id != 0){
        MYSQL_ROWS rows2= CheckUserBorrow(id);
        MYSQL_ROWS *rows = &rows2;
        if (rows->data == NULL) {
            return -1;
        }else {
            while (rows) {
                for (int i = 0; i < 4; i++) {
                    printf("%s\t",rows->data[i]);
                }
               rows=rows->next;

            }
            return 0;
        }
    }else {
        MYSQL_ROWS rows2= CheckAllBorrow();
        MYSQL_ROWS *rows = &rows2;
        if (rows->data == NULL) {
            return -1;
        }else {
            while (rows) {
                for (int i = 0; i < 4; i++) {
                    printf("%s\t",rows->data[i]);

                }
                rows=rows->next;

            }
            return 0;
       }
    }
}
int checkBooksBorrow(int id) {
    MYSQL_ROWS rows2= CheckBooksBorrow(id);
    MYSQL_ROWS *rows = &rows2;
    if (rows->data == NULL) {
        return -1;
    }else {
        while (rows) {
            for (int i = 0; i < 4; i++) {
                printf("%s\t",rows->data[i]);

            }
            rows = rows->next;
        }
        return 0;
    }
}

int rootResetPass(int id,char* c1,char* c2) {
    if (CheckUser(id) > 0 || CheckUser(id) == 0) {
        if (strcmp(c1,c2)==0) {
            RootResetPass(id,c1);
            return 0;
        }else {
            return -1;
        }
    }else {
        return -1;
    }
}