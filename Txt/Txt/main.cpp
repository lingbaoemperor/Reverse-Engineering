// TextTest.cpp : 定义应用程序的入口点。
//

#include <Windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>
HWND hwndBtn =NULL;
HWND hwndSta = NULL;
HWND hwndMod = NULL;
HWND hwndTxt = NULL;
DWORD dw = 30;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hwndBtn = NULL;
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
			{
				hwndBtn = CreateWindow(
				L"BUTTON",   // predefined class
				L"Refresh",       // button text
				WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // styles

				// Size and position values are given explicitly, because
				// the CW_USEDEFAULT constant gives zero values for buttons.
				150,         // starting x position
				10,         // starting y position
				100,        // button width
				100,        // button height
				hwnd,       // parent window
				(HMENU)1,       // No menu. NO menu that means ID of Btn
				(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
				NULL);      // pointer not needed

				hwndSta = CreateWindow(
				L"STATIC",   // predefined class
				L"Ini",       // button text
				WS_VISIBLE | WS_CHILD | BS_TEXT,  // styles

				// Size and position values are given explicitly, because
				// the CW_USEDEFAULT constant gives zero values for buttons.
				10,         // starting x position
				10,         // starting y position
				100,        // button width
				100,        // button height
				hwnd,       // parent window
				(HMENU)2,       // No menu. NO menu that means ID of Btn
				(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
				NULL);      // pointer not needed

				hwndMod = CreateWindow(
				L"BUTTON",   // predefined class
				L"Test",       // button text
				WS_VISIBLE | WS_CHILD,  // styles

				// Size and position values are given explicitly, because
				// the CW_USEDEFAULT constant gives zero values for buttons.
				150,         // starting x position
				150,         // starting y position
				100,        // button width
				100,        // button height
				hwnd,       // parent window
				(HMENU)3,       // No menu. NO menu that means ID of Btn
				(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
				NULL);      // pointer not needed

				hwndTxt = CreateWindow(
				L"EDIT",   // predefined class
				L"",       // button text
				WS_VISIBLE | WS_CHILD,  // styles

				// Size and position values are given explicitly, because
				// the CW_USEDEFAULT constant gives zero values for buttons.
				10,         // starting x position
				150,         // starting y position
				100,        // button width
				100,        // button height
				hwnd,       // parent window
				(HMENU)4,       // No menu. NO menu that means ID of Btn
				(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
				NULL);      // pointer not needed
			}
            break;
             //MoveWindow(hwndSmaller, 100,100,  100, 100, TRUE) ;
        case WM_COMMAND:
            if(LOWORD(wParam) == 1 && HIWORD(wParam) == BN_CLICKED)
            {
                CHAR s[20] = {0,};
				sprintf(s,"%d",dw);
				SetWindowTextA(hwndSta,s);
            }
			else if(LOWORD(wParam) == 3 && HIWORD(wParam) == BN_CLICKED)
			{
				CHAR  s[20] = {0,};
				GetWindowTextA(hwndTxt,s,19);
				dw =atoi(s);
			}
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
