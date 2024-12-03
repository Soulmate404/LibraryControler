//
// Created by 14403 on 2024/12/3.
//

#ifndef LIBRARYCONTROLLER_BOOKBORROWSERVICE_H
#define LIBRARYCONTROLLER_BOOKBORROWSERVICE_H
int selectByID(int id);
int selectByName(char* name);
int addBorrow(int userid,char* name,int bookid,char* time);
int deleteBorrow(int userid,int bookid);
#endif //LIBRARYCONTROLLER_BOOKBORROWSERVICE_H
