#include "hook.h"
#include <tchar.h>
unsigned char g_pICW[5] = {0,};
BOOL hook_by_code(LPCSTR szDllName,LPCSTR szFunName,PROC pfnNew,PBYTE pOrgBytes)
{
    FARPROC pfnOrg;
    DWORD dwOldProtect,dwAddress;
    byte pBuf[5] = {0xE9,0,};
    PBYTE pByte;

    //Ҫ��ȡ��API��ַ
    pfnOrg = (FARPROC)GetProcAddress(GetModuleHandleA(szDllName),szFunName);
    pByte = (PBYTE)pfnOrg;

    //��һ���ֽ�ΪE9(JMP)�����Ѿ���ȡ,
    if(pByte[0] == 0xE9)
        return FALSE;

    //���ڴ����Read����
    VirtualProtect((LPVOID)pfnOrg,5,PAGE_EXECUTE_READWRITE,&dwOldProtect);
    //����ԭ�������ǰ����ֽ�
    memcpy(pOrgBytes,(PBYTE)pfnOrg,5);

    //����JMP���δ����������JMPָ��ĵ�ַ�����Լ��ṩ�ĺ�����ַ��ȥ��ȡ������ַ��5
    dwAddress = (DWORD)pfnNew - (DWORD)pfnOrg - 5;
    memcpy(&pBuf[1],(PBYTE)dwAddress,4);

    //�޸Ĺ�ȡ����ǰ����ֽ�ΪJMP...����������
    memcpy((PBYTE)pfnOrg,pBuf,5);

    //�ָ��ڴ�����
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

	//ִ��InterConnectW֮ǰ�Ѵ��루ǰ����ֽڣ��Ļ�ȥ
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

	//InternetConnectִ�н������ٴ��޸�����루��NewInternetConnect��
    hook_by_code("wininet.dll","InternetConnectW",(PROC)NewInternetConnectW,g_pICW);

    return hInt;
}
/*
BOOL WINAPI NewCreateProcessInternalW(HANDLE hToken,
										LPCTSTR lpApplicationName,
										LPTSTR lpCommandLine,
										LPSECURITY_ATTRIBUTES lpProcessAttributes,
										LPSECURITY_ATTRIBUTES lpThreadAttributes,
										BOOL bInheritHandles,
										DWORD dwCreationFlags,
										LPVOID lpEnvironment,
										LPCTSTR lpCurrentDirectory,
										LPSTARTUPINFOA lpStartupInfo,
										LPPROCESS_INFORMATION lpProcessInformation,
										PHANDLE hNewToken)
{
	BOOL status = FALSE;
    FARPROC pFunc = NULL;
    DWORD dwPID = 0;
    static DWORD dwPrevPID = 0;

    HMODULE hMod = NULL;
    TCHAR szModPath[MAX_PATH] = {0,};
    hMod = GetModuleHandle(L"kernel32.dll");

    pFunc = GetProcAddress(hMod,"CreateProcessInternalW");
	

	//�ָ�����
	status = ((PCREATEPROCESSINTERNALW)pFunc)(hToken,lpApplicationName,lpCommandLine,lpProcessAttributes,
											lpThreadAttributes,bInheritHandles,
											dwCreationFlags,lpEnvironment,
											lpCurrentDirectory,
											lpStartupInfo,
											lpProcessInformation,
											hNewToken);
	if(!status)
	{
		wprintf(L"CreateProcessInternalW Error: %n\n",GetLastError());
		return FALSE;
	}

	
	hook_by_code("kernel32.dll","CreateProcessInternalW",(PROC)NewCreateProcessInternalW,g_pCPI);
  
    return TRUE;
}
*/