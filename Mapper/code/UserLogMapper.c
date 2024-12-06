//
// Created by 14403 on 2024/12/3.
//

#include "UserLogMapper.h"
#include <string.h>
#include "mysql.h"

#include "SQLPointer.h"
#include <stdlib.h>
#include <stdio.h>


int CheckUserInLog(int id, char* pass_wd) {
    char ID[50];
    sprintf(ID, "%d", id);

    char sql[256];
    strcpy(sql, "select * from user where id=");
    strcat(sql, ID);
    strcat(sql, " and pass_wd = '");
    strcat(sql, pass_wd);
    strcat(sql, "';");
    mysql_query(conn,sql);
    res= mysql_use_result(conn);
    if((row= mysql_fetch_row(res))){
        int a= atoi(row[3]);
        mysql_free_result(res);
        return a;
    } else{
        mysql_free_result(res);
        return -1;
    }
}
int ResetPass(int id, char* pass) {
    if (conn == NULL) {
        return -1;
    }
    if(pass==NULL){
        return -1;
    }



    const char *sql = "UPDATE user SET pass_wd = ? WHERE id = ?";
    if (mysql_stmt_prepare(stmt, sql, strlen(sql))) {
        mysql_stmt_free_result(stmt);
        return -1;
    }

    MYSQL_BIND bind[2];

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = pass;
    bind[0].buffer_length = strlen(pass);
    bind[0].is_null = 0;
    bind[0].length = NULL;

    bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = &id;
    bind[1].is_null = 0;
    bind[1].length = 0;

    if (mysql_stmt_bind_param(stmt, bind)) {
        mysql_stmt_free_result(stmt);
        return -1;
    }

    if (mysql_stmt_execute(stmt)) {
        mysql_stmt_free_result(stmt);
        return -1;
    }

    if (mysql_stmt_affected_rows(stmt) > 0) {
        mysql_stmt_free_result(stmt);
        return 0;
    } else {
        mysql_stmt_free_result(stmt);
        return -1;
    }
}
