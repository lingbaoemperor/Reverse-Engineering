#include "ptool.h"
#include <tchar.h>
#include <TlHelp32.h>

DWORD Tool:: FindProcessID(LPCTSTR szProcessName)
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