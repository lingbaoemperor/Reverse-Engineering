#include <Windows.h>
#include <tchar.h>
#include <TlHelp32.h>
#define DEF_PROC_NAME (L"notepad.exe")
#define DEF_DLL_NAME (L"Injection.dll")

DWORD FindProcessID(LPCTSTR szProcessName);		//获得进程ID
BOOL SetPrivilege(LPCTSTR lpszPrivilege,BOOL bEnablePrivilege);
BOOL EjectDll(DWORD dwPID,LPCTSTR szDllName);

int main()
{
	DWORD dwPID  = 0xFFFFFFFF;
	dwPID = FindProcessID(DEF_PROC_NAME);
	if(dwPID == 0XFFFFFFFF)
	{
		wprintf(L"There is no %s process!!!\n",DEF_PROC_NAME);
		return 1;
	}
	wprintf(L"PID of %s is %d\n",DEF_PROC_NAME,dwPID);

	if(!SetPrivilege(SE_DEBUG_NAME,TRUE))
		return 1;

	if(EjectDll(dwPID,DEF_DLL_NAME))
		wprintf(L"EjectDll(%d,%s) success!!!\n",dwPID,DEF_DLL_NAME);
	else
		wprintf(L"EjectDll(%d,%s) failed!!!\n",dwPID,DEF_DLL_NAME);
	return 0;
}

DWORD FindProcessID(LPCTSTR szProcessName)
{
	DWORD dwPID = 0xFFFFFFFF;
	HANDLE hSnapShot = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 pe;
	
	//获取系统快照
	pe.dwSize = sizeof(PROCESSENTRY32);
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL,NULL);

	//查找进程
	Process32First(hSnapShot,&pe);
	do
	{
		if(!wcsicmp(szProcessName,(LPCTSTR)pe.szExeFile))
		{
			dwPID = pe.th32ProcessID;
			break;
		}
	}while(Process32Next(hSnapShot,&pe));
	
	CloseHandle(hSnapShot);
	return dwPID;
}

BOOL SetPrivilege(LPCTSTR lpszPrivilege,BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
	HANDLE hToken;
	LUID luid;

	if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
	{
		wprintf(L"OpenProcessToken Error: %u\n",GetLastError());
		return FALSE;
	}
	if(!LookupPrivilegeValue(NULL,	//lookup privilege on local system
							lpszPrivilege,	//privilege to lookup
							&luid))			//receives LUID of privilege
	{
		wprintf(L"LookupPrivilegeValue Erroe: %u\n",GetLastError());
		return FALSE;
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if(bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	//Enable the privilege or disable all privileges.
	if(!AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(TOKEN_PRIVILEGES),NULL,NULL))
	{
		wprintf(L"AdjustTokenPrivileges Error: %u\n",GetLastError());
		return FALSE;
	}
	if(GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		wprintf(L"The token does not have the specified privilege. \n");
		return FALSE;
	}
	return TRUE;
}

BOOL EjectDll(DWORD dwPID,LPCTSTR szDllName)
{
	BOOL bMore = FALSE,bFound = FALSE;
	HANDLE hSnapShot,hProcess,hThread;
	HMODULE hModule = NULL;
	MODULEENTRY32 me = {sizeof(me)};
	LPTHREAD_START_ROUTINE pThreadProc;

	//用TH32CS_SNAPMODULE参数，获取加载到notepad进程的DLL名字
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,dwPID);

	bMore = Module32First(hSnapShot,&me);
	for(;bMore;bMore = Module32Next(hSnapShot,&me))
	{
		if(!wcsicmp((LPCTSTR)me.szModule,szDllName) || !wcsicmp((LPCTSTR)me.szExePath,szDllName))
		{
			bFound = TRUE;
			break;
		}
	}
	if(!bFound)
	{
		CloseHandle(hSnapShot);
		return FALSE;
	}
	if(!(hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPID)))
	{
		wprintf(L"OpenProcess(%d) failed!!! [%d]\n",dwPID,GetLastError());
		return FALSE;
	}
	hModule = GetModuleHandle(L"kernel32.dll");
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule,"FreeLibrary");

	hThread = CreateRemoteThread(hProcess,NULL,0,pThreadProc,me.modBaseAddr,0,NULL);

	WaitForSingleObject(hThread,INFINITE);
	
	CloseHandle(hThread);
	CloseHandle(hProcess);
	CloseHandle(hSnapShot);
	
	return TRUE;
}