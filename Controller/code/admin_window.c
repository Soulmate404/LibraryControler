#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../Service/code/adminService.h"
#include "../../Mapper/code/AdminMapper.h"
#include "admin_window.h"

// 全局变量声明
static HWND hBookId, hBookName, hWriter, hLastNum, hPosition;
static HWND hAddBookBtn, hDelBookBtn;
static HWND hAdminUserId, hAdminUserName, hAdminPassword, hAuthority;
static HWND hAddUserBtn, hDelUserBtn, hUpdateAuthBtn;
static HWND hCheckUserBtn, hCheckBookBtn;
static HWND hResetPassId, hNewPass1, hNewPass2, hResetPassBtn;
static HWND hAdminResultText;

// 函数实现
HWND CreateAdminWindow(HINSTANCE hInstance) {
    const char CLASS_NAME[] = "Admin Window Class";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = AdminWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    // 创建窗口（居中显示）
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int windowWidth = 1000;
    int windowHeight = 800;
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    HWND hwnd = CreateWindowEx(
            0,
            CLASS_NAME,
            "图书管理系统 - 管理员界面",
            WS_OVERLAPPEDWINDOW,
            posX, posY, windowWidth, windowHeight,
            NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return NULL;

    // 图书管理部分
    CreateWindow("STATIC", "图书管理", WS_VISIBLE | WS_CHILD,
                 20, 20, 100, 25, hwnd, NULL, hInstance, NULL);

    CreateWindow("STATIC", "图书ID:", WS_VISIBLE | WS_CHILD,
                 20, 50, 60, 25, hwnd, NULL, hInstance, NULL);
    hBookId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                           90, 50, 100, 25, hwnd, NULL, hInstance, NULL);

    CreateWindow("STATIC", "书名:", WS_VISIBLE | WS_CHILD,
                 200, 50, 40, 25, hwnd, NULL, hInstance, NULL);
    hBookName = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                             250, 50, 150, 25, hwnd, NULL, hInstance, NULL);

    CreateWindow("STATIC", "作者:", WS_VISIBLE | WS_CHILD,
                 410, 50, 40, 25, hwnd, NULL, hInstance, NULL);
    hWriter = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                           460, 50, 150, 25, hwnd, NULL, hInstance, NULL);

    CreateWindow("STATIC", "数量:", WS_VISIBLE | WS_CHILD,
                 620, 50, 40, 25, hwnd, NULL, hInstance, NULL);
    hLastNum = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                            670, 50, 60, 25, hwnd, NULL, hInstance, NULL);

    CreateWindow("STATIC", "位置:", WS_VISIBLE | WS_CHILD,
                 740, 50, 40, 25, hwnd, NULL, hInstance, NULL);
    hPosition = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                             790, 50, 100, 25, hwnd, NULL, hInstance, NULL);

    hAddBookBtn = CreateWindow("BUTTON", "添加图书", WS_VISIBLE | WS_CHILD,
                               20, 90, 100, 30, hwnd, (HMENU)1, hInstance, NULL);
    hDelBookBtn = CreateWindow("BUTTON", "删除图书", WS_VISIBLE | WS_CHILD,
                               130, 90, 100, 30, hwnd, (HMENU)2, hInstance, NULL);
    hCheckBookBtn = CreateWindow("BUTTON", "查看借阅情况", WS_VISIBLE | WS_CHILD,
                                 240, 90, 100, 30, hwnd, (HMENU)3, hInstance, NULL);

    // 用户管理部分
    CreateWindow("STATIC", "用户管理", WS_VISIBLE | WS_CHILD,
                 20, 140, 100, 25, hwnd, NULL, hInstance, NULL);

    CreateWindow("STATIC", "用户ID:", WS_VISIBLE | WS_CHILD,
                 20, 170, 60, 25, hwnd, NULL, hInstance, NULL);
    hAdminUserId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                                90, 170, 100, 25, hwnd, NULL, hInstance, NULL);

    CreateWindow("STATIC", "用户名:", WS_VISIBLE | WS_CHILD,
                 200, 170, 50, 25, hwnd, NULL, hInstance, NULL);
    hAdminUserName = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                                  260, 170, 150, 25, hwnd, NULL, hInstance, NULL);

    CreateWindow("STATIC", "密码:", WS_VISIBLE | WS_CHILD,
                 420, 170, 40, 25, hwnd, NULL, hInstance, NULL);
    hAdminPassword = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
                                  470, 170, 150, 25, hwnd, NULL, hInstance, NULL);

    CreateWindow("STATIC", "权限(0/1):", WS_VISIBLE | WS_CHILD,
                 630, 170, 70, 25, hwnd, NULL, hInstance, NULL);
    hAuthority = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                              710, 170, 40, 25, hwnd, NULL, hInstance, NULL);

    hAddUserBtn = CreateWindow("BUTTON", "添加用户", WS_VISIBLE | WS_CHILD,
                               20, 210, 100, 30, hwnd, (HMENU)4, hInstance, NULL);
    hDelUserBtn = CreateWindow("BUTTON", "删除用户", WS_VISIBLE | WS_CHILD,
                               130, 210, 100, 30, hwnd, (HMENU)5, hInstance, NULL);
    hUpdateAuthBtn = CreateWindow("BUTTON", "修改权限", WS_VISIBLE | WS_CHILD,
                                  240, 210, 100, 30, hwnd, (HMENU)6, hInstance, NULL);
    hCheckUserBtn = CreateWindow("BUTTON", "查看用户借阅", WS_VISIBLE | WS_CHILD,
                                 350, 210, 100, 30, hwnd, (HMENU)7, hInstance, NULL);

    // 密码重置部分
    CreateWindow("STATIC", "密码重置", WS_VISIBLE | WS_CHILD,
                 20, 260, 100, 25, hwnd, NULL, hInstance, NULL);

    CreateWindow("STATIC", "用户ID:", WS_VISIBLE | WS_CHILD,
                 20, 290, 60, 25, hwnd, NULL, hInstance, NULL);
    hResetPassId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                                90, 290, 100, 25, hwnd, NULL, hInstance, NULL);

    CreateWindow("STATIC", "新密码:", WS_VISIBLE | WS_CHILD,
                 200, 290, 50, 25, hwnd, NULL, hInstance, NULL);
    hNewPass1 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
                             260, 290, 150, 25, hwnd, NULL, hInstance, NULL);

    CreateWindow("STATIC", "确认密码:", WS_VISIBLE | WS_CHILD,
                 420, 290, 70, 25, hwnd, NULL, hInstance, NULL);
    hNewPass2 = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
                             500, 290, 150, 25, hwnd, NULL, hInstance, NULL);

    hResetPassBtn = CreateWindow("BUTTON", "重置密码", WS_VISIBLE | WS_CHILD,
                                 660, 290, 100, 30, hwnd, (HMENU)8, hInstance, NULL);

    // 结果显示区域
    hAdminResultText = CreateWindow("EDIT", "",
                                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_READONLY | 
                                    WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL,
                                    20, 340, 940, 400, hwnd, NULL, hInstance, NULL);

    HFONT hFont = CreateFont(
        16, 0,
        0, 0,
        FW_NORMAL,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN,
        "Consolas"
    );

    SendMessage(hAdminResultText, WM_SETFONT, (WPARAM)hFont, TRUE);

    ShowWindow(hwnd, SW_SHOW);
    return hwnd;
}

