#include <Windows.h>
#include <tchar.h>

//DLLע�룬����Զ���̣߳����߳�����Ŀ����̣�������Ŀ��DLL��Ŀ��DLL�ڴ����̣߳�ִ�й���(����վ�����ض���)
BOOL InjectDll(DWORD dwPID,LPCTSTR szDllPath)
{
	HANDLE hProcess = NULL,hThread = NULL;
	HMODULE hMod = NULL;
	LPVOID pRemoteBuf = NULL;
	DWORD dwBufSize = (DWORD)(wcslen(szDllPath)+1)*sizeof(TCHAR);
	LPTHREAD_START_ROUTINE pThreadProc;

	if(!(hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPID)))
	{
		wprintf(L"OpenProcess(%d) failed!!! [%d]\n",dwPID,GetLastError());
		return FALSE;
	}

	//�����ڴ棬�洢DLL����
	pRemoteBuf = VirtualAllocEx(hProcess,NULL,dwBufSize,MEM_COMMIT,PAGE_READWRITE);

	WriteProcessMemory(hProcess,pRemoteBuf,(LPVOID)szDllPath,dwBufSize,NULL);

	hMod = GetModuleHandle(L"kernel32.dll");
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod,"LoadLibraryW");
	hThread = CreateRemoteThread(hProcess,NULL,0,pThreadProc,pRemoteBuf,0,NULL);
	WaitForSingleObject(hThread,INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	return TRUE;
}

#define PATH (L"F:\\workdoc\\VS\\Rdll\\x64\\Release\\Rdll.dll")
int main()
{
	if(InjectDll(1568,PATH))
		wprintf(L"InjectionDll(\"%s\") success!!!\n",PATH);
	else
		wprintf(L"InjectionDll(\"%s\") failed!!!\n",PATH);
	return 0;
}