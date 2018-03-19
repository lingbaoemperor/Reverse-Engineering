#include "Ini.h"

Ini app;
int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
	app.MyRegister(hThisInstance);
	app.CreateMainWindow(hThisInstance,nCmdShow);
    /* The program return-value is 0 - The value that PostQuitMessage() gave */
	return app.rtMsg().wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)                  /* handle the messages */
    {
		case WM_CREATE:
			{
				app.CreateChildWindows(hwnd);
			}
			break;
             //MoveWindow(hwndSmaller, 100,100,  100, 100, TRUE) ;
        case WM_COMMAND:
			{
				app.ReceiveCommand(hwnd,wParam,lParam);
			}
            break;
		case WM_NOTIFY:
			{
				app.ListViewMsg(wParam,lParam);
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