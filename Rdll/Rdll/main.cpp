
// a sample exported function
#include <tchar.h>
#include <Windows.h>
#include <WinInet.h>
unsigned char g_pICW[5] = {0,};
typedef HINTERNET (WINAPI *ICW)(HINTERNET hInternet,
                                     LPCWSTR lpszServerName,
                                     INTERNET_PORT nServerPort,
                                     LPCTSTR lpszUsername,
                                     LPCTSTR lpszPassword,
                                     DWORD dwService,
                                     DWORD dwFlags,
                                     DWORD dwContext);

BOOL hook_by_code(LPCSTR szDllName,LPCSTR szFunName,PROC pfnNew,PBYTE pOrgBytes)
{
    FARPROC pfnOrg;
    DWORD dwOldProtect,dwAddress;
    byte pBuf[5] = {0xE9,0,};
    PBYTE pByte;

    //要钩取的API地址
    pfnOrg = (FARPROC)GetProcAddress(GetModuleHandleA(szDllName),szFunName);
    pByte = (PBYTE)pfnOrg;

    //第一个字节为E9(JMP)，则已经钩取,
    if(pByte[0] == 0xE9)
        return FALSE;

    //向内存添加Read属性
    VirtualProtect((LPVOID)pfnOrg,5,PAGE_EXECUTE_READWRITE,&dwOldProtect);
    //备份原来代码的前五个字节
    memcpy(pOrgBytes,(PBYTE)pfnOrg,5);

    //计算JMP到何处，是相对与JMP指令的地址，即自己提供的函数地址减去钩取函数地址减5
    dwAddress = (DWORD)pfnNew - (DWORD)pfnOrg - 5;
    memcpy(&pBuf[1],(PBYTE)dwAddress,4);

    //修改钩取函数前面的字节为JMP...跳到。。。
    memcpy((PBYTE)pfnOrg,pBuf,5);

    //恢复内存属性
    VirtualProtect((LPVOID)pfnOrg,5,dwOldProtect,&dwOldProtect);
    return TRUE;
}

BOOL unhook_by_code(LPCSTR szDllName,LPCSTR szFuncName,PBYTE pOrgBytes)
{
    FARPROC pFunc;
    DWORD dwOldProtect;
    PBYTE pByte;

    pFunc = GetProcAddress(GetModuleHandleA(szDllName),szFuncName);
    pByte = (PBYTE)pFunc;

    if(pByte[0] != 0xE9)
        return FALSE;

    VirtualProtect((LPVOID)pFunc,5,PAGE_EXECUTE_READWRITE,&dwOldProtect);
    memcpy((PBYTE)pFunc,pOrgBytes,5);

    VirtualProtect((LPVOID)pFunc,5,dwOldProtect,&dwOldProtect);
    return TRUE;
}
HINTERNET WINAPI NewInternetConnectW(HINTERNET hInternet,
                                     LPCWSTR lpszServerName,
                                     INTERNET_PORT nServerPort,
                                     LPCTSTR lpszUsername,
                                     LPCTSTR lpszPassword,
                                     DWORD dwService,
                                     DWORD dwFlags,
                                     DWORD dwContext)

{
    HINTERNET hInt = NULL;
    FARPROC pFu = NULL;
    HMODULE hMod = NULL;

	//执行InterConnectW之前把代码（前五个字节）改回去
    unhook_by_code("wininet.dll","InternetConnectW",g_pICW);

    hMod = GetModuleHandle(L"wininet.dll");
    pFu = GetProcAddress(hMod,"InternetConnectW");

    hInt = ((ICW)pFu)(hInternet,
                        L"www.taobao.com",
                        nServerPort,
                        lpszUsername,
                        lpszPassword,
                        dwService,
                        dwFlags,
                        dwContext);

	//InternetConnect执行结束，再次修改其代码（到NewInternetConnect）
    hook_by_code("wininet.dll","InternetConnectW",(PROC)NewInternetConnectW,g_pICW);

    return hInt;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	char szCursorProc[MAX_PATH] = {0,};
	char* p= NULL;
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
		{
			GetModuleFileNameA(NULL,szCursorProc,MAX_PATH);
			p = strchr(szCursorProc,'\\');
			if(p != NULL && !_stricmp(p+1,"iexplore.exe"))
			{
				LoadLibrary(L"wininet.dll");
			}
			//不执行InternetConnect执行NewInternectConnect
			hook_by_code("wininet.dll","InternetConnectW",(PROC)NewInternetConnectW,g_pICW);
			//MessageBoxA(NULL,"AAA","bbb",MB_OK);
            break;
		}
        case DLL_PROCESS_DETACH:
			MessageBoxA(NULL,"detach","1",MB_OK);
            unhook_by_code("wininet.dll","InternetConnectW",g_pICW);
            break;
    }
    return TRUE; // succesful
}