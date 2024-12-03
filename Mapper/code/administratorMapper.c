#include <mysql.h>
#include <stdio.h>
#include "../head/mysqlPointer.h"


MYSQL_ROW SelectUser(int id){
    char ID[50];
    sprintf(ID, "%d", id);

    char sql[256];
    strcpy(sql, "SELECT * FROM user WHERE id=");
    strcat(sql, ID);
    strcat(sql, ";");

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(conn));
        return NULL;
    }
    res= mysql_use_result(conn);
    return mysql_fetch_row(res);
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
    MYSQL_ROWS *left,*right,*head;
    res= mysql_use_result(conn);
    head->data=left->data= mysql_fetch_row(res);
    while(( right->data= mysql_fetch_row(res))){
        left->next= right;
    }
    left->next=NULL;
    return *head;
}
MYSQL_ROWS CheckAllBorrow(){

    char sql[256];
    strcpy(sql, "SELECT * FROM borrow;");
    if (mysql_query(conn, sql)) {
        fprintf(stderr, "SQL error: %s\n", mysql_error(conn));
        MYSQL_ROWS s;
        s.data=NULL;
        s.next=NULL;
        return  s;
    }
    MYSQL_ROWS *left,*right,*head;
    res= mysql_use_result(conn);
    head->data=left->data= mysql_fetch_row(res);
    while(( right->data= mysql_fetch_row(res))){
        left->next= right;
    }
    left->next=NULL;
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
    MYSQL_ROWS *left,*right,*head;
    res= mysql_use_result(conn);
    head->data=left->data= mysql_fetch_row(res);
    while(( right->data= mysql_fetch_row(res))){
        left->next= right;
    }
    left->next=NULL;
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
