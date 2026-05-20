#include <windows.h>
#include <stdio.h>

typedef BOOL(*SetGhostHookFunc)(HWND, BOOL);

HWND window_list[1024];
int window_count = 0;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    if (IsWindowVisible(hwnd)) {
        int length = GetWindowTextLength(hwnd);
        if (length > 0) {
            char title[512];
            GetWindowTextA(hwnd, title, sizeof(title));
            
            HWND owner = GetWindow(hwnd, GW_OWNER);
            LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
            
            if ((owner == NULL || (exStyle & WS_EX_APPWINDOW)) && !(exStyle & WS_EX_TOOLWINDOW)) {
                window_list[window_count] = hwnd;
                printf("[%d] %s\n", window_count + 1, title);
                window_count++;
                if (window_count >= 1024) return FALSE;
            }
        }
    }
    return TRUE;
}

int main() {
  
    printf("                    No seen Win                     \n");
    
    char dllPath[MAX_PATH];
    GetFullPathNameA("hook.dll", MAX_PATH, dllPath, NULL);
    
    HMODULE hDll = LoadLibraryA(dllPath);
    if (hDll == NULL) {
        printf("[ERROR] Could not load '%s'.\n", dllPath);
        system("pause");
        return 1;
    }
    
    SetGhostHookFunc SetGhostHook = (SetGhostHookFunc)GetProcAddress(hDll, "SetGhostHook");
    if (SetGhostHook == NULL) {
        printf("[ERROR] Could not find SetGhostHook function in DLL.\n");
        FreeLibrary(hDll);
        system("pause");
        return 1;
    }

    printf("Currently running applications:\n");
    EnumWindows(EnumWindowsProc, 0);
    printf("Enter the number of the application: ");
    
    int choice;
    if (scanf("%d", &choice) == 1 && choice > 0 && choice <= window_count) {
        HWND selected_hwnd = window_list[choice - 1];
        char title[512];
        GetWindowTextA(selected_hwnd, title, sizeof(title));
        
        printf("\nSelected: '%s'\n", title);
        printf("[1] Hide from screen share (Ghost Mode)\n");
        printf("[2] Unhide (Normal Mode)\n");
        printf("Choose action (1 or 2): ");
        
        int action;
        if (scanf("%d", &action) == 1 && (action == 1 || action == 2)) {
            BOOL hide = (action == 1);
            BOOL result = SetGhostHook(selected_hwnd, hide);
            
            if (result) {
                printf("\n[SUCCESS] Hook command sent to '%s'!\n", title);
                if (hide) {
                    printf("If you heard a 'BEEP' sound, it is now a Ghost (Hidden).\n");
                } else {
                    printf("If you heard a 'BEEP' sound, it is now Visible again.\n");
                }
            } else {
                printf("\n[FAILED] Could not attach to '%s'.\n", title);
                printf("Make sure this program is running as Administrator.\n");
            }
        } else {
            printf("\n[ERROR] Invalid action selection.\n");
        }
    } else {
        printf("\n[ERROR] Invalid application selection.\n");
    }
    
    FreeLibrary(hDll);
    
    printf("\nPress Enter to exit...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    getchar();
    
    return 0;
}
