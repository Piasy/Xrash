// Win32Example.cpp
//

#include <windows.h>
#include <Winbase.h>

#include <DbgHelp.h>

#include <winuser.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "Resource.h"

#include <cstdint>
#include <cinttypes>
#include <iostream>
#include <ctime>
#include <sstream>
#include <fstream>
#include <chrono>
#include <vector>
#include <map>
#include <algorithm>

#include <crash.h>

using namespace std;
using namespace std::chrono;

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

HWND g_hwnd;
HWND g_crash_button;

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

#pragma comment(lib, "DbgHelp")

LONG WINAPI
unhandled_exception_filter(struct _EXCEPTION_POINTERS* exception_point) {
    char buf[1024];
    int64_t now =
        duration_cast<milliseconds>(system_clock::now().time_since_epoch())
            .count();
    snprintf(buf, 1023, "Crash-Win32Example-%" PRId64 ".dmp", now);
    string dumpName(buf);
    wstring wDumpName(dumpName.begin(), dumpName.end());
    HANDLE dump_file = CreateFile(wDumpName.c_str(), GENERIC_WRITE, 0, NULL,
                                  CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (dump_file) {
        MINIDUMP_EXCEPTION_INFORMATION dump_exception;

        dump_exception.ExceptionPointers = exception_point;
        dump_exception.ThreadId = GetCurrentThreadId();
        dump_exception.ClientPointers = TRUE;

        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), dump_file,
                          MiniDumpNormal, &dump_exception, NULL, NULL);

        CloseHandle(dump_file);
        dump_file = NULL;
    }

    return EXCEPTION_EXECUTE_HANDLER;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    SetUnhandledExceptionFilter(unhandled_exception_filter);

#if 0
    AllocConsole();
    freopen("conout$", "w", stdout);
    freopen("conout$", "w", stderr);
#endif

    // global init
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_IROOMWINDOWSNATIVEDEMO, szWindowClass,
                MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    wchar_t moduleBuf[1000];
    GetModuleFileName(NULL, moduleBuf, 999);
    wstring moduleStr(moduleBuf);
    size_t idx = moduleStr.find_last_of(L"/\\");
    wstring modulePath = moduleStr.substr(0, idx);

    // program init
    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(
        hInstance, MAKEINTRESOURCE(IDC_IROOMWINDOWSNATIVEDEMO));

    MSG msg;

    // main msg loop
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

//
//  register window
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon =
        LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IROOMWINDOWSNATIVEDEMO));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_IROOMWINDOWSNATIVEDEMO);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   create window
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance;  // save handle

    g_hwnd = CreateWindowEx(
        NULL, szWindowClass, szTitle,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN, 0, 0, 0, 0,
        nullptr, nullptr, hInstance, nullptr);

    if (!g_hwnd) {
        return FALSE;
    }
	SetWindowPos(g_hwnd, HWND_NOTOPMOST, 100, 100, 960, 540, SWP_NOMOVE);
    ShowWindow(g_hwnd, nCmdShow);
    UpdateWindow(g_hwnd);

    return TRUE;
}

//
//  handle msg
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
    switch (message) {
        case WM_CREATE: {
			g_crash_button = CreateWindow(
                TEXT("button"), TEXT("crash"), WS_CHILD | WS_VISIBLE,
                100, 20, 75, 20, hWnd, (HMENU)IDB_BUTTON_CRASH,
                ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            break;
        }
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            // menu
            switch (wmId) {
                case IDM_ABOUT:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd,
                              About);
                    break;
                case IDM_EXIT:
                    break;
                case IDB_BUTTON_CRASH: {
					testCrash();
                    break;
                }
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
			break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO draw
            EndPaint(hWnd, &ps);
			break;
        }
        case WM_CLOSE: {
			return DefWindowProc(hWnd, message, wParam, lParam);
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}
