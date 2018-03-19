#include "ptool.h"
#include <tchar.h>
#include <TlHelp32.h>

DWORD Tool:: FindProcessID(LPCTSTR szProcessName)
{
	DWORD dwPID = 0xFFFFFFFF;
	HANDLE hSnapShot = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 pe;
	
	//��ȡϵͳ����
	pe.dwSize = sizeof(PROCESSENTRY32);
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL,NULL);

	//���ҽ���
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