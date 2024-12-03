//
// Created by 14403 on 2024/11/29.
//

#ifndef LIBRARYCONTROLLER_ADMINISTRATORSERVICE_H
#define LIBRARYCONTROLLER_ADMINISTRATORSERVICE_H
int addBooks(int id,char* name,char* writer,int last_num,char* position);
int deleteBooks(int id);
int addUser(int id,char* name,char* pass_wd,int authority);
int deleteUser(int id);
int selectUser(int id);
int updateAuthority(int id,int authority);
int checkUserBorrow(int id);
int checkAllBorrow();
int checkBooksBorrow(int id);
char* rootResetPass(int id,char* c1,char* c2);
#endif //LIBRARYCONTROLLER_ADMINISTRATORSERVICE_H
