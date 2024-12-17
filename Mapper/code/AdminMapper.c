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

int AddBooks(int id, char* name, char* writer, int last_num,  char* position) {
    if (conn == NULL) {
        fprintf(stderr, "Database connection is not initialized.\n");
        return -1;
    }


    const char* sql = "INSERT INTO book (id, name, writer, remaining_quantity, position) VALUES (?, ?, ?, ?, ?)";


    stmt = mysql_stmt_init(conn);
    if (stmt == NULL) {
        fprintf(stderr, "mysql_stmt_init() failed\n");
        return -1;
    }


    if (mysql_stmt_prepare(stmt, sql, strlen(sql)) != 0) {
        fprintf(stderr, "mysql_stmt_prepare() failed: %s\n", mysql_error(conn));
        mysql_stmt_close(stmt);
        return -1;
    }


    MYSQL_BIND bind[5];


    memset(bind, 0, sizeof(bind));


    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &id;
    bind[0].is_null = 0;
    bind[0].length = 0;


    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (char*)name;
    bind[1].buffer_length = strlen(name);
    bind[1].is_null = 0;
    bind[1].length = NULL;


    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = (char*)writer;
    bind[2].buffer_length = strlen(writer);
    bind[2].is_null = 0;
    bind[2].length = NULL;


    bind[3].buffer_type = MYSQL_TYPE_LONG;
    bind[3].buffer = &last_num;
    bind[3].is_null = 0;
    bind[3].length = 0;


    bind[4].buffer_type = MYSQL_TYPE_STRING;
    bind[4].buffer = (char*)position;
    bind[4].buffer_length = strlen(position);
    bind[4].is_null = 0;
    bind[4].length = NULL;


    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        fprintf(stderr, "mysql_stmt_bind_param() failed: %s\n", mysql_error(conn));
        mysql_stmt_free_result(stmt);
        return -1;
    }


    if (mysql_stmt_execute(stmt) != 0) {
        fprintf(stderr, "mysql_stmt_execute() failed: %s\n", mysql_error(conn));
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
int AddUser(int id, char* name, char* pass_wd, int authority) {
    if (conn == NULL) {
        fprintf(stderr, "Database connection is not initialized.\n");
        return -1;
    }

    const char* query = "INSERT INTO user (id, name, pass_wd, authority) VALUES (?, ?, ?, ?)";

    // 初始化 statement
    stmt = mysql_stmt_init(conn);
    if (stmt == NULL) {
        fprintf(stderr, "mysql_stmt_init() failed: %s\n", mysql_error(conn));
        return -1;
    }

    // 准备语句
    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        fprintf(stderr, "mysql_stmt_prepare() failed: %s\n", mysql_error(conn));
        mysql_stmt_close(stmt);
        return -1;
    }

    // 初始化绑定结构体
    MYSQL_BIND bind[4];
    memset(bind, 0, sizeof(bind));

    // 设置参数长度
    unsigned long name_length = strlen(name);
    unsigned long pass_length = strlen(pass_wd);

    // 绑定 id
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &id;
    bind[0].is_null = 0;
    bind[0].length = 0;

    // 绑定 name
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = name;
    bind[1].buffer_length = name_length;
    bind[1].length = &name_length;
    bind[1].is_null = 0;

    // 绑定 password
    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = pass_wd;
    bind[2].buffer_length = pass_length;
    bind[2].length = &pass_length;
    bind[2].is_null = 0;

    // 绑定 authority
    bind[3].buffer_type = MYSQL_TYPE_LONG;
    bind[3].buffer = &authority;
    bind[3].is_null = 0;
    bind[3].length = 0;

    // 绑定参数
    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        fprintf(stderr, "mysql_stmt_bind_param() failed: %s\n", mysql_error(conn));
        mysql_stmt_close(stmt);
        return -1;
    }

    // 执行语句
    if (mysql_stmt_execute(stmt) != 0) {
        fprintf(stderr, "mysql_stmt_execute() failed: %s\n", mysql_error(conn));
        mysql_stmt_close(stmt);
        return -1;
    }

    // 检查影响的行数
    if (mysql_stmt_affected_rows(stmt) > 0) {
        mysql_stmt_close(stmt);
        return 0;  // 插入成功
    } else {
        mysql_stmt_close(stmt);
        return -1;  // 插入失败
    }
}
int DeleteBook( int id) {
    if (conn == NULL) {
        fprintf(stderr, "Database connection is not initialized.\n");
        return -1;
    }


    const char *query = "DELETE FROM book WHERE id = ?";


    stmt = mysql_stmt_init(conn);
    if (stmt == NULL) {
        fprintf(stderr, "mysql_stmt_init() failed: %s\n", mysql_error(conn));
        return -1;
    }


    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        fprintf(stderr, "mysql_stmt_prepare() failed: %s\n", mysql_error(conn));
        mysql_stmt_free_result(stmt);
        return -1;
    }


    MYSQL_BIND bind[1];


    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = (char*)&id;
    bind[0].is_null = 0;


    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        fprintf(stderr, "mysql_stmt_bind_param() failed: %s\n", mysql_error(conn));
        mysql_stmt_free_result(stmt);
        return -1;
    }


    if (mysql_stmt_execute(stmt) != 0) {
        fprintf(stderr, "mysql_stmt_execute() failed: %s\n", mysql_error(conn));
        mysql_stmt_free_result(stmt);
        return -1;
    }


    if (mysql_stmt_affected_rows(stmt) > 0) {
        mysql_stmt_free_result(stmt);
        return 0;  // 删除成功
    } else {
        mysql_stmt_free_result(stmt);
        return -1;  // 删除失败
    }
}
int DeleteUser(int id){

    const char *query = "DELETE FROM user WHERE id = ?";

    stmt = mysql_stmt_init(conn);
    if (stmt == NULL) {
        fprintf(stderr, "mysql_stmt_init() failed: %s\n", mysql_error(conn));
        return -1;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        fprintf(stderr, "mysql_stmt_prepare() failed: %s\n", mysql_error(conn));
        mysql_stmt_free_result(stmt);
        return -1;
    }

    MYSQL_BIND bind[1];
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = (char *)&id;
    bind[0].is_null = 0;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        fprintf(stderr, "mysql_stmt_bind_param() failed: %s\n", mysql_error(conn));
        mysql_stmt_free_result(stmt);
        return -1;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        fprintf(stderr, "mysql_stmt_execute() failed: %s\n", mysql_error(conn));
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

int UpdateUserAuthority(int id, int authority){

    const char *query = "UPDATE user SET authority = ? WHERE id = ?";

    stmt = mysql_stmt_init(conn);
    if (stmt == NULL) {
        fprintf(stderr, "mysql_stmt_init() failed: %s\n", mysql_error(conn));
        return -1;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        fprintf(stderr, "mysql_stmt_prepare() failed: %s\n", mysql_error(conn));
        mysql_stmt_free_result(stmt);
        return -1;
    }

    MYSQL_BIND bind[2];
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = (char *)&authority;
    bind[0].is_null = 0;

    bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = (char *)&id;
    bind[1].is_null = 0;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        fprintf(stderr, "mysql_stmt_bind_param() failed: %s\n", mysql_error(conn));
        mysql_stmt_free_result(stmt);
        return -1;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        fprintf(stderr, "mysql_stmt_execute() failed: %s\n", mysql_error(conn));
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
        s.data = NULL;
        s.next = NULL;
        return s;
    }

    MYSQL_ROWS *head = NULL;
    MYSQL_ROWS *left = NULL;
    MYSQL_ROWS *right = NULL;
    MYSQL_ROW row1 = NULL;
    res = mysql_use_result(conn);

    while ((row1 = mysql_fetch_row(res)) != NULL) {
        right = malloc(sizeof(MYSQL_ROWS));
        if (!right) {
            fprintf(stderr, "Memory allocation error\n");
            break;
        }

        // 为行数据分配内存并复制数据
        right->data = malloc(4 * sizeof(char *));
        for(int i = 0; i < 4; i++) {
            if(row1[i]) {
                right->data[i] = strdup(row1[i]);
            } else {
                right->data[i] = NULL;
            }
        }

        right->next = NULL;

        if (head == NULL) {
            head = left = right;
        } else {
            left->next = right;  // 修正链表连接
            left = right;
        }
    }

    mysql_free_result(res);

    if (head == NULL) {
        MYSQL_ROWS s;
        s.data = NULL;
        s.next = NULL;
        return s;
    }

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

    MYSQL_ROWS *head = NULL;
    MYSQL_ROWS *left = NULL;
    MYSQL_ROWS *right = NULL;
    MYSQL_ROW row1 = NULL;
    res = mysql_use_result(conn);

    while ((row1 = mysql_fetch_row(res)) != NULL) {
        right = malloc(sizeof(MYSQL_ROWS));
        if (!right) {
            fprintf(stderr, "Memory allocation error\n");
            break;
        }
        right->data = row1;
        right->next = NULL;

        if (head == NULL) {
            head = left = right;
        } else {
            left->next = right;
            left = right;
        }
    }
    if (head == NULL) {
        MYSQL_ROWS s;
        s.data = NULL;
        s.next = NULL;
        return s;
    }
    mysql_free_result(res);
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
        s.data = NULL;
        s.next = NULL;
        return s;
    }

    MYSQL_ROWS *head = NULL;
    MYSQL_ROWS *left = NULL;
    MYSQL_ROWS *right = NULL;
    MYSQL_ROW row1 = NULL;
    res = mysql_use_result(conn);

    while ((row1 = mysql_fetch_row(res)) != NULL) {
        right = malloc(sizeof(MYSQL_ROWS));
        if (!right) {
            fprintf(stderr, "Memory allocation error\n");
            break;
        }
        
        // 为行数据分配内存并复制数据
        right->data = malloc(4 * sizeof(char *));
        for(int i = 0; i < 4; i++) {
            if(row1[i]) {
                right->data[i] = strdup(row1[i]);
            } else {
                right->data[i] = NULL;
            }
        }
        
        right->next = NULL;

        if (head == NULL) {
            head = left = right;
        } else {
            left->next = right;  // 修正链表连接
            left = right;
        }
    }

    mysql_free_result(res);

    if (head == NULL) {
        MYSQL_ROWS s;
        s.data = NULL;
        s.next = NULL;
        return s;
    }

    return *head;
}
int RootResetPass(int id, char *pass) {
    if (pass == NULL) {
        return -1;
    }

    // 修正 SQL 语句，移除多余的引号
    const char *sql = "UPDATE user SET pass_wd = ? WHERE id = ?";
    
    stmt = mysql_stmt_init(conn);
    if (!stmt) {
        fprintf(stderr, "mysql_stmt_init() failed: %s\n", mysql_error(conn));
        return -1;
    }

    if (mysql_stmt_prepare(stmt, sql, strlen(sql))) {
        fprintf(stderr, "mysql_stmt_prepare() failed: %s\n", mysql_error(conn));
        mysql_stmt_close(stmt);
        return -1;
    }

    // 初始化绑定结构体
    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    // 设置密码长度
    unsigned long pass_length = strlen(pass);

    // 绑定密码参数
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = pass;
    bind[0].buffer_length = pass_length;
    bind[0].length = &pass_length;
    bind[0].is_null = 0;

    // 绑定 ID 参数
    bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = &id;
    bind[1].is_null = 0;
    bind[1].length = 0;

    if (mysql_stmt_bind_param(stmt, bind)) {
        fprintf(stderr, "mysql_stmt_bind_param() failed: %s\n", mysql_error(conn));
        mysql_stmt_close(stmt);
        return -1;
    }

    if (mysql_stmt_execute(stmt)) {
        fprintf(stderr, "mysql_stmt_execute() failed: %s\n", mysql_error(conn));
        mysql_stmt_close(stmt);
        return -1;
    }

    if (mysql_stmt_affected_rows(stmt) > 0) {
        mysql_stmt_close(stmt);
        return 0;
    } else {
        mysql_stmt_close(stmt);
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
