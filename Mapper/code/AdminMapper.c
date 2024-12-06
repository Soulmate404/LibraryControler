//
// Created by 14403 on 2024/12/3.
//

#include "AdminMapper.h"
#include <mysql.h>
#include <stdio.h>
#include "SQLPointer.h"


MYSQL_ROW SelectUser(int id){
    MYSQL_BIND bind[4];


    const char *query = "SELECT * FROM user WHERE id = ?";





    if (mysql_stmt_prepare(stmt, query, strlen(query))) {
        fprintf(stderr, "mysql_stmt_prepare() failed: %s\n", mysql_error(conn));
        return NULL;
    }


    MYSQL_BIND param;
    param.buffer_type = MYSQL_TYPE_LONG;
    param.buffer = &id;
    param.is_null = 0;
    param.length = 0;


    if (mysql_stmt_bind_param(stmt, &param)) {
        fprintf(stderr, "mysql_stmt_bind_param() failed: %s\n", mysql_error(conn));
        return NULL;
    }


    if (mysql_stmt_execute(stmt)) {
        fprintf(stderr, "mysql_stmt_execute() failed: %s\n", mysql_error(conn));
        return NULL;
    }


    int ID;
    char name[10];
    char password[20];
    int privilege_level;

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &ID;
    bind[0].is_null = 0;
    bind[0].length = 0;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = name;
    bind[1].buffer_length = sizeof(name);
    bind[1].is_null = 0;
    bind[1].length = 0;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = password;
    bind[2].buffer_length = sizeof(password);
    bind[2].is_null = 0;
    bind[2].length = 0;

    bind[3].buffer_type = MYSQL_TYPE_LONG;
    bind[3].buffer = &privilege_level;
    bind[3].is_null = 0;
    bind[3].length = 0;


    if (mysql_stmt_bind_result(stmt, bind)) {
        fprintf(stderr, "mysql_stmt_bind_result() failed: %s\n", mysql_error(conn));
        return NULL;
    }


    static MYSQL_ROW mysqlRow= NULL;
    mysqlRow=malloc(sizeof (char)*500);
    if (mysql_stmt_fetch(stmt) == 0) {
        char charID[50];
        sprintf(charID,"%d",ID);
        char charPrivilege[1];
        sprintf(charPrivilege,"%d",privilege_level);
        char *Name=name;
        mysqlRow[0] = charID;
        mysqlRow[1] = Name;
        mysqlRow[2] = password;
        mysqlRow[3] = charPrivilege;
    } else {

        mysqlRow = NULL;
    }


    mysql_stmt_free_result(stmt);

    return mysqlRow;
}

int AddBooks(int id,char* name,char* writer,int last_num,char* position){
    if (conn == NULL) {
        fprintf(stderr, "Database connection is not initialized.\n");
        return -1;
    }


    char sql[512];
    sprintf(sql, "INSERT INTO book (id, name, writer, remaining_quantity,position) VALUES (%d, '%s','%s', %d,'%s');",
            id, name ? name : "NULL", writer?writer:"NULL",last_num,position?position:"NULL");


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
int AddUser(int id,char* name,char* pass_wd,int authority){
    if (conn == NULL) {
        fprintf(stderr, "Database connection is not initialized.\n");
        return -1;
    }


    char sql[512];
    sprintf(sql, "INSERT INTO user (id, name, pass_wd,authority) VALUES (%d, '%s','%s', %d);",
            id, name ? name : "NULL", pass_wd?pass_wd:"NULL",authority);


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
int DeleteBook(int id){
    char ID[50];
    sprintf(ID, "%d", id);


    char sql[256];
    strcpy(sql, "DELETE FROM book WHERE id= ");
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
int DeleteUser(int id){
    char ID[50];
    sprintf(ID, "%d", id);


    char sql[256];
    strcpy(sql, "DELETE FROM user WHERE id= ");
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
int UpdateUserAuthority(int id,int authority){
    char ID[50];
    sprintf(ID, "%d", id);
    char a[50];
    sprintf(a, "%d", authority);


    char sql[256];
    strcpy(sql, "UPDATE user SET authority = ");
    strcat(sql, a);
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
MYSQL_ROWS CheckUserBorrow(int id){
    char ID[50];
    sprintf(ID, "%d", id);
    char sql[256];
    strcpy(sql, "SELECT * FROM borrow WHERE user_id = ");
    strcat(sql, ID);
    strcat(sql, ";");

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(conn));
        MYSQL_ROWS s;
        s.data=NULL;
        s.next=NULL;
        return  s;
    }
    MYSQL_ROWS* head = NULL;
    MYSQL_ROWS* left = NULL;
    MYSQL_ROWS* right = NULL;

    // 获取查询结果
    MYSQL_RES *res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_use_result() failed. Error: %s\n", mysql_error(conn));
        MYSQL_ROWS s;
        s.data = NULL;
        s.next = NULL;
        return s;
    }

    // 逐行处理数据
    MYSQL_ROW row1 = mysql_fetch_row(res);
    if (row1) {
        head = left = malloc(sizeof(MYSQL_ROWS));
        if (head == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            mysql_free_result(res);
            MYSQL_ROWS s;
            s.data = NULL;
            s.next = NULL;
            return s;
        }

        // 复制第一行的数据
        head->data = malloc(sizeof(char*) * mysql_num_fields(res));
        for (unsigned int i = 0; i < mysql_num_fields(res); ++i) {
            head->data[i] = row1[i] ? strdup(row1[i]) : NULL;
        }
        head->next = NULL;

        left = head;

        // 继续处理剩下的行
        while ((row1 = mysql_fetch_row(res)) != NULL) {
            right = malloc(sizeof(MYSQL_ROWS));
            if (right == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                mysql_free_result(res);
                return *head;  // 返回已经创建的链表
            }

            // 复制当前行的数据
            right->data = malloc(sizeof(char*) * mysql_num_fields(res));
            for (unsigned int i = 0; i < mysql_num_fields(res); ++i) {
                right->data[i] = row1[i] ? strdup(row1[i]) : NULL;
            }

            // 链接到上一个节点
            left->next = right;
            left = right;
            right->next = NULL;
        }
    }

    mysql_free_result(res);  // 释放结果集

    return *head;
}

MYSQL_ROWS CheckAllBorrow() {
    char sql[256];
    strcpy(sql, "SELECT * FROM borrow;");

    // 执行查询
    if (mysql_query(conn, sql)) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(conn));
        MYSQL_ROWS s;
        s.data = NULL;
        s.next = NULL;
        return s;
    }

    MYSQL_ROWS* head = NULL;
    MYSQL_ROWS* left = NULL;
    MYSQL_ROWS* right = NULL;

    // 获取查询结果
    MYSQL_RES *res = mysql_use_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_use_result() failed. Error: %s\n", mysql_error(conn));
        MYSQL_ROWS s;
        s.data = NULL;
        s.next = NULL;
        return s;
    }

    // 逐行处理数据
    MYSQL_ROW row1 = mysql_fetch_row(res);
    if (row1) {
        head = left = malloc(sizeof(MYSQL_ROWS));
        if (head == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            mysql_free_result(res);
            MYSQL_ROWS s;
            s.data = NULL;
            s.next = NULL;
            return s;
        }

        // 复制第一行的数据
        head->data = malloc(sizeof(char*) * mysql_num_fields(res));
        for (unsigned int i = 0; i < mysql_num_fields(res); ++i) {
            head->data[i] = row1[i] ? strdup(row1[i]) : NULL;
        }
        head->next = NULL;

        left = head;

        // 继续处理剩下的行
        while ((row1 = mysql_fetch_row(res)) != NULL) {
            right = malloc(sizeof(MYSQL_ROWS));
            if (right == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                mysql_free_result(res);
                return *head;  // 返回已经创建的链表
            }

            // 复制当前行的数据
            right->data = malloc(sizeof(char*) * mysql_num_fields(res));
            for (unsigned int i = 0; i < mysql_num_fields(res); ++i) {
                right->data[i] = row1[i] ? strdup(row1[i]) : NULL;
            }

            // 链接到上一个节点
            left->next = right;
            left = right;
            right->next = NULL;
        }
    }

    mysql_free_result(res);  // 释放结果集

    return *head;
}

MYSQL_ROWS CheckBooksBorrow(int id){
    char ID[50];
    sprintf(ID, "%d", id);
    char sql[256];
    strcpy(sql, "SELECT * FROM borrow WHERE book_id = ");
    strcat(sql, ID);
    strcat(sql, ";");

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(conn));
        MYSQL_ROWS s;
        s.data=NULL;
        s.next=NULL;
        return  s;
    }
    MYSQL_ROWS* head = NULL;
    MYSQL_ROWS* left = NULL;
    MYSQL_ROWS* right = NULL;

    // 获取查询结果
    MYSQL_RES *res = mysql_use_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_use_result() failed. Error: %s\n", mysql_error(conn));
        MYSQL_ROWS s;
        s.data = NULL;
        s.next = NULL;
        return s;
    }

    // 逐行处理数据
    MYSQL_ROW row1 = mysql_fetch_row(res);
    if (row1) {
        head = left = malloc(sizeof(MYSQL_ROWS));
        if (head == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            mysql_free_result(res);
            MYSQL_ROWS s;
            s.data = NULL;
            s.next = NULL;
            return s;
        }

        // 复制第一行的数据
        head->data = malloc(sizeof(char*) * mysql_num_fields(res));
        for (unsigned int i = 0; i < mysql_num_fields(res); ++i) {
            head->data[i] = row1[i] ? strdup(row1[i]) : NULL;
        }
        head->next = NULL;

        left = head;

        // 继续处理剩下的行
        while ((row1 = mysql_fetch_row(res)) != NULL) {
            right = malloc(sizeof(MYSQL_ROWS));
            if (right == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                mysql_free_result(res);
                return *head;  // 返回已经创建的链表
            }

            // 复制当前行的数据
            right->data = malloc(sizeof(char*) * mysql_num_fields(res));
            for (unsigned int i = 0; i < mysql_num_fields(res); ++i) {
                right->data[i] = row1[i] ? strdup(row1[i]) : NULL;
            }

            // 链接到上一个节点
            left->next = right;
            left = right;
            right->next = NULL;
        }
    }

    mysql_free_result(res);  // 释放结果集

    return *head;
}
int RootResetPass(int id,char* pass){
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
int CheckUser(int id){
    char ID[50];
    sprintf(ID, "%d", id);

    char sql[256];
    strcpy(sql, "SELECT * FROM user WHERE id=");
    strcat(sql, ID);
    strcat(sql, ";");

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(conn));
        return -1;
    }
    res= mysql_use_result(conn);
    row= mysql_fetch_row(res);
    if(row!=NULL){
        mysql_free_result(res);
        return 0;
    } else{
        mysql_free_result(res);
        return -1;
    }
}
