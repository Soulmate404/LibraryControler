//
// Created by 14403 on 2024/12/14.
//

#ifndef LIBRARY_USER_WINDOW_H
#define LIBRARY_USER_WINDOW_H

#include <windows.h>

// 函数声明
HWND CreateUserWindow(HINSTANCE hInstance, int userId);
LRESULT CALLBACK UserWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif //LIBRARY_USER_WINDOW_H
