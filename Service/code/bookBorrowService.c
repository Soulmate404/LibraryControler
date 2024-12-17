//
// Created by 14403 on 2024/12/3.
//

#include "bookBorrowService.h"
#include <stdio.h>
#include "../../Mapper/code/BookBorrowMapper.h"

int selectByID(int id, char* result) {
    MYSQL_ROW row = SelectByID(id);
    
    if (row == NULL) {
        return -1;
    }

    // ��ս���ַ���
    result[0] = '\0';
    
    // ��ӱ�ͷ��ʹ�ÿո�����Ʊ���Ի�ø��õĶ���Ч��
    strcat(result, "ͼ��ID    ����                ����                ʣ������    λ��\r\n");
    strcat(result, "----------------------------------------------------------------\r\n");
    
    // ƴ�ӽ����ȷ��ÿ���й̶����
    char temp[100];
    sprintf(temp, "%-10s", row[0]); strcat(result, temp);  // ID �п� 10
    sprintf(temp, "%-20s", row[1]); strcat(result, temp);  // �����п� 20
    sprintf(temp, "%-20s", row[2]); strcat(result, temp);  // �����п� 20
    sprintf(temp, "%-12s", row[3]); strcat(result, temp);  // ʣ�������п� 12
    sprintf(temp, "%s", row[4]); strcat(result, temp);     // λ��
    strcat(result, "\r\n");
    
    return 0;
}

int selectByName(char* name, char* result) {
    char *found = strchr(name, ';');
    if(found != NULL) {
        return -1;
    }
    
    MYSQL_RES *mysqlRes = SelectByName(name);
    if(mysql_num_rows(mysqlRes) == 0 || mysql_num_rows(mysqlRes) > 100) {
        mysql_free_result(mysqlRes);
        return -1;
    }
    
    // ��ս���ַ���
    result[0] = '\0';
    
    // ��ӱ�ͷ
    strcat(result, "ͼ��ID    ����                ����                ʣ������    λ��\r\n");
    strcat(result, "----------------------------------------------------------------\r\n");
    
    MYSQL_ROW row;
    unsigned int filed = mysql_num_fields(mysqlRes);
    while((row = mysql_fetch_row(mysqlRes))) {
        char temp[100];
        sprintf(temp, "%-10s", row[0]); strcat(result, temp);
        sprintf(temp, "%-20s", row[1]); strcat(result, temp);
        sprintf(temp, "%-20s", row[2]); strcat(result, temp);
        sprintf(temp, "%-12s", row[3]); strcat(result, temp);
        sprintf(temp, "%s", row[4]); strcat(result, temp);
        strcat(result, "\r\n");
    }
    
    mysql_free_result(mysqlRes);
    return 0;
}

int addBorrow(int userid, char* name, int bookid, char* time) {
    // ���ȼ��ͼ���Ƿ����
    if (CheckBook(bookid) != 0) {
        return 1;  // ͼ�鲻����
    }
    
    // ���ͼ���Ƿ���ʣ��
    int remaining = GetBookNum(bookid);
    if (remaining <= 0) {
        return 2;  // ͼ���ѱ�����
    }
    
    // ����û��Ƿ��Ѿ����Ĺ��Ȿ��
    if (CheckBorrow(userid, bookid) == 0) {
        return 3;  // �û��Ѿ����Ĺ��Ȿ��
    }
    
    // ִ�н������
    return AddBorrow(userid, name, bookid, time);
}

int deleteBorrow(int userid,int bookid) {
    if (CheckBorrow(userid,bookid) == 0) {
        return DeleteBorrow(userid,bookid);
    }else {
        return -1;
    }
}