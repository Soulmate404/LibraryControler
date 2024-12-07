//
// Created by 14403 on 2024/12/3.
//

#include "BookBorrowMapper.h"
#include <stdio.h>
#include <mysql.h>

#include "SQLPointer.h"


int CheckBook(int id) {
    char ID[50];
    sprintf(ID, "%d", id);

    char sql[256];
    strcpy(sql, "SELECT * FROM book WHERE id=");
    strcat(sql, ID);
    strcat(sql, ";");

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(conn));
        return -1;
    }
    res= mysql_use_result(conn);
    if((row= mysql_fetch_row(res))){
        mysql_free_result(res);
        return 0;
    } else{
        mysql_free_result(res);
        return -1;
    }
}
int GetBookNum(int id) {
    char ID[50];
    sprintf(ID, "%d", id);

    char sql[256];
    strcpy(sql, "SELECT * FROM book WHERE id=");
    strcat(sql, ID);
    strcat(sql, ";");

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(conn));
        return -1;
    }

    res = mysql_use_result(conn);
    if ((row = mysql_fetch_row(res))) {
        int a=atoi(row[3]);
        mysql_free_result(res);
        return a;
    } else {
        mysql_free_result(res);
        return -1;
    }
}
MYSQL_ROW SelectByID(int id) {
    char ID[50];
    sprintf(ID, "%d", id);

    char sql[256];
    strcpy(sql, "SELECT * FROM book WHERE id=");
    strcat(sql, ID);
    strcat(sql, ";");


    if (mysql_real_query(conn, sql, strlen(sql))) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(conn));
        return NULL;
    }

    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed. Error: %s\n", mysql_error(conn));
        return NULL;
    }


    MYSQL_ROW mysqlRow = mysql_fetch_row(res);
    if (mysqlRow == NULL) {
        mysql_free_result(res);  // 不要忘记释放资源
        return NULL;
    }


    size_t num_fields = mysql_num_fields(res);


    MYSQL_ROW result = malloc(sizeof(char*) * num_fields);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        mysql_free_result(res);
        return NULL;
    }


    for (size_t i = 0; i < num_fields; ++i) {
        if (mysqlRow[i]) {
            result[i] = strdup(mysqlRow[i]);
        } else {
            result[i] = NULL;
        }
    }

    mysql_free_result(res);

    return result;
}

MYSQL_RES * SelectByName(char *name) {

    char query[256];
    char Name[100];
    strcpy(Name,"%");
    strcat(Name,name);
    strcat(Name,"%");
    snprintf(query, sizeof(query), "SELECT * FROM book WHERE name like '%s'", Name);


    if (mysql_query(conn, query)) {
        fprintf(stderr, "SELECT query failed: %s\n", mysql_error(conn));
        MYSQL_RES *d = NULL;
        d->data=NULL;
        return  d; // 查询失败，返回 NULL
    }


    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "Store result failed: %s\n", mysql_error(conn));
        MYSQL_RES *d;
        d->data=NULL;
        return  d; // 获取结果失败，返回 NULL
    }


    if (mysql_num_rows(res) == 0) {
        mysql_free_result(res);  // 释放结果集
        MYSQL_RES *d;
        d->data=NULL;
        return  d;
    }

    return res;
}
int AddBorrow(int userid, char* name, int bookid, char* time) {
    if (conn == NULL) {
        return -1;
    }


    const char *sql = "INSERT INTO borrow (user_id, name, book_id, borrow_time) VALUES (?, ?, ?, ?)";
    if (mysql_stmt_prepare(stmt, sql, strlen(sql))) {
        mysql_stmt_free_result(stmt);
        return -1;
    }

    MYSQL_BIND bind[4];

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &userid;
    bind[0].is_null = 0;
    bind[0].length = 0;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = name ? name : "NULL";
    bind[1].buffer_length = name ? strlen(name) : 4;
    bind[1].is_null = 0;
    bind[1].length = NULL;

    bind[2].buffer_type = MYSQL_TYPE_LONG;
    bind[2].buffer = &bookid;
    bind[2].is_null = 0;
    bind[2].length = 0;

    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = time ? time : "NULL";
    bind[3].buffer_length = time ? strlen(time) : 4;
    bind[3].is_null =0;
    bind[3].length = NULL;

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
int DeleteBorrow(int userid,int bookid){
    char uID[50];
    sprintf(uID, "%d", userid);
    char bID[50];
    sprintf(bID, "%d", bookid);

    char sql[256];
    strcpy(sql, "DELETE FROM borrow WHERE user_id= ");
    strcat(sql, uID);
    strcat(sql, " and book_id =");
    strcat(sql, bID);
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
int CheckBorrow(int userid,int bookid){
    char uID[50];
    sprintf(uID, "%d", userid);
    char bID[50];
    sprintf(bID, "%d", bookid);

    char sql[256];
    strcpy(sql, "SELECT * FROM borrow WHERE user_id= ");
    strcat(sql, uID);
    strcat(sql, " and book_id =");
    strcat(sql, bID);
    strcat(sql, ";");
    if (mysql_query(conn, sql)) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(conn));
        return -1;
    }
    res= mysql_use_result(conn);
    if((row= mysql_fetch_row(res))){
        mysql_free_result(res);
        return 0;
    } else{
        mysql_free_result(res);
        return -1;
    }
}
