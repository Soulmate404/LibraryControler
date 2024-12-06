//
// Created by 14403 on 2024/12/4.
//

#ifndef LIBRARYCONTROLLER_SQLPOINTER_H
#define LIBRARYCONTROLLER_SQLPOINTER_H

#include <mysql.h>

extern MYSQL *conn;
extern MYSQL_RES *res;
extern MYSQL_ROW row;
extern MYSQL_STMT *stmt;

void init();
void freeSQLResources();
#endif //LIBRARYCONTROLLER_SQLPOINTER_H
