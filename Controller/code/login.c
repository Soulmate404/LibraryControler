#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../Service/code/userLog.h"
#include "../../Mapper/code/SQLPointer.h"
#include "admin_window.h"
#include "user_window.h"

// ��������

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// �ؼ����
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

    // ��ȡ��Ļ�ߴ�
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // ���ô��ڳߴ�
    int windowWidth = 400;
    int windowHeight = 300;

    // ���㴰�ھ���λ��
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    // �������ڣ�����λ�ò�����
    HWND hwnd = CreateWindowEx(
            0,                          // ��չ��ʽ
            CLASS_NAME,                 // ��������
            "ͼ�����ϵͳ��¼",         // ���ڱ���
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // �޸Ĵ�����ʽ���Ƴ���󻯰�ť
            posX, posY,                // ����λ��
            windowWidth, windowHeight,  // ??�ڴ�С
            NULL,       // �����ھ��
            NULL,       // �˵����
            hInstance,  // ʵ�����
            NULL        // ��������
    );

    if (hwnd == NULL) {
        return 0;
    }

    // �����û��������
    hUsername = CreateWindow(
            "EDIT", "",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            50, 50, 280, 30,
            hwnd, NULL, hInstance, NULL
    );

    // �������������
    hPassword = CreateWindow(
            "EDIT", "",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD,
            50, 100, 280, 30,
            hwnd, NULL, hInstance, NULL
    );

    // ������¼��ť
    hLogin = CreateWindow(
            "BUTTON", "��¼",
            WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            50, 150, 280, 35,
            hwnd, (HMENU)1, hInstance, NULL
    );



    // ��ʾ����
    ShowWindow(hwnd, nCmdShow);

    // ��Ϣѭ��
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
            // ���ô��ڱ���ɫΪ��ɫ
            SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)CreateSolidBrush(RGB(255, 255, 255)));
            return 0;
        }

        case WM_COMMAND: {
            if (LOWORD(wParam) == 1) { // ��¼��ť�����
                char username[100];
                char password[100];

                GetWindowText(hUsername, username, 100);
                GetWindowText(hPassword, password, 100);

                // ���û���ת��Ϊ����ID
                int userId = atoi(username);

                // ����Login����������֤
                int loginResult = login(userId, password);

                switch(loginResult) {
                    case -1:
                        MessageBox(hwnd, "�û������ڣ�", "����", MB_OK | MB_ICONERROR);
                        break;
                    case 0: // ����Ա��¼�ɹ�
                        ShowWindow(hwnd, SW_HIDE);
                        HWND adminWindow = CreateAdminWindow(GetModuleHandle(NULL));
                        if (adminWindow == NULL) {
                            MessageBox(hwnd, "��������Ա����ʧ�ܣ�", "����", MB_OK | MB_ICONERROR);
                            ShowWindow(hwnd, SW_SHOW);
                        }
                        break;
                    case 1: // ��ͨ�û���¼�ɹ�
                        ShowWindow(hwnd, SW_HIDE);
                        HWND userWindow = CreateUserWindow(GetModuleHandle(NULL), userId);
                        if (userWindow == NULL) {
                            MessageBox(hwnd, "�����û�����ʧ�ܣ�", "����", MB_OK | MB_ICONERROR);
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