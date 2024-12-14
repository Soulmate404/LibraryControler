//
// Created by 14403 on 2024/12/3.
//

#ifndef LIBRARYCONTROLLER_ADMINSERVICE_H
#define LIBRARYCONTROLLER_ADMINSERVICE_H

#define MAX_RESULT_LENGTH 4096

int addBooks(int id, char* name, char* writer, int last_num, char* position);
int deleteBooks(int id);
int addUser(int id, char* name, char* pass_wd, int authority);
int deleteUser(int id);
int selectUser(int id, char* result);
int updateAuthority(int id, int authority);
int checkUserBorrow(int id, char* result);
int checkBooksBorrow(int id, char* result);
int rootResetPass(int id, char* c1, char* c2);

#endif //LIBRARYCONTROLLER_ADMINSERVICE_H
