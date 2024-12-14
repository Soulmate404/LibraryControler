//
// Created by 14403 on 2024/12/4.
//

#include <stdio.h>
#include "SQLPointer.h"
const char *server = "8.137.13.231";
const char *user = "guo";
const char *password = "guo12345";
const char *database = "library";

MYSQL *conn = NULL;
MYSQL_RES *res = NULL;
MYSQL_ROW row = NULL;
MYSQL_STMT *stmt=NULL;


void init(){
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return;
    }

    // 设置字符集为 utf8
    mysql_options(conn, MYSQL_SET_CHARSET_NAME, "utf8");
    mysql_options(conn, MYSQL_INIT_COMMAND, "SET NAMES utf8");





    if (mysql_real_connect(conn, server, user, password, database, 6848, NULL, 0)&& (stmt = mysql_stmt_init(conn))) {

    }else{
        printf("failed");
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
