#include "Ini.h"

void Ini::ListViewMsg(WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	//ListView
	case LISTVIEW_FINAL:
		{
			LPNMITEMACTIVATE now = (LPNMITEMACTIVATE)lParam;
			switch(now ->hdr.code)
			{
			case NM_CLICK:
				{
					selected = now ->iItem;
				}
				break;
			case NM_DBLCLK:
				break;
			case NM_RCLICK:
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}

void Ini::ReceiveCommand(HWND hwnd,WPARAM wParam, LPARAM lParam)
{
	WORD lw = LOWORD(wParam);
	WORD hw = HIWORD(wParam);
	switch(lw)
	{
	case IDM_FILE_SAVE:
		{
			::OutputDebugStringW(L"Menu Save\n");
			if(!mem.SaveAsFile())
			{
				MessageBox(hwnd,L"写文件失败！！！",L"Warning",MB_OK);
			}
		}
		break;
	case BTN_CONFIRM:		//confirm
		{
			if(hw == BN_CLICKED)
			{
				WCHAR s[50] = {0,};
				CHAR des[50] = {0,};
				GetWindowTextW(hEdit_pid,s,50);
				DWORD dw = tool.FindProcessID(s);
				if(dw == 0xFFFFFFFF)
				{
					MessageBox(hwnd,L"输入错误！！！",L"Waring",MB_OK);
					break;
				}
				sprintf_s(des,"当前进程：%d",dw);
				SetWindowTextA(hStatic_pid,des);
				mem.setPid(dw);
			}
		}
		break;
	case BTN_SEARCH:		//search
		{
			if(hw == BN_CLICKED)
			{
				if(mem.getPid() == 0)
				{
					MessageBox(hwnd,L"未知进程号！！！",L"Waring",MB_OK);
					break;
				}
				CHAR  s[50] = {0,};
				GetWindowTextA(hEdit_num,s,50);
				DWORD dw =atoi(s);
				if(dw == 0)
				{
					MessageBox(hwnd,L"输入错误！！！",L"Waring",MB_OK);
					break;
				}
				mem.setNum(dw);
				v.clear();
				v = mem.OnceFinalResult();
				if(v.empty())
				{
					MessageBox(hwnd,L"内存读取时出错或查找不到结果！！！",L"Waring",MB_OK);
					ClearListView(hListView_final);
					break;
				}
				//插入值ListView
				ShowResultAdd();
			}
		}
		break;
	case BTN_MOD:
		{
			if(hw == BN_CLICKED)
			{
				CHAR s[50] = {0,};
				GetWindowTextA(hEdit_mod,s,50);
				DWORD dw = atoi(s);
				if(dw == 0)
				{
					MessageBox(hwnd,L"输入错误！！！",L"Waring",MB_OK);
					break;
				}
				if(selected == -1)
				{
					MessageBox(hwnd,L"未选择修改位置！！！",L"Waring",MB_OK);
					break;
				}
				if(!mem.WriteMem(v.at(selected),dw))
					MessageBox(hwnd,L"写入时出错误！！！",L"Waring",MB_OK);
				
			}
		}
		break;
	case BTN_STATE:
		{
			if(hw == BN_CLICKED)
				if(!mem.ShowMemState(mp))
				{
					MessageBox(hwnd,L"Error For State!!!",L"Warning",MB_OK);
					ClearListView(hListView_part);
					break;
				}
				ShowMemState();
				SetInfoText();
		}
		break;
	default:
		break;
	}
}

void Ini::CreateChildWindows(HWND hwnd)
{
	//第一列,进程pid
	hEdit_pid = CreateWindow(
				L"EDIT",   // predefined class
				L"",       // button text
				WS_VISIBLE | WS_CHILD,  // styles

				// Size and position values are given explicitly, because
				// the CW_USEDEFAULT constant gives zero values for buttons.
				10,         // starting x position
				10,         // starting y position
				100,        // button width
				50,        // button height
				hwnd,       // parent window
				(HMENU)EDIT_PID,       // No menu. NO menu that means ID of Btn
				(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
				NULL);      // pointer not needed
	//查找的值
	hEdit_num = CreateWindow(
				L"EDIT",   // predefined class
				L"",       // button text
				WS_VISIBLE | WS_CHILD,  // styles

				// Size and position values are given explicitly, because
				// the CW_USEDEFAULT constant gives zero values for buttons.
				10,         // starting x position
				70,         // starting y position
				100,        // button width
				50,        // button height
				hwnd,       // parent window
				(HMENU)EDIT_NUM,       // No menu. NO menu that means ID of Btn
				(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
				NULL);      // pointer not needed
	//第二列，确定pid
	hBtn_confirm = CreateWindow(
				L"BUTTON",   // predefined class
				L"Confirm",       // button text
				WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // styles

				// Size and position values are given explicitly, because
				// the CW_USEDEFAULT constant gives zero values for buttons.
				120,         // starting x position
				10,         // starting y position
				100,        // button width
				50,        // button height
				hwnd,       // parent window
				(HMENU)BTN_CONFIRM,       // No menu. NO menu that means ID of Btn
				(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
				NULL);      // pointer not needed
	//查找
	hBtn_search = CreateWindow(
				L"BUTTON",   // predefined class
				L"Search",       // button text
				WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // styles

				// Size and position values are given explicitly, because
				// the CW_USEDEFAULT constant gives zero values for buttons.
				120,         // starting x position
				70,         // starting y position
				100,        // button width
				50,        // button height
				hwnd,       // parent window
				(HMENU)BTN_SEARCH,       // No menu. NO menu that means ID of Btn
				(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
				NULL);      // pointer not needed
	//当前进程
	hStatic_pid = CreateWindow(
				L"STATIC",   // predefined class
				L"当前进程：",       // button text
				WS_VISIBLE | WS_CHILD,  // styles

				// Size and position values are given explicitly, because
				// the CW_USEDEFAULT constant gives zero values for buttons.
				10,         // starting x position
				130,         // starting y position
				100,        // button width
				100,        // button height
				hwnd,       // parent window
				(HMENU)STATIC_PID,       // No menu. NO menu that means ID of Btn
				(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
				NULL);      // pointer not needed
	//ListView
	hListView_final = CreateWindowExW(NULL,  
                L"SysListView32",
				NULL,
                WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS,
                240, 10, 300, 400,
                hwnd, (HMENU)LISTVIEW_FINAL,   
                (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),   
                NULL);
	//创建Listview列
	selected = -1;
	//创建ListView列
	CreateColumnFinal();
	//要写入的值
	hEdit_mod = CreateWindow(
				L"EDIT",   // predefined class
				L"",       // button text
				WS_VISIBLE | WS_CHILD,  // styles
				// Size and position values are given explicitly, because
				// the CW_USEDEFAULT constant gives zero values for buttons.
				10,         // starting x position
				240,         // starting y position
				100,        // button width
				50,        // button height
				hwnd,       // parent window
				(HMENU)EDIT_MOD,       // No menu. NO menu that means ID of Btn
				(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
				NULL);      // pointer not needed
	//确定写入
	hBtn_mod = CreateWindow(
				L"BUTTON",   // predefined class
				L"Modify",       // button text
				WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // styles

				// Size and position values are given explicitly, because
				// the CW_USEDEFAULT constant gives zero values for buttons.
				120,         // starting x position
				240,         // starting y position
				100,        // button width
				50,        // button height
				hwnd,       // parent window
				(HMENU)BTN_MOD,       // No menu. NO menu that means ID of Btn
				(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
				NULL);      // pointer not needed

	//显示内存占用状态按钮
	hBtn_State = CreateWindow(
				L"BUTTON",   // predefined class
				L"MemState",       // button text
				WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // styles
				10,         // starting x position
				450,         // starting y position
				100,        // button width
				50,        // button height
				hwnd,       // parent window
				(HMENU)BTN_STATE,       // No menu. NO menu that means ID of Btn
				(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
				NULL);      // pointer not needed
	//进程块状态
	hListView_part = CreateWindowExW(NULL,
                L"SysListView32",
				NULL,
                WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS,
                240, 450, 300, 350,
                hwnd, (HMENU)LISTVIEW_PART,   
                (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),   
                NULL);
	CreateColumnPart();

	//可操作进程块信息
	hStatic_info = CreateWindow(
				L"STATIC",   // predefined class
				L"当前进程：",       // button text
				WS_VISIBLE | WS_CHILD,  // styles

				// Size and position values are given explicitly, because
				// the CW_USEDEFAULT constant gives zero values for buttons.
				120,         // starting x position
				450,         // starting y position
				100,        // button width
				80,        // button height
				hwnd,       // parent window
				(HMENU)STATIC_PID,       // No menu. NO menu that means ID of Btn
				(HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
				NULL);      // pointer not needed
}

BOOL Ini:: MyRegister(HINSTANCE& hThisInstance)
{
	/*  Make the class name into a global variable  */
	TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

	/*  Declare Windows procedure  */
	//LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

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
	return TRUE;
}

void Ini:: CreateMainWindow(HINSTANCE& hThisInstance,int& nCmdShow)
{
	//菜单栏
	hMenu = CreateMenu();
	hMenuPop1 = CreateMenu();
	AppendMenu(hMenuPop1,MF_STRING,IDM_FILE_SAVE,L"Dump");
	AppendMenu(hMenu,MF_POPUP,(UINT)hMenuPop1,L"File");

	/*  Make the class name into a global variable  */
	TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
	/* The class is registered, let's create the program*/
    hWnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           hMenu,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hWnd, nCmdShow);

	/* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }
}

//显示查找结果地址和值
void Ini::ShowResultAdd()
{
	LVITEM item;			//ListView Item
	ClearListView(hListView_final);
	RtlZeroMemory(&item, sizeof(LVITEM));
	item.mask = LVIF_TEXT;  
	vector<DWORD>::iterator iter = v.begin();
	//WCHAR ws[30] = {0,};
	//wsprintf(ws,L"Length is %d\n",v.size());
	//OutputDebugString(L"");
	DWORD i = 0;
	while(iter != v.end())
	{
		item.iItem = i;		//行
		item.iSubItem = 0;	//列
		WCHAR s[50] = {0,};
		DWORD dw = (*iter);
		wsprintf(s,L"0x%.8x",dw);
		item.pszText = s;
		ListView_InsertItem(hListView_final,&item);

		item.iSubItem = 1;
		wsprintf(s,L"%d",mem.getNum());
		item.pszText = s;
		ListView_SetItem(hListView_final,&item);
		iter++;
		i++;
	}
}

void Ini::ShowMemState()
{
	LVITEM item;			//ListView Item
	//清空ListView
	ClearListView(hListView_part);
	
	RtlZeroMemory(&item, sizeof(LVITEM));
	item.mask = LVIF_TEXT;  
	map<DWORD,DWORD>::iterator iter = mp.begin();
	DWORD i = 0;
	while(iter != mp.end())
	{
		item.iItem = i;		//行
		item.iSubItem = 0;	//列
		WCHAR s[50] = {0,};
		wsprintf(s,L"0x%.8x",iter ->first);
		item.pszText = s;
		ListView_InsertItem(hListView_part,&item);

		item.iSubItem = 1;
		wsprintf(s,L"%x",iter ->second);
		item.pszText = s;
		ListView_SetItem(hListView_part,&item);
		iter++;
		i++;
	}
    //item.iImage = -1;
}

void Ini::ClearListView(HWND hList)
{
	int c = ListView_GetItemCount(hList);
	while(c >= 0)
	{
		ListView_DeleteItem(hList,c);
		c--;
	}
}

void Ini::CreateColumnFinal()
{
	LVCOLUMN list;
	RtlZeroMemory(&list, sizeof(LVCOLUMN));
	RECT rt;
	GetWindowRect(hListView_final,&rt);
	list.mask = list.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;	//掩码
	list.fmt = LVCFMT_LEFT;		//左对齐
	list.cx = (rt.right-rt.left)/2;	//宽
	list.pszText = L"地址";
	ListView_InsertColumn(hListView_final,0,&list);
	list.cx = (rt.right-rt.left)/2;	//宽
	list.pszText = L"值";
	ListView_InsertColumn(hListView_final,1,&list);
}

void Ini::CreateColumnPart()
{
	LVCOLUMN list;
	RtlZeroMemory(&list, sizeof(LVCOLUMN));
	RECT rt;
	GetWindowRect(hListView_part,&rt);
	list.mask = list.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;	//掩码
	list.fmt = LVCFMT_LEFT;		//左对齐
	list.cx = (rt.right-rt.left)/2;	//宽
	list.pszText = L"地址";
	ListView_InsertColumn(hListView_part,0,&list);
	list.cx = (rt.right-rt.left)/2;	//宽
	list.pszText = L"大小";
	ListView_InsertColumn(hListView_part,1,&list);
}

void Ini::SetInfoText()
{
	DWORD dw = 0;
	for(map<DWORD,DWORD>::iterator it = mp.begin();it != mp.end();it++)
		dw += it ->second;
	DWORD dwB = dw%1024;
	DWORD dwKB = dw/1024%1024;
	DWORD dwMB = dw/1024/1024%1024;
	DWORD dwGB = dw/1024/1024/1024;
	WCHAR s[50] = {0,};
	wsprintf(s,L"可操作内存大小：\n0x%x\n%dG %dM %dKB %dB",dw,dwGB,dwMB,dwKB,dwB);
	SetWindowTextW(hStatic_info,s);
}