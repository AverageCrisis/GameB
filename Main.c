#include <stdio.h>

#pragma warning(push, 3)

#pragma warning(disable:4668)

#pragma warning(disable:5105)

#include <windows.h>

#pragma warning(pop)

#include "Main.h"

HWND gGameWindow;

BOOL gGameIsRunning;

GAMEBITMAP gDrawingSurface;


int  WINAPI WinMain(
    _In_      HINSTANCE   Instance,
    _In_opt_  HINSTANCE   PreviousInstance,
    _In_      PSTR        CommandLine,
    _In_      INT         CmdShow
)
{
    


    UNREFERENCED_PARAMETER(Instance);

    UNREFERENCED_PARAMETER(PreviousInstance);

    UNREFERENCED_PARAMETER(CommandLine);

    UNREFERENCED_PARAMETER(CmdShow);

    if (GameIsAlreadyRunning() == TRUE)
    {
        MessageBoxA(NULL, "Another instance of this program is already running!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

    if (CreateMainGameWindow() != ERROR_SUCCESS)
    {
        goto Exit;
    }

    gDrawingSurface.BitmapInfo.bmiHeader.biSize = sizeof(gDrawingSurface.BitmapInfo.bmiHeader);

    gDrawingSurface.BitmapInfo.bmiHeader.biWidth = GAME_RES_WIDTH;

    gDrawingSurface.BitmapInfo.bmiHeader.biHeight = GAME_RES_HEIGHT;

    gDrawingSurface.BitmapInfo.bmiHeader.biBitCount = GAME_BPP;

    gDrawingSurface.BitmapInfo.bmiHeader.biCompression = BI_RGB;

    gDrawingSurface.BitmapInfo.bmiHeader.biPlanes = 1;

    gDrawingSurface.Memory = VirtualAlloc(
        NULL,                            // The memory address where to allocate memory (NULL creates a new one)
        GAME_DRAWING_AREA_MEMORY_SIZE,   // The size of how much memory we want to allocate
        MEM_RESERVE | MEM_COMMIT,        // Type of the allocation
        PAGE_READWRITE);                 // Protection of the memory

    if (gDrawingSurface.Memory == NULL)
    {
        MessageBoxA(NULL, "Failed to allocate memory for drawing surface!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

    MSG Message = { 0 };

    gGameIsRunning = TRUE;

    while (gGameIsRunning == TRUE)
    {
        while (PeekMessageA(&Message, gGameWindow, 0, 0, PM_REMOVE))
        {
            DispatchMessageA(&Message);
        }
        
        ProcessPlayerInput();

        RenderFrameGraphics();

        Sleep(1);
    }

    

Exit:

    return(0);
}

LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, LPARAM _In_ LParam)
{
    LRESULT Result = 0;


    switch (Message)
    {
   
        case WM_CLOSE:
        {

            gGameIsRunning = FALSE;
            PostQuitMessage(0);

            break;
        }
        default:
        {
            Result = DefWindowProcA(WindowHandle, Message, WParam, LParam);
        }
    }
    return(Result);
}

DWORD CreateMainGameWindow(void)
{
    DWORD Result = ERROR_SUCCESS;

    WNDCLASSEXA WindowClass = { 0 };

    WindowClass.cbSize = sizeof(WNDCLASSEXA); // Size in bytes

    WindowClass.style = 0;                    // We want full screen style

    WindowClass.lpfnWndProc = MainWindowProc;        // Long pointer to a window procedure function

    WindowClass.cbClsExtra = 0; // dontmatter

    WindowClass.cbWndExtra = 0; // dontmatter

    WindowClass.hInstance = GetModuleHandleA(NULL); // Where it should reside

    WindowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION); // Wants a handle to an icon (IDI is built in)

    WindowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION); // handle for a small icon

    WindowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);

    WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // What color? COLOR_WINDOW is default gray

    WindowClass.lpszMenuName = NULL; // Drop down menus 

    WindowClass.lpszClassName = GAME_NAME "_WINDOWCLASS"; // (long pointer to a string) null terminated string



    if (RegisterClassExA(&WindowClass) == 0)
    {
        Result = GetLastError();
        MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

    gGameWindow = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        WindowClass.lpszClassName,
        GAME_NAME,      // Title
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, // Style
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        640, 480,
        NULL, NULL, GetModuleHandleA(NULL), NULL
    );

    //ShowWindow(WindowHandle, TRUE);

    if (gGameWindow == NULL)
    {
        Result = GetLastError();
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

Exit:
    return(0);
}

BOOL GameIsAlreadyRunning(void)
{
    HANDLE Mutex = NULL;

    Mutex = CreateMutexA(NULL, FALSE, GAME_NAME "_GameMutex");

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

void ProcessPlayerInput(void)
{
    
    short EscapeKeyIsDown = GetAsyncKeyState(VK_ESCAPE);
    
    if (EscapeKeyIsDown)
    {
        SendMessageA(gGameWindow, WM_CLOSE, 0, 0);
    }
    
}

void RenderFrameGraphics(void)
{

}