//
// Created by 14403 on 2024/12/3.
//

#include "UserLogMapper.h"
#include <string.h>
#include "mysql.h"
#include "../code/mysqlPointer.h"
#include <stdio.h>
#include <stdlib.h>



const char *server = "localhost";
const char *user = "root";
const char *password = "guo12345";
const char *database = "librarymanager";
void init(){
    conn = mysql_init(NULL);
    if (mysql_real_connect(conn, server, user, password, database, 3306, NULL, 0)) {
        printf("success\n");
    }
}

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
        mysql_free_result(res);
        return atoi(row[3]);
    } else{
        mysql_free_result(res);
        return -1;
    }
}
int ResetPass(int id,char* pass) {


    char ID[50];
    sprintf(ID, "%d", id);


    char sql[256];
    strcpy(sql, "UPDATE user SET pass_wd = '");
    strcat(sql, pass);
    strcat(sql, "' WHERE id=");
    strcat(sql, ID);
    strcat(sql, ";");


    if (mysql_query(conn, sql)) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(conn));
        return -1;
    }


    if (mysql_affected_rows(conn) > 0) {
        return 0;
    } else {
        return -1;
    }
}
