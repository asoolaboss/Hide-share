# No Seen Win 👻

A Windows utility program that allows you to hide specific application windows from screen-sharing software (like Discord, OBS, Zoom, etc.). The tool works by injecting a DLL and utilizing the `SetWindowDisplayAffinity` Windows API.

## Features

- **Ghost Mode (Hide)**: Makes a window completely invisible to capture tools and screen-sharing applications.
- **Normal Mode (Unhide)**: Restores the window's normal visibility status.
- **Easy-to-use CLI**: Simply select a running application from the list and apply the effect.

## How It Works

The application uses `SetWindowsHookEx` to inject `hook.dll` into the target application's process space. Once injected, a custom Windows Message triggers the target application to call `SetWindowDisplayAffinity(hwnd, WDA_EXCLUDEFROMCAPTURE)` on its own window, effectively blocking it from being captured by other applications.

## Building from Source

You will need a C compiler like **GCC (MinGW)** installed and added to your system's PATH.

1. Clone the repository:
   ```cmd
   git clone https://github.com/asoolaboss/Hide-share.git
   cd Hide-share
   ```

2. Run the provided build script:
   ```cmd
   build.bat
   ```

This will generate `hook.dll` and `ghost.exe` in the same directory.

## Usage

1. **Run as Administrator**: The program MUST be run as an Administrator to properly inject into other applications.
2. Run `ghost.exe`.
3. The console will display a list of currently running windows.
4. Enter the number corresponding to the application you want to hide.
5. Choose action `1` to Hide (Ghost Mode) or `2` to Unhide.
6. A 'BEEP' sound indicates successful injection and status change.

> **Note**: Some applications with aggressive anti-cheat or custom rendering engines might not be fully compatible with window hooks.

## Warnings & Disclaimer

- **False Positives**: Because this tool uses DLL injection (`SetWindowsHookEx`), some Antivirus software might flag it as malicious. This is a common false positive for tools of this nature. The source code is completely open for review.
- **Responsibility**: This tool is provided for educational purposes. The authors are not responsible for any misuse of this software.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.