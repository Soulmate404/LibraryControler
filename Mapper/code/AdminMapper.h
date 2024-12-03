//
// Created by 14403 on 2024/12/3.
//

#ifndef LIBRARYCONTROLLER_ADMINMAPPER_H
#define LIBRARYCONTROLLER_ADMINMAPPER_H

#include <mysql.h>

MYSQL_ROW SelectUser(int id);
int AddBooks(int id,char* name,char* writer,int last_num,char* position);
int AddUser(int id,char* name,char* pass_wd,int authority);
int DeleteBook(int id);
int DeleteUser(int id);
int UpdateUserAuthority(int id,int authority);
MYSQL_ROWS CheckUserBorrow(int id);
MYSQL_ROWS CheckAllBorrow();
MYSQL_ROWS CheckBooksBorrow(int id);
int RootResetPass(int id,char* pass);
int CheckUser(int id);
#endif //LIBRARYCONTROLLER_ADMINMAPPER_H
