//
// Created by 14403 on 2024/12/3.
//

#include "BookBorrowMapper.h"
#include <stdio.h>
#include <mysql.h>
#include "../code/mysqlPointer.c"

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
MYSQL_ROW SelectByID(int id){
    char ID[50];
    sprintf(ID, "%d", id);

    char sql[256];
    strcpy(sql, "SELECT * FROM book WHERE id=");
    strcat(sql, ID);
    strcat(sql, ";");

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(conn));
        return NULL;
    }
    res= mysql_use_result(conn);
    return mysql_fetch_row(res);
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
    MYSQL_ROWS* left,*right,*head;
    res= mysql_use_result(conn);
    head->data=left->data= mysql_fetch_row(res);
    while(( right->data= mysql_fetch_row(res))){
        left->next= right;
    }
    left->next=NULL;
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
