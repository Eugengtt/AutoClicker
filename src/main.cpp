#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:WinMainCRTStartup")
#include <windows.h>
#include <string>

#define ID_BUTTON_START 1001
#define ID_TIMER_CLICK 1002
#define ID_TEXTBOX_DELAY 1003
#define ID_COMBO_MOUSE 1004
#define ID_RADIO_INFINITE 1005
#define ID_RADIO_FIXED 1006
#define ID_TEXTBOX_COUNT 1007
#define ID_CHECKBOX_DOUBLE 1008

#define ID_HOTKEY_F3 1

bool isActive = false;
HWND hButtonStart, hTxtDelay, hComboMouse, hRadioInfinite, hRadioFixed, hTxtCount, hCheckDouble;
HWND hLblDelay, hLblMouse, hLblRepeat;

int clickDelay = 10;
int clickType = 0;
bool isInfinite = true;
bool isDoubleClick = false;
int targetClicks = 100;
int clickCounter = 0;

void UpdateRepeatFields(HWND hwnd) {
    isInfinite = IsDlgButtonChecked(hwnd, ID_RADIO_INFINITE) == BST_CHECKED;
    EnableWindow(hTxtCount, !isInfinite);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE: {
            RegisterHotKey(hwnd, ID_HOTKEY_F3, 0, VK_F3);

            hLblDelay = CreateWindow("STATIC", "Click interval (ms):", WS_VISIBLE | WS_CHILD, 20, 20, 130, 20, hwnd, NULL, NULL, NULL);
            hTxtDelay = CreateWindow("EDIT", "10", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 160, 18, 60, 20, hwnd, (HMENU)ID_TEXTBOX_DELAY, NULL, NULL);

            hLblMouse = CreateWindow("STATIC", "Mouse button:", WS_VISIBLE | WS_CHILD, 20, 55, 130, 20, hwnd, NULL, NULL, NULL);
            hComboMouse = CreateWindow("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 160, 52, 90, 100, hwnd, (HMENU)ID_COMBO_MOUSE, NULL, NULL);
            SendMessage(hComboMouse, CB_ADDSTRING, 0, (LPARAM)"Left");
            SendMessage(hComboMouse, CB_ADDSTRING, 0, (LPARAM)"Right");
            SendMessage(hComboMouse, CB_SETCURSEL, 0, 0);

            hCheckDouble = CreateWindow("BUTTON", "Double Click (2 click-uri)", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 160, 80, 180, 20, hwnd, (HMENU)ID_CHECKBOX_DOUBLE, NULL, NULL);

            hLblRepeat = CreateWindow("STATIC", "Click repeat:", WS_VISIBLE | WS_CHILD, 20, 110, 130, 20, hwnd, NULL, NULL, NULL);

            hRadioInfinite = CreateWindow("BUTTON", "Repeat until stopped", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP, 160, 110, 160, 20, hwnd, (HMENU)ID_RADIO_INFINITE, NULL, NULL);
            hRadioFixed = CreateWindow("BUTTON", "Repeat exactly:", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 160, 135, 115, 20, hwnd, (HMENU)ID_RADIO_FIXED, NULL, NULL);

            hTxtCount = CreateWindow("EDIT", "100", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 280, 135, 50, 20, hwnd, (HMENU)ID_TEXTBOX_COUNT, NULL, NULL);

            CheckDlgButton(hwnd, ID_RADIO_INFINITE, BST_CHECKED);
            UpdateRepeatFields(hwnd);

            hButtonStart = CreateWindow("BUTTON", "START (F3)", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 175, 310, 45, hwnd, (HMENU)ID_BUTTON_START, NULL, NULL);

            break;
        }

        case WM_COMMAND: {
            if (LOWORD(wParam) == ID_RADIO_INFINITE || LOWORD(wParam) == ID_RADIO_FIXED) {
                UpdateRepeatFields(hwnd);
            }

            if (LOWORD(wParam) == ID_BUTTON_START) {
                isActive = !isActive;

                if (isActive) {
                    char textDelay[10];
                    GetWindowText(hTxtDelay, textDelay, 10);
                    clickDelay = atoi(textDelay);
                    if (clickDelay <= 0) clickDelay = 1;

                    clickType = SendMessage(hComboMouse, CB_GETCURSEL, 0, 0);
                    isDoubleClick = IsDlgButtonChecked(hwnd, ID_CHECKBOX_DOUBLE) == BST_CHECKED;

                    UpdateRepeatFields(hwnd);
                    if (!isInfinite) {
                        char textCount[10];
                        GetWindowText(hTxtCount, textCount, 10);
                        targetClicks = atoi(textCount);
                        if (targetClicks <= 0) targetClicks = 1;
                    }

                    clickCounter = 0;
                    SetWindowText(hButtonStart, "STOP (F3)");
                    SetTimer(hwnd, ID_TIMER_CLICK, clickDelay, NULL);
                } else {
                    SetWindowText(hButtonStart, "START (F3)");
                    KillTimer(hwnd, ID_TIMER_CLICK);
                }
            }
            break;
        }

        case WM_HOTKEY: {
            if (wParam == ID_HOTKEY_F3) {
                SendMessage(hwnd, WM_COMMAND, ID_BUTTON_START, 0);
            }
            break;
        }

        case WM_TIMER: {
            if (wParam == ID_TIMER_CLICK) {
                if (isActive) {
                    int loops = isDoubleClick ? 2 : 1;

                    INPUT input[2] = {0};
                    input[0].type = INPUT_MOUSE;
                    input[1].type = INPUT_MOUSE;

                    if (clickType == 0) {
                        input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                        input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
                    } else {
                        input[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
                        input[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
                    }

                    for (int i = 0; i < loops; i++) {
                        SendInput(2, input, sizeof(INPUT));
                        if (isDoubleClick && i == 0) {
                            Sleep(1);
                        }
                    }

                    if (!isInfinite) {
                        clickCounter++;
                        if (clickCounter >= targetClicks) {
                            SendMessage(hwnd, WM_COMMAND, ID_BUTTON_START, 0);
                        }
                    }
                }
            }
            break;
        }

        case WM_DESTROY: {
            UnregisterHotKey(hwnd, ID_HOTKEY_F3);
            KillTimer(hwnd, ID_TIMER_CLICK);
            PostQuitMessage(0);
            break;
        }

        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

int main() {
    FreeConsole();
    HINSTANCE hInst = GetModuleHandle(NULL);

    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"AutoClickerV4Clasa";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClassW(&wc)) return -1;

    HWND hwnd = CreateWindowW(
        L"AutoClickerV4Clasa",
        L"Universal Auto-Clicker Pro v4",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        100, 100, 365, 270,
        NULL, NULL, hInst, NULL
    );

    MSG msg = {0};
    while (GetMessage(&msg, NULL, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
