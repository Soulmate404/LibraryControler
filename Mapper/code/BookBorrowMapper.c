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

    // 执行查询
    if (mysql_real_query(conn, sql, strlen(sql))) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(conn));
        return NULL;
    }

    // 获取查询结果
    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed. Error: %s\n", mysql_error(conn));
        return NULL;
    }

    // 确保至少有一行数据
    MYSQL_ROW mysqlRow = mysql_fetch_row(res);
    if (mysqlRow == NULL) {
        mysql_free_result(res);  // 不要忘记释放资源
        return NULL;
    }

    // 获取结果集中的字段数量
    size_t num_fields = mysql_num_fields(res);

    // 复制结果到动态分配的内存
    MYSQL_ROW result = malloc(sizeof(char*) * num_fields);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        mysql_free_result(res);
        return NULL;
    }

    // 逐个复制每个字段的内容
    for (size_t i = 0; i < num_fields; ++i) {
        if (mysqlRow[i]) {
            result[i] = strdup(mysqlRow[i]);
        } else {
            result[i] = NULL;
        }
    }

    mysql_free_result(res);  // 确保释放结果集

    return result;
}

MYSQL_ROWS SelectByName(char* name){
    char sql[256];
    strcpy(sql, "SELECT * FROM book WHERE name like ");
    strcat(sql, "'%");
    strcat(sql, name);
    strcat(sql, "%';");

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
    res = mysql_store_result(conn);
    MYSQL_ROW row1 = mysql_fetch_row(res);
    if (row1) {
        head = left = malloc(sizeof(MYSQL_ROWS));
        head->data = row1;
        while ((right = malloc(sizeof(MYSQL_ROWS))) && (right->data = mysql_fetch_row(res))) {
            left->next = right;
            left = right;
        }
        left->next = NULL;
    }
    mysql_free_result(res);
    return *head;
}

int AddBorrow(int userid, char* name, int bookid, char* time) {
    if (conn == NULL) {
        fprintf(stderr, "Database connection is not initialized.\n");
        return -1;
    }


    char sql[512];
    sprintf(sql, "INSERT INTO borrow (user_id, name, book_id, borrow_time) VALUES (%d, '%s', %d, '%s');",
            userid, name ? name : "NULL", bookid, time ? time : "NULL");


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