LRESULT CALLBACK AdminWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case 1: // 添加图书
                {
                    char idStr[100], name[100], writer[100], lastNumStr[100], position[100];
                    GetWindowText(hBookId, idStr, 100);
                    GetWindowText(hBookName, name, 100);
                    GetWindowText(hWriter, writer, 100);
                    GetWindowText(hLastNum, lastNumStr, 100);
                    GetWindowText(hPosition, position, 100);

                    if (strlen(idStr) > 0 && strlen(name) > 0 && strlen(lastNumStr) > 0) {
                        int id = atoi(idStr);
                        int lastNum = atoi(lastNumStr);
                        if (addBooks(id, name, writer, lastNum, position) == 0) {
                            MessageBox(hwnd, "添加图书成功！", "提示", MB_OK | MB_ICONINFORMATION);
                        } else {
                            MessageBox(hwnd, "添加图书失败！", "错误", MB_OK | MB_ICONERROR);
                        }
                    }
                    break;
                }

                case 2: // 删除图书
                {
                    char idStr[100];
                    GetWindowText(hBookId, idStr, 100);
                    if (strlen(idStr) > 0) {
                        int id = atoi(idStr);
                        if (deleteBooks(id) == 0) {
                            MessageBox(hwnd, "删除图书成功！", "提示", MB_OK | MB_ICONINFORMATION);
                        } else {
                            MessageBox(hwnd, "删除图书失败！", "错误", MB_OK | MB_ICONERROR);
                        }
                    }
                    break;
                }

                case 3: // 查看图书借阅情况
                {
                    char idStr[100];
                    char result[MAX_RESULT_LENGTH];  // 添加结果缓冲区
                    GetWindowText(hBookId, idStr, 100);
                    if (strlen(idStr) > 0) {
                        int id = atoi(idStr);
                        if (checkBooksBorrow(id, result) == -1) {
                            SetWindowText(hAdminResultText, "未找到该图书的借阅记录\r\n");
                        } else {
                            SetWindowText(hAdminResultText, result);
                        }
                    }
                    break;
                }

                case 4: // 添加用户
                {
                    char idStr[100], name[100], password[100], authorityStr[100];
                    GetWindowText(hAdminUserId, idStr, 100);
                    GetWindowText(hAdminUserName, name, 100);
                    GetWindowText(hAdminPassword, password, 100);
                    GetWindowText(hAuthority, authorityStr, 100);

                    if (strlen(idStr) > 0 && strlen(name) > 0 && strlen(password) > 0 && strlen(authorityStr) > 0) {
                        int id = atoi(idStr);
                        int authority = atoi(authorityStr);
                        if (addUser(id, name, password, authority) == 0) {
                            MessageBox(hwnd, "添加用户成功！", "提示", MB_OK | MB_ICONINFORMATION);
                        } else {
                            MessageBox(hwnd, "添加用户失败！", "错误", MB_OK | MB_ICONERROR);
                        }
                    }
                    break;
                }

                case 5: // 删除用户
                {
                    char idStr[100];
                    GetWindowText(hAdminUserId, idStr, 100);
                    if (strlen(idStr) > 0) {
                        int id = atoi(idStr);
                        if (deleteUser(id) == 0) {
                            MessageBox(hwnd, "删除用户成功！", "提示", MB_OK | MB_ICONINFORMATION);
                        } else {
                            MessageBox(hwnd, "删除用户失败！", "错误", MB_OK | MB_ICONERROR);
                        }
                    }
                    break;
                }

                case 6: // 修改权限
                {
                    char idStr[100], authorityStr[100];
                    GetWindowText(hAdminUserId, idStr, 100);
                    GetWindowText(hAuthority, authorityStr, 100);
                    if (strlen(idStr) > 0 && strlen(authorityStr) > 0) {
                        int id = atoi(idStr);
                        int authority = atoi(authorityStr);
                        int result = updateAuthority(id, authority);
                        switch(result) {
                            case 0:
                                MessageBox(hwnd, "修改权限成功！", "提示", MB_OK | MB_ICONINFORMATION);
                                break;
                            case 1:
                                MessageBox(hwnd, "用户ID错误！", "错误", MB_OK | MB_ICONERROR);
                                break;
                            case 2:
                                MessageBox(hwnd, "权限值错误！", "错误", MB_OK | MB_ICONERROR);
                                break;
                        }
                    }
                    break;
                }

                case 7: // 查看用户借阅
                {
                    char idStr[100];
                    char result[MAX_RESULT_LENGTH];  // 添加结果缓冲区
                    GetWindowText(hAdminUserId, idStr, 100);
                    if (strlen(idStr) > 0) {
                        int id = atoi(idStr);
                        if (checkUserBorrow(id, result) == -1) {
                            SetWindowText(hAdminResultText, "未找到该用户的借阅记录\r\n");
                        } else {
                            SetWindowText(hAdminResultText, result);
                        }
                    }
                    break;
                }

                case 8: // 重置密码
                {
                    char idStr[100], pass1[100], pass2[100];
                    GetWindowText(hResetPassId, idStr, 100);
                    GetWindowText(hNewPass1, pass1, 100);
                    GetWindowText(hNewPass2, pass2, 100);

                    if (strlen(idStr) > 0 && strlen(pass1) > 0 && strlen(pass2) > 0) {
                        int id = atoi(idStr);
                        if(strcmp(pass1, pass2) != 0) {
                            MessageBox(hwnd, "两次输入的密码不一致！", "错误", MB_OK | MB_ICONERROR);
                        } else {
                            int result = RootResetPass(id, pass1);
                            if (result == -1) {
                                MessageBox(hwnd, "重置密码失败！", "错误", MB_OK | MB_ICONERROR);
                            } else {
                                MessageBox(hwnd, "密码重置成功！", "提示", MB_OK | MB_ICONINFORMATION);
                            }
                        }
                    }
                    break;
                }
            }
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}