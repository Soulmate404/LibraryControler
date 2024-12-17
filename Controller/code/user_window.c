//
// Created by 14403 on 2024/12/14.
//
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../Service/code/bookBorrowService.h"
#include "user_window.h"

// ȫ�ֱ�������
static HWND hSearchId, hSearchName, hSearchBtn;
static HWND hBorrowId, hBorrowBtn, hReturnBtn;
static HWND hResultText;
static int currentUserId; // �洢��ǰ��¼�û���ID

// ����ʵ��
HWND CreateUserWindow(HINSTANCE hInstance, int userId) {
    currentUserId = userId;

    // ע�ᴰ����
    const char CLASS_NAME[] = "User Window Class";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = UserWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    // �������ڣ�������ʾ��
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int windowWidth = 800;
    int windowHeight = 600;
    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    HWND hwnd = CreateWindowEx(
            0,
            CLASS_NAME,
            "ͼ�����ϵͳ - �û�����",
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

    // �����ؼ�
    // ��ѯ����
    CreateWindow("STATIC", "��ID��ѯ:", WS_VISIBLE | WS_CHILD,
                 20, 20, 80, 25, hwnd, NULL, hInstance, NULL);

    hSearchId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                             100, 20, 100, 25, hwnd, NULL, hInstance, NULL);

    CreateWindow("STATIC", "��������ѯ:", WS_VISIBLE | WS_CHILD,
                 220, 20, 80, 25, hwnd, NULL, hInstance, NULL);

    hSearchName = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                               300, 20, 200, 25, hwnd, NULL, hInstance, NULL);

    hSearchBtn = CreateWindow("BUTTON", "��ѯ", WS_VISIBLE | WS_CHILD,
                              520, 20, 80, 25, hwnd, (HMENU)1, hInstance, NULL);

    // �軹�鲿��
    CreateWindow("STATIC", "ͼ��ID:", WS_VISIBLE | WS_CHILD,
                 20, 60, 80, 25, hwnd, NULL, hInstance, NULL);

    hBorrowId = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                             100, 60, 100, 25, hwnd, NULL, hInstance, NULL);

    hBorrowBtn = CreateWindow("BUTTON", "����", WS_VISIBLE | WS_CHILD,
                              220, 60, 80, 25, hwnd, (HMENU)2, hInstance, NULL);

    hReturnBtn = CreateWindow("BUTTON", "����", WS_VISIBLE | WS_CHILD,
                              320, 60, 80, 25, hwnd, (HMENU)3, hInstance, NULL);

    // �������壬���ڱ�����ʾ���������
    HFONT hFont = CreateFont(
        16, 0,                    // �߶�16, ����Զ�
        0, 0,                     // �ı���б����ת�Ƕ�
        FW_NORMAL,               // �����ϸ
        FALSE, FALSE, FALSE,     // б�塢�»��ߡ�ɾ����
        DEFAULT_CHARSET,         // ʹ��Ĭ���ַ���
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        FIXED_PITCH | FF_MODERN, // ʹ�õȿ�����
        "Consolas"              // ʹ�� Consolas �ȿ�����
    );

    hResultText = CreateWindow("EDIT", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_READONLY | 
        WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL,
        20, 100, 740, 440,
        hwnd, NULL, hInstance, NULL
    );

    // ��������
    SendMessage(hResultText, WM_SETFONT, (WPARAM)hFont, TRUE);

    ShowWindow(hwnd, SW_SHOW);
    return hwnd;
}

LRESULT CALLBACK UserWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case 1: // ��ѯ��ť
                {
                    char idStr[100], nameStr[100], result[MAX_RESULT_LENGTH] = "";
                    GetWindowText(hSearchId, idStr, 100);
                    GetWindowText(hSearchName, nameStr, 100);

                    if (strlen(idStr) == 0 && strlen(nameStr) == 0) {
                        MessageBox(hwnd, "�������ѯ������", "��ʾ", MB_OK | MB_ICONINFORMATION);
                        break;
                    }

                    if (strlen(idStr) > 0) {
                        int bookId = atoi(idStr);
                        if (selectByID(bookId, result) == -1) {
                            SetWindowText(hResultText, "δ�ҵ���Ӧͼ��\r\n");
                        } else {
                            SetWindowText(hResultText, result);
                        }
                    }
                    else if (strlen(nameStr) > 0) {
                        if (selectByName(nameStr, result) == -1) {
                            SetWindowText(hResultText, "δ�ҵ���Ӧͼ��\r\n");
                        } else {
                            SetWindowText(hResultText, result);
                        }
                    }
                    break;
                }

                case 2: // ���鰴ť
                {
                    char bookIdStr[100];
                    GetWindowText(hBorrowId, bookIdStr, 100);
                    if (strlen(bookIdStr) > 0) {
                        int bookId = atoi(bookIdStr);
                        // ��ȡ��ǰʱ��
                        time_t now;
                        time(&now);
                        char timeStr[100];
                        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d", localtime(&now));

                        int result = addBorrow(currentUserId, NULL, bookId, timeStr);
                        switch(result) {
                            case 0:
                                MessageBox(hwnd, "����ɹ���", "��ʾ", MB_OK | MB_ICONINFORMATION);
                                break;
                            case 1:
                                MessageBox(hwnd, "���鲻���ڣ�", "����", MB_OK | MB_ICONERROR);
                                break;
                            case 2:
                                MessageBox(hwnd, "�����ѱ����꣡", "����", MB_OK | MB_ICONERROR);
                                break;
                            case 3:
                                MessageBox(hwnd, "���Ѿ����Ĺ��Ȿ�飡", "����", MB_OK | MB_ICONERROR);
                                break;
                            default:
                                MessageBox(hwnd, "����ʧ�ܣ�", "����", MB_OK | MB_ICONERROR);
                                break;
                        }
                    } else {
                        MessageBox(hwnd, "������ͼ��ID��", "��ʾ", MB_OK | MB_ICONINFORMATION);
                    }
                    break;
                }

                case 3: // ���鰴ť
                {
                    char bookIdStr[100];
                    GetWindowText(hBorrowId, bookIdStr, 100);
                    if (strlen(bookIdStr) > 0) {
                        int bookId = atoi(bookIdStr);
                        if (deleteBorrow(currentUserId, bookId) == 0) {
                            MessageBox(hwnd, "����ɹ���", "��ʾ", MB_OK | MB_ICONINFORMATION);
                        } else {
                            MessageBox(hwnd, "����ʧ�ܣ���ȷ���Ƿ���Ĺ����顣", "����", MB_OK | MB_ICONERROR);
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