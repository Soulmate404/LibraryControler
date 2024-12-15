//
// Created by 14403 on 2024/12/14.
//
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../Service/code/bookBorrowService.h"
#include "user_window.h"

// 全局变量声明
static HWND hSearchId, hSearchName, hSearchBtn;
static HWND hBorrowId, hBorrowBtn, hReturnBtn;
static HWND hResultText;
static int currentUserId; // 存储当前登录用户的ID

// 函数实现
HWND CreateUserWindow(HINSTANCE hInstance, int userId) {
    currentUserId = userId;

    // 注册窗口类
    const char CLASS_NAME[] = "User Window Class";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = UserWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    // 创建窗口（居中显示）
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int windowWidth = 800;
    int windowHeight = 600;
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    HWND hwnd = CreateWindowEx(
            0,
            CLASS_NAME,
            "图书管理系统 - 用户界面",
            WS_OVERLAPPEDWINDOW,
            posX, posY, windowWidth, windowHeight,
            NULL,
            NULL,
            hInstance,
            NULL
    );

    if (hwnd == NULL) {
        return NULL;
    }

    // 创建控件
    // 查询部分
    CreateWindow("STATIC", "按ID查询:", WS_VISIBLE | WS_CHILD,
                 20, 20, 80, 25, hwnd, NULL, hInstance, NULL);

    hSearchId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                             100, 20, 100, 25, hwnd, NULL, hInstance, NULL);

    CreateWindow("STATIC", "按书名查询:", WS_VISIBLE | WS_CHILD,
                 220, 20, 80, 25, hwnd, NULL, hInstance, NULL);

    hSearchName = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                               300, 20, 200, 25, hwnd, NULL, hInstance, NULL);

    hSearchBtn = CreateWindow("BUTTON", "查询", WS_VISIBLE | WS_CHILD,
                              520, 20, 80, 25, hwnd, (HMENU)1, hInstance, NULL);

    // 借还书部分
    CreateWindow("STATIC", "图书ID:", WS_VISIBLE | WS_CHILD,
                 20, 60, 80, 25, hwnd, NULL, hInstance, NULL);

    hBorrowId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                             100, 60, 100, 25, hwnd, NULL, hInstance, NULL);

    hBorrowBtn = CreateWindow("BUTTON", "借书", WS_VISIBLE | WS_CHILD,
                              220, 60, 80, 25, hwnd, (HMENU)2, hInstance, NULL);

    hReturnBtn = CreateWindow("BUTTON", "还书", WS_VISIBLE | WS_CHILD,
                              320, 60, 80, 25, hwnd, (HMENU)3, hInstance, NULL);

    // 创建字体，用于避免显示区域的乱码
    HFONT hFont = CreateFont(
        16, 0,                    // 高度16, 宽度自动
        0, 0,                     // 文本倾斜和旋转角度
        FW_NORMAL,               // 字体粗细
        FALSE, FALSE, FALSE,     // 斜体、下划线、删除线
        DEFAULT_CHARSET,         // 使用默认字符集
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN, // 使用等宽字体
        "Consolas"              // 使用 Consolas 等宽字体
    );

    hResultText = CreateWindow("EDIT", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_READONLY | 
        WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL,
        20, 100, 740, 440,
        hwnd, NULL, hInstance, NULL
    );

    // 设置字体
    SendMessage(hResultText, WM_SETFONT, (WPARAM)hFont, TRUE);

    ShowWindow(hwnd, SW_SHOW);
    return hwnd;
}

LRESULT CALLBACK UserWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case 1: // 查询按钮
                {
                    char idStr[100], nameStr[100], result[MAX_RESULT_LENGTH] = "";
                    GetWindowText(hSearchId, idStr, 100);
                    GetWindowText(hSearchName, nameStr, 100);

                    if (strlen(idStr) == 0 && strlen(nameStr) == 0) {
                        MessageBox(hwnd, "请输入查询条件！", "提示", MB_OK | MB_ICONINFORMATION);
                        break;
                    }

                    if (strlen(idStr) > 0) {
                        int bookId = atoi(idStr);
                        if (selectByID(bookId, result) == -1) {
                            SetWindowText(hResultText, "未找到对应图书\r\n");
                        } else {
                            SetWindowText(hResultText, result);
                        }
                    }
                    else if (strlen(nameStr) > 0) {
                        if (selectByName(nameStr, result) == -1) {
                            SetWindowText(hResultText, "未找到对应图书\r\n");
                        } else {
                            SetWindowText(hResultText, result);
                        }
                    }
                    break;
                }

                case 2: // 借书按钮
                {
                    char bookIdStr[100];
                    GetWindowText(hBorrowId, bookIdStr, 100);
                    if (strlen(bookIdStr) > 0) {
                        int bookId = atoi(bookIdStr);
                        // 获取当前时间
                        time_t now;
                        time(&now);
                        char timeStr[100];
                        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d", localtime(&now));

                        int result = addBorrow(currentUserId, NULL, bookId, timeStr);
                        switch(result) {
                            case 0:
                                MessageBox(hwnd, "借书成功！", "提示", MB_OK | MB_ICONINFORMATION);
                                break;
                            case 1:
                                MessageBox(hwnd, "该书不存在！", "错误", MB_OK | MB_ICONERROR);
                                break;
                            case 2:
                                MessageBox(hwnd, "该书已被借完！", "错误", MB_OK | MB_ICONERROR);
                                break;
                            case 3:
                                MessageBox(hwnd, "您已经借阅过这本书！", "错误", MB_OK | MB_ICONERROR);
                                break;
                            default:
                                MessageBox(hwnd, "借书失败！", "错误", MB_OK | MB_ICONERROR);
                                break;
                        }
                    } else {
                        MessageBox(hwnd, "请输入图书ID！", "提示", MB_OK | MB_ICONINFORMATION);
                    }
                    break;
                }

                case 3: // 还书按钮
                {
                    char bookIdStr[100];
                    GetWindowText(hBorrowId, bookIdStr, 100);
                    if (strlen(bookIdStr) > 0) {
                        int bookId = atoi(bookIdStr);
                        if (deleteBorrow(currentUserId, bookId) == 0) {
                            MessageBox(hwnd, "还书成功！", "提示", MB_OK | MB_ICONINFORMATION);
                        } else {
                            MessageBox(hwnd, "还书失败！请确认是否借阅过此书。", "错误", MB_OK | MB_ICONERROR);
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