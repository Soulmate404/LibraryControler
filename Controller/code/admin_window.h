//
// Created by 14403 on 2024/12/14.
//

#include <windows.h>

#ifndef LIBRARY_ADMIN_WINDOW_H
#define LIBRARY_ADMIN_WINDOW_H

// 函数声明
HWND CreateAdminWindow(HINSTANCE hInstance);
LRESULT CALLBACK AdminWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif //LIBRARY_ADMIN_WINDOW_H
