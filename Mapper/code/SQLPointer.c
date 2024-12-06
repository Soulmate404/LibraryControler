//
// Created by 14403 on 2024/12/4.
//

#include <stdio.h>
#include "SQLPointer.h"
const char *server = "192.168.5.21";
const char *user = "root";
const char *password = "guo12345";
const char *database = "librarymanager";

MYSQL *conn = NULL;
MYSQL_RES *res = NULL;
MYSQL_ROW row = NULL;
MYSQL_STMT *stmt=NULL;


void init(){
    conn = mysql_init(NULL);



    if (mysql_real_connect(conn, server, user, password, database, 3306, NULL, 0)&& (stmt = mysql_stmt_init(conn))) {
        printf("success\n");
    }

    //mysql_set_character_set(conn, "utf8mb4");

}
// 初始化全局变量


// 函数用于释放资源
void freeSQLResources() {
    if (res != NULL) {
        mysql_free_result(res);
        res = NULL;
    }
    if (conn != NULL) {
        mysql_close(conn);
        conn = NULL;
    }
    if(stmt!=NULL){
        mysql_stmt_close(stmt);
        stmt=NULL;
    }
}
