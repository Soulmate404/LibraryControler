#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../Service/code/userLog.h"
#include "../../Mapper/code/SQLPointer.h"
#include "admin_window.h"
#include "user_window.h"

// 函数声明

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 控件句柄
HWND hUsername, hPassword, hLogin, hSignup;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    freeSQLResources();
    init();
    const char CLASS_NAME[] = "Login Window Class";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    // 获取屏幕尺寸
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // 设置窗口尺寸
    int windowWidth = 400;
    int windowHeight = 300;

    // 计算窗口居中位置
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    // 创建窗口（更新位置参数）
    HWND hwnd = CreateWindowEx(
            0,                          // 扩展样式
            CLASS_NAME,                 // 窗口类名
            "图书管理系统登录",         // 窗口标题
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // 修改窗口样式，移除最大化按钮
            posX, posY,                // 居中位置
            windowWidth, windowHeight,  // ��口大小
            NULL,       // 父窗口句柄
            NULL,       // 菜单句柄
            hInstance,  // 实例句柄
            NULL        // 附加数据
    );

    if (hwnd == NULL) {
        return 0;
    }

    // 创建用户名输入框
    hUsername = CreateWindow(
            "EDIT", "",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            50, 50, 280, 30,
            hwnd, NULL, hInstance, NULL
    );

    // 创建密码输入框
    hPassword = CreateWindow(
            "EDIT", "",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
            50, 100, 280, 30,
            hwnd, NULL, hInstance, NULL
    );

    // 创建登录按钮
    hLogin = CreateWindow(
            "BUTTON", "登录",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            50, 150, 280, 35,
            hwnd, (HMENU)1, hInstance, NULL
    );



    // 显示窗口
    ShowWindow(hwnd, nCmdShow);

    // 消息循环
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // 设置窗口背景色为白色
            SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(RGB(255, 255, 255)));
            return 0;
        }

        case WM_COMMAND: {
            if (LOWORD(wParam) == 1) { // 登录按钮被点击
                char username[100];
                char password[100];

                GetWindowText(hUsername, username, 100);
                GetWindowText(hPassword, password, 100);

                // 将用户名转换为整数ID
                int userId = atoi(username);

                // 调用Login函数进行验证
                int loginResult = login(userId, password);

                switch(loginResult) {
                    case -1:
                        MessageBox(hwnd, "用户不存在！", "错误", MB_OK | MB_ICONERROR);
                        break;
                    case 0: // 管理员登录成功
                        ShowWindow(hwnd, SW_HIDE);
                        HWND adminWindow = CreateAdminWindow(GetModuleHandle(NULL));
                        if (adminWindow == NULL) {
                            MessageBox(hwnd, "创建管理员界面失败！", "错误", MB_OK | MB_ICONERROR);
                            ShowWindow(hwnd, SW_SHOW);
                        }
                        break;
                    case 1: // 普通用户登录成功
                        ShowWindow(hwnd, SW_HIDE);
                        HWND userWindow = CreateUserWindow(GetModuleHandle(NULL), userId);
                        if (userWindow == NULL) {
                            MessageBox(hwnd, "创建用户界面失败！", "错误", MB_OK | MB_ICONERROR);
                            ShowWindow(hwnd, SW_SHOW);
                        }
                        break;
                }
            }
            return 0;
        }

        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }


    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
#pragma clang diagnostic pop