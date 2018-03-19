#ifndef HOOK_H_INCLUDED
#define HOOK_H_INCLUDED


#endif // HOOK_H_INCLUDED
#include <windows.h>
#include <wininet.h>


//unsigned char g_pCPI[5] = {0,};
BOOL hook_by_code(LPCSTR szDllName,LPCSTR szFunName,PROC pfnNew,PBYTE pOrgBytes);

BOOL unhook_by_code(LPCSTR szDllName,LPCSTR szFuncName,PBYTE pOrgBytes);
HINTERNET WINAPI NewInternetConnectW(HINTERNET hInternet,
                                     LPCWSTR lpszServerName,
                                     INTERNET_PORT nServerPort,
                                     LPCTSTR lpszUsername,
                                     LPCTSTR lpszPassword,
                                     DWORD dwService,
                                     DWORD dwFlags,
                                     DWORD dwContext);
typedef HINTERNET (WINAPI *ICW)(HINTERNET hInternet,
                                     LPCWSTR lpszServerName,
                                     INTERNET_PORT nServerPort,
                                     LPCTSTR lpszUsername,
                                     LPCTSTR lpszPassword,
                                     DWORD dwService,
                                     DWORD dwFlags,
                                     DWORD dwContext);
typedef BOOL (WINAPI *PCREATEPROCESSINTERNALW)(HANDLE hToken,
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
											PHANDLE hNewToken);