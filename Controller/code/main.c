#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../../Service/code/adminService.h"
#include "../../Service/code/userLog.h"
#include "../../Service/code/bookBorrowService.h"
#include "../../Mapper/code/SQLPointer.h"


// 欢迎页面
void welcome_page() {
    printf("欢迎来到图书馆管理系统\n");
}

// 获取当前时间字符串
void get_current_time(char *time_str) {
    time_t now;
    struct tm *tm_info;
    time(&now);
    tm_info = localtime(&now);
    strftime(time_str, 50, "%Y-%m-%d %H:%M:%S", tm_info);
}

// 处理普通用户菜单
void handle_user_menu(int user_type) {
    int flag;
    while (1) {
        system("cls");
        if (user_type == 1) {
            system("cls");// 普通用户菜单
            printf("欢迎来到图书管理系统(普通用户模式)\n");
            printf("修改密码请输入1\n");
            printf("通过ID码查找书籍请输入2\n");
            printf("通过书名查找书籍请输入3\n");
            printf("借书登记请输入4\n");
            printf("还书登记请输入5\n");
            printf("返回登陆界面-1\n");
            scanf("%d", &flag);
            getchar(); // 清除缓冲区中的换行符

            switch (flag) {
                case -1:
                    return;
                case 1: {
                    system("cls");
                    printf("修改密码\n");
                    int user_id;
                    char old_password[100], new_password[100], confirm_password[100];
                    printf("请输入你的id：");
                    scanf("%d", &user_id);
                    getchar(); // 清除缓冲区中的换行符

                    printf("请输入旧的密码：");
                    fgets(old_password, sizeof(old_password), stdin);
                    old_password[strcspn(old_password, "\n")] = 0;

                    printf("请输入新的密码：");
                    fgets(new_password, sizeof(new_password), stdin);
                    new_password[strcspn(new_password, "\n")] = 0;

                    printf("请再次输入新的密码：");
                    fgets(confirm_password, sizeof(confirm_password), stdin);
                    confirm_password[strcspn(confirm_password, "\n")] = 0;

                    if (strcmp(new_password, confirm_password) != 0) {
                        printf("两次输入的新密码不一致，请重新输入。\n");
                        break;
                    }

                    if (resetPass(user_id, new_password,confirm_password) ==0) {
                        printf("密码修改成功。\n");
                    } else {
                        printf("密码修改失败。\n");
                    }
                    printf("按回车键以继续");
                    getchar();
                    break;
                }
                case 2: {

                    int book_ID;
                    printf("请输入图书的ID码：");
                    scanf("%d", &book_ID);
                    getchar(); // 清除缓冲区中的换行符

                    if (selectByID(book_ID) == -1) {
                        printf("该ID对应的书不存在。\n");
                    } else {
                        int result = selectByID(book_ID);
                        if (result ==0) {
                            printf("找到书籍信息\n");

                        }
                    }
                    printf("按回车键以继续");
                    getchar();
                    break;
                }
                case 3: {
                    char book_name[100];
                    printf("请输入图书的名字：");
                    fgets(book_name, sizeof(book_name), stdin);
                    book_name[strcspn(book_name, "\n")] = 0;

                    int rows = selectByName(book_name);
                    if (rows!=0) {
                        printf("没有找到对应名称的书籍。\n");
                    }
                    printf("按回车键以继续");
                    getchar();
                    break;
                }
                case 4: {
                    int user_id, book_id;
                    char book_name[100];
                    char current_time[50];

                    printf("请输入你的id：");
                    scanf("%d", &user_id);
                    getchar(); // 清除缓冲区中的换行符

                    printf("请输入书名：");
                    fgets(book_name, sizeof(book_name), stdin);
                    book_name[strcspn(book_name, "\n")] = 0;

                    printf("请输入书的ID码：");
                    scanf("%d", &book_id);

                    get_current_time(current_time);
                    int result = addBorrow(user_id, book_name, book_id, current_time);
                    if (result == 0) {
                        printf("借书成功。\n");
                    } else {
                        printf("借书失败。\n");
                    }
                    printf("按回车键以继续");
                    getchar();
                    break;
                }
                case 5: {
                    int user_id, book_id;
                    printf("请输入你的id：");
                    scanf("%d", &user_id);
                    getchar(); // 清除缓冲区中的换行符

                    printf("请输入所借书籍的ID: ");
                    scanf("%d", &book_id);

                    if (deleteBorrow(user_id, book_id) == 0) {
                        printf("还书成功。\n");
                    } else {
                        printf("还书失败。\n");
                    } printf("按回车键以继续");
                    getchar();
                    break;
                }
                default:
                    printf("无效选项，请重新选择。\n");
            }
        }
    }
}

// 处理管理员菜单
void handle_admin_menu() {

    int flag;
    while (1) {
        system("cls");
        printf("欢迎来到图书管理系统（管理员模式）\n");
        printf("修改密码请输入1\n");
        printf("通过ID码查找书籍请输入2\n");
        printf("通过书名查找书籍请输入3\n");
        printf("借书登记请输入4\n");
        printf("还书登记请输入5\n");
        printf("检查书籍是否存在请输入6\n");
        printf("通过ID获取用户信息请输入7\n");
        printf("检查用户是否存在请输入8\n");
        printf("增加书籍请输入9\n");
        printf("增加用户请输入10\n");
        printf("根据输入的id删除书籍请输入11\n");
        printf("根据ID删除用户请输入12\n");
        printf("修改用户权限等级请输入13\n");
        printf("通过ID返回对应用户的借书情况请输入14\n");
        printf("检索全部的借阅请输入15\n");
        printf("通过ID返回对应书籍的借阅情况请输入16\n");
        printf("以管理员权限重置密码请输入17\n");
        printf("返回上一界面请输入-1\n");

        scanf("%d", &flag);
        getchar(); // 清除缓冲区中的换行符

        if (flag == -1) {
            return;
        }

        switch (flag) {
            case 1: {
                int user_id;
                char new_password[100];
                char new_password2[100];
                printf("请输入你的id：");
                scanf("%d", &user_id);
                getchar(); // 清除缓冲区中的换行符

                printf("请输入新的密码：");
                fgets(new_password, sizeof(new_password), stdin);
                new_password[strcspn(new_password, "\n")] = 0;
                printf("请输入新的密码：");
                fgets(new_password2, sizeof(new_password), stdin);
                new_password2[strcspn(new_password2, "\n")] = 0;

                if (resetPass(user_id, new_password,new_password2) == 0) {
                    printf("密码修改成功。\n");
                } else {
                    printf("密码修改失败。\n");
                } printf("按回车键以继续");
                getchar();
                break;
            }
            case 2: {
                int book_ID;
                printf("请输入图书的ID码：");
                scanf("%d", &book_ID);
                getchar(); // 清除缓冲区中的换行符
                int result = selectByID(book_ID);

                if (result == -1) {
                    printf("该ID对应的书不存在。\n");
                } else {

                    if (result ==0) {
                        printf("找到书籍信息：\n");


                    }
                } printf("按回车键以继续");
                getchar();
                break;
            }
            case 3: {
                char book_name[100];
                printf("请输入书名：");
                fgets(book_name, sizeof(book_name), stdin);
                book_name[strcspn(book_name, "\n")] = 0;

                int result = selectByName(book_name);
                if (result==0) {
                    printf("success");

                } else {
                    printf("没有找到该书。\n");
                } printf("按回车键以继续");
                getchar();
                break;
            }

            case 4: {
                int user_id, book_id;
                char borrow_time[20]; // 假定时间字符串长度不超过20
                time_t now;
                struct tm *current_time;
                char book_name[100];

                time(&now);
                current_time = localtime(&now);
                strftime(borrow_time, sizeof(borrow_time), "%Y-%m-%d %H:%M:%S", current_time);

                printf("请输入用户ID：");
                scanf("%d", &user_id);
                getchar(); // 清除缓冲区中的换行符
                printf("请输入书名：");
                fgets(book_name, sizeof(book_name), stdin);
                book_name[strcspn(book_name, "\n")] = 0;

                printf("请输入图书的ID码：");
                scanf("%d", &book_id);
                getchar(); // 清除缓冲区中的换行符

                if (addBorrow(user_id, "", book_id, borrow_time) == 0) {
                    printf("借书登记成功。\n");
                } else {
                    printf("借书登记失败。\n");
                } printf("按回车键以继续");
                getchar();
                break;
            }

            case 5: {
                int user_id, book_id;
                printf("请输入用户ID：");
                scanf("%d", &user_id);
                getchar(); // 清除缓冲区中的换行符

                printf("请输入要归还的图书的ID码：");
                scanf("%d", &book_id);
                getchar(); // 清除缓冲区中的换行符

                if (deleteBorrow(user_id, book_id) == 0) {
                    printf("还书登记成功。\n");
                } else {
                    printf("还书登记失败。\n");
                } printf("按回车键以继续");
                getchar();
                break;
            }
            case 6: {
                int book_ID;
                printf("请输入图书的ID码：");
                scanf("%d", &book_ID);
                getchar(); // 清除缓冲区中的换行符

                if (selectByID(book_ID) == 0) {
                    printf("书籍存在。\n");
                } else {
                    printf("书籍不存在。\n");
                } printf("按回车键以继续");
                getchar();
                break;
            }
            case 7: {
                int user_id;
                printf("请输入用户的ID：");
                scanf("%d", &user_id);
                getchar(); // 清除缓冲区中的换行符

                int result = selectUser(user_id);
                if (result ==0) {

                } else {
                    printf("用户不存在。\n");
                } printf("按回车键以继续");
                getchar();
                break;
            }
            case 8: {
                int user_id;
                printf("请输入用户的ID：");
                scanf("%d", &user_id);
                getchar(); // 清除缓冲区中的换行符

                if (selectByID(user_id) == 0) {
                    printf("用户存在。\n");
                } else {
                    printf("用户不存在。\n");
                } printf("按回车键以继续");
                getchar();
                break;
            }
            case 9: {
                int book_id, last_num;
                char book_name[100], writer[100], position[100];
                printf("请输入书的ID：");
                scanf("%d", &book_id);
                getchar(); // 清除缓冲区中的换行符

                printf("请输入书名：");
                fgets(book_name, sizeof(book_name), stdin);
                book_name[strcspn(book_name, "\n")] = 0;

                printf("请输入作者：");
                fgets(writer, sizeof(writer), stdin);
                writer[strcspn(writer, "\n")] = 0;

                printf("请输入剩余数量：");
                scanf("%d", &last_num);
                getchar(); // 清除缓冲区中的换行符

                printf("请输入位置：");
                fgets(position, sizeof(position), stdin);
                position[strcspn(position, "\n")] = 0;

                if (addBooks(book_id, book_name, writer, last_num, position) == 0) {
                    printf("书籍添加成功。\n");
                } else {
                    printf("书籍添加失败。\n");
                } printf("按回车键以继续");
                getchar();
                break;
            }
            case 10: {
                int user_id, authority;
                char user_name[100], password[100];
                printf("请输入用户ID：");
                scanf("%d", &user_id);
                getchar(); // 清除缓冲区中的换行符

                printf("请输入用户名：");
                fgets(user_name, sizeof(user_name), stdin);
                user_name[strcspn(user_name, "\n")] = 0;

                printf("请输入密码：");
                fgets(password, sizeof(password), stdin);
                password[strcspn(password, "\n")] = 0;

                printf("请输入权限等级：");
                scanf("%d", &authority);
                getchar(); // 清除缓冲区中的换行符

                if (addUser(user_id, user_name, password, authority) == 0) {
                    printf("用户添加成功。\n");
                } else {
                    printf("用户添加失败。\n");
                }
                printf("按回车键以继续");
                getchar();
                break;
            }
            case 11: {
                int book_id;
                printf("请输入要删除的书籍ID：");
                scanf("%d", &book_id);
                getchar(); // 清除缓冲区中的换行符

                if (deleteBooks(book_id) == 0) {
                    printf("书籍删除成功。\n");
                } else {
                    printf("书籍删除失败。\n");
                } printf("按回车键以继续");
                getchar();
                break;
            }
            case 12: {
                int user_id;
                printf("请输入要删除的用户ID：");
                scanf("%d", &user_id);
                getchar(); // 清除缓冲区中的换行符

                if (deleteUser(user_id) == 0) {
                    printf("用户删除成功。\n");
                } else {
                    printf("用户删除失败。\n");
                } printf("按回车键以继续");
                getchar();
                break;
            }
            case 13: {
                int user_id, authority;
                printf("请输入用户ID：");
                scanf("%d", &user_id);
                getchar(); // 清除缓冲区中的换行符

                printf("请输入新的权限等级：");
                scanf("%d", &authority);
                getchar(); // 清除缓冲区中的换行符

                if (updateAuthority(user_id, authority) == 0) {
                    printf("权限等级修改成功。\n");
                } else {
                    printf("权限等级修改失败。\n");
                } printf("按回车键以继续");
                getchar();
                break;
            }
            case 14: {
                int user_id;
                printf("请输入用户的ID：");
                scanf("%d", &user_id);
                getchar(); // 清除缓冲区中的换行符

                int rows = checkUserBorrow(user_id);
                if (rows==0) {

                } else {
                    printf("用户没有借阅记录。\n");
                } printf("按回车键以继续");
                getchar();
                break;
            }
            case 15: {
                int rows = checkUserBorrow(0);
                if (rows ==0) {

                } else {
                    printf("没有借阅记录。\n");
                } printf("按回车键以继续");
                getchar();
                break;
            }
            case 16: {
                int book_id;
                printf("请输入书籍的ID：");
                scanf("%d", &book_id);
                getchar(); // 清除缓冲区中的换行符

                int result = checkBooksBorrow(book_id);
                if (result ==0) {

                } else {
                    printf("书籍没有借阅记录。\n");
                } printf("按回车键以继续");
                getchar();
                break;
            }
            case 17: {
                int user_id;
                char new_password[100];
                char new_password2[100];
                printf("请输入用户的ID：");
                scanf("%d", &user_id);
                getchar(); // 清除缓冲区中的换行符

                printf("请输入新的密码：");
                fgets(new_password, sizeof(new_password), stdin);
                new_password[strcspn(new_password, "\n")] = 0;
                printf("请再次确认密码：");
                fgets(new_password2, sizeof(new_password), stdin);
                new_password2[strcspn(new_password2, "\n")] = 0;

                if (rootResetPass(user_id, new_password,new_password2) == 0) {
                    printf("密码重置成功。\n");
                } else {
                    printf("密码重置失败。\n");
                } printf("按回车键以继续");
                getchar();
                break;
            }
            default:
                printf("无效选项，请重新选择。\n");
        }
    }
}

int main() {
    // MYSQL *conn; // 如果使用MySQL，请取消注释并初始化连接

    system("color F0");
    freeSQLResources();
    init();
    welcome_page();

    while (1) {
        int id;
        char password[100];

        printf("请输入用户名: （如需退出程序请输入-1）");
        scanf("%d", &id);
        getchar(); // 清除缓冲区中的换行符
        if(id==-1){
            break;
        }

        printf("请输入密码：");
        fgets(password, sizeof(password), stdin);
        password[strcspn(password, "\n")] = '\0'; // 移除可能存在的换行符

        int user_whether_exist = login(id, password);

        if (user_whether_exist == -1) {
            printf("该用户不存在或密码错误。\n");
            printf("返回上一界面请输入-1：\n");
            int back;
            scanf("%d", &back);
            getchar(); // 清除缓冲区中的换行符
            if (back == -1) {
                continue;
            }
        } else {
            // 根据用户类型调用相应菜单
            if (user_whether_exist == 0) {
                handle_admin_menu();
            } else if (user_whether_exist == 1) {
                handle_user_menu(user_whether_exist);
            }
        }
    }
    return 0;
}