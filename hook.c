#include <windows.h>

#ifndef WDA_EXCLUDEFROMCAPTURE
#define WDA_EXCLUDEFROMCAPTURE 0x00000011
#endif
#ifndef WDA_MONITOR
#define WDA_MONITOR 0x00000001
#endif
#ifndef WDA_NONE
#define WDA_NONE 0x00000000
#endif

#define WM_GHOST_HIDE (WM_APP + 4040)
#define WM_GHOST_UNHIDE (WM_APP + 4041)

LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        CWPSTRUCT* cwp = (CWPSTRUCT*)lParam;
        
        if (cwp->message == WM_GHOST_HIDE) {
            MessageBeep(MB_ICONINFORMATION);
            
            BOOL res = SetWindowDisplayAffinity(cwp->hwnd, WDA_EXCLUDEFROMCAPTURE);
            if (!res) {
                SetWindowDisplayAffinity(cwp->hwnd, WDA_MONITOR);
            }
        } else if (cwp->message == WM_GHOST_UNHIDE) {
            MessageBeep(MB_ICONASTERISK); // Different beep for unhide
            
            // Revert back to normal display
            SetWindowDisplayAffinity(cwp->hwnd, WDA_NONE);
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

__declspec(dllexport) BOOL SetGhostHook(HWND hwnd, BOOL hide) {
    if (hwnd == NULL) return FALSE;
    
    DWORD pid = 0;
    DWORD tid = GetWindowThreadProcessId(hwnd, &pid);
    if (tid == 0) return FALSE;
    
    HINSTANCE hInst = GetModuleHandle("hook.dll");
    
    HHOOK hHook = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, hInst, tid);
    
    if (hHook != NULL) {
        SendMessage(hwnd, hide ? WM_GHOST_HIDE : WM_GHOST_UNHIDE, 0, 0);
        UnhookWindowsHookEx(hHook);
        return TRUE;
    }
    return FALSE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    return TRUE;
}