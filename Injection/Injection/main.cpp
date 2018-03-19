#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"UrlMon.lib")
HMODULE g_hMod;
#define DEF_URL (L"https://timgsa.baidu.com/timg?image&amp;quality=80&amp;size=b9999_10000&amp;sec=1511411500&amp;di=c3579fa756ba0fe854f2982c2aa05f87&amp;imgtype=jpg&amp;er=1&amp;src=http%3A%2F%2Fimg3.duitang.com%2Fuploads%2Fitem%2F201501%2F17%2F20150117183625_8eBxP.jpeg")
#define DEF_FILE_NAME (L"aaa.jpeg")
DWORD WINAPI ThreadProc(LPVOID lParam)
{
	TCHAR szPath[_MAX_PATH] = {0,};
	if(!GetModuleFileNameW(g_hMod,szPath,MAX_PATH))
		return FALSE;
	TCHAR* p = wcsrchr(szPath,'\\');
	if(!p)
		return FALSE;

	wcscpy_s(p+1,_MAX_PATH,DEF_FILE_NAME);
	URLDownloadToFile(NULL,DEF_URL,szPath,0,NULL);
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDll,DWORD fdwReason,LPVOID lpvReserved)
{
	HANDLE hThread = NULL;
	g_hMod = (HMODULE)hinstDll;
	switch(fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		OutputDebugString(L"mydll Injection!!!");
		hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadProc,NULL,0,NULL);
		CloseHandle(hThread);
		break;
	}
	return TRUE;
}