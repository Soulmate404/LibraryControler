//
// Created by 14403 on 2024/12/3.
//

#include "userLog.h"
#include <stdio.h>
#include "../../Mapper/code/UserLogMapper.h"

int login(int id,char* pass_wd) {
    return CheckUserInLog(id,pass_wd);
}

int resetPass(int id,char* c1,char* c2) {
    if (*c1 == *c2) {
        if (!ResetPass(id,c1)) {
            return 0;
        }else{
            return -1;
        }

    }else {
        return -1;
    }
}