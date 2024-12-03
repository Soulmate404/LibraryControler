#include <stdio.h>
#include "../../Mapper/head/userLoginMapper.h"

int login(int id,char* pass_wd) {
    return CheckUser(id,pass_wd);
}

char* resetPass(int id,char* c1,char* c2) {
    if (*c1 == *c2) {
        if (!ResetPass(id,c1)) {
            return c1;
        }else{
            return NULL;
        }

    }else {
        return NULL;
    }
}