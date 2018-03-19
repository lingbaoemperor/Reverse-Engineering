#include <Windows.h>
#include <tchar.h>
#include <CommCtrl.h>
#include "mem.h"
#include "ptool.h"
#define EDIT_PID 1
#define EDIT_NUM 2
#define BTN_CONFIRM 3
#define BTN_SEARCH 4
#define STATIC_PID 5
#define EDIT_MOD 7
#define BTN_MOD 8
#define LISTVIEW_FINAL 6
#define BTN_STATE 9
#define LISTVIEW_PART 10

#define IDM_FILE_SAVE 11

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

class Ini{
private:
	HWND hWnd;               /* This is the handle for our window */
	MSG messages;            /* Here messages to the application are saved */
	WNDCLASSEX wincl;        /* Data structure for the windowclass */

	HMENU hMenu;			//主菜单,水平
	HMENU hMenuPop1;			

	//1
	HWND hEdit_pid;			//进程pid	ID 1	(1,1)	10 10 100 50
	HWND hEdit_num;			//要查找的数值	ID 2	(2,1)	10 70 100 50
	//2
	HWND hBtn_confirm;		//确定 ID 3		(1,2)	120 10 100 50
	HWND hBtn_search;		//查找 ID 4		(2,2)	120 70 100 50

	//3
	HWND hStatic_pid;		//当前进程 ID 5		(3,-)	10 130 100 100

	//4
	HWND hEdit_mod;			//ID 7				(4,1)	10 240 100 50
	HWND hBtn_mod;			//ID 8				(4,2)	120 240 100 50

	//4
	HWND hListView_final;	//最终结果 ID 6				240 10 300 500
	HWND hBtn_State;		//内存状态			(5,?)	120 300 100 50

	HWND hListView_part;	//内存块状态

	HWND hStatic_info;		//可操作进程块信息

	Mem<DWORD> mem;
	vector<DWORD> v;
	map<DWORD,DWORD> mp;

	Tool tool;

	int selected;

public:
	Ini() {selected = -1;}
	MSG rtMsg(){return messages;}
	BOOL MyRegister(HINSTANCE& hThisInstance);
	void CreateMainWindow(HINSTANCE& hThisInstance,int& nCmdShow);
	void CreateChildWindows(HWND hwnd);
	void ReceiveCommand(HWND hwnd,WPARAM wParam, LPARAM lParam);
	void ShowResultAdd();	
	void ListViewMsg(WPARAM wParam,LPARAM lParam);
	void ShowMemState();
	void CreateColumnFinal();
	void CreateColumnPart();
	void SetInfoText();
	void ClearListView(HWND);
};

