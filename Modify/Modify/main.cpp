#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <iterator>
using namespace std;
vector<DWORD> vec1;
vector<DWORD> vec2;
vector<DWORD> vec;
DWORD lf;

void Common()
{
    sort(vec1.begin(),vec1.end());     
    sort(vec2.begin(),vec2.end());     
    set_intersection(vec1.begin(),vec1.end(),vec2.begin(),vec2.end(),back_inserter(vec));//求交集
	vec2.clear();
	vec2.assign(vec.begin(),vec.end());
}

void Unique()
{
	vector<DWORD>::iterator iter;
    sort(vec1.begin(),vec1.end());
    iter = unique(vec1.begin(),vec1.end());
    if(iter != vec1.end()){
        vec1.erase(iter,vec.end());
    }

	for(iter=vec1.begin(); iter!=vec1.end(); ++iter)
		cout<<"0x" <<setw(8)<<setfill('0')<<hex<<*iter<<endl;
    cout<<"1------------------------------------------" <<endl;

	if(vec2.empty())
		vec2.assign(vec1.begin(),vec1.end());

	for(iter=vec2.begin(); iter!=vec2.end(); ++iter)
		cout<<"0x" <<setw(8)<<setfill('0')<<hex<<*iter<<endl;
    cout<<"2------------------------------------------" <<endl;
}

void pt()
{
	vector<DWORD>::iterator iter;
	for(iter=vec.begin(); iter!=vec.end(); ++iter)
		cout<<"0x" <<setw(8)<<setfill('0')<<hex<<*iter<<endl;
    cout<<"------------------------------------------" <<endl;
}
void Check(LPVOID lpBuf,SIZE_T size,DWORD baseAddress)
{
	PUCHAR p = (PUCHAR)lpBuf;
	PDWORD q = NULL;
	for(int i=0;i < size-4;i++)
	{
		q = (PDWORD)(p+i);
		if(*q == lf)
		{
			vec1.push_back(baseAddress+i);
		}
	}
}

BOOL ReadMem(DWORD dwPID)
{
	HANDLE hProcess = NULL,hThread = NULL;
	HMODULE hMod = NULL;
	LPVOID pRemoteBuf = NULL;
	if(!(hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPID)))
	{
		wprintf(L"OpenProcess(%d) failed!!! [%d]\n",dwPID,GetLastError());
		return FALSE;
	}

	DWORD startAddress = 0;
	//查询内存
	while(startAddress >= 0 && startAddress < 0x7fff0000)
	{
		MEMORY_BASIC_INFORMATION mbi = {0};
		SIZE_T size = 0;
		size = VirtualQueryEx(hProcess,(LPVOID)startAddress,&mbi,sizeof(MEMORY_BASIC_INFORMATION));
		if(size != sizeof(MEMORY_BASIC_INFORMATION))
		{
			wprintf(L"0x%x\n",startAddress);
			wprintf(L"Query Error: %d\n",GetLastError());
			CloseHandle(hProcess);
			return FALSE;
		}
		//wprintf(L"%d\n",mbi.RegionSize);
		if(mbi.RegionSize == 0) break;
		if(mbi.Protect == PAGE_READWRITE && mbi.State == MEM_COMMIT)
		{
			LPVOID lpBuf = (LPVOID)malloc(mbi.RegionSize);
			SIZE_T read = 0;
			BOOL b = FALSE;
			b = ReadProcessMemory(hProcess,(LPVOID)startAddress,lpBuf,mbi.RegionSize,&read);
			if(read != mbi.RegionSize || !b)
			{
				wprintf(L"%d %d\n",read,mbi.RegionSize);
				wprintf(L"Read Error: %d\n",GetLastError());
				CloseHandle(hProcess);
				free(lpBuf);
				return FALSE;
			}
			Check(lpBuf,read,startAddress);
			free(lpBuf);
		}
		startAddress += mbi.RegionSize;
	}

	//riteProcessMemory(hProcess,pRemoteBuf,(LPVOID)szDllPath,dwBufSize,NULL);
	CloseHandle(hProcess);
	return TRUE;
}

BOOL Modify(DWORD dwPID)
{
	DWORD addr = 0;
	cout <<"Modify Address:" <<endl;
	cin >>hex >>addr;
	HANDLE hProcess = NULL;
	if(!(hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPID)))
	{
		wprintf(L"OpenProcess(%d) failed!!! [%d]\n",dwPID,GetLastError());
		return FALSE;
	}
	char s[5] = {30,0,0,0,0};
	DWORD padd = addr;

	SIZE_T size = 0;
	BOOL b = WriteProcessMemory(hProcess,(LPVOID)padd,s,4,&size);
	if(!b)
	{
		wprintf(L"Write Error : %d\n",GetLastError());
		return FALSE;
	}
	CloseHandle(hProcess);
}
int main()
{
	DWORD pid = 0;
	cin >>pid;

	vec2.clear();

	while(true)
	{
		vec1.clear();
		vec.clear();
		cout <<" Left Lives :" <<endl;
		cin >>lf;
		ReadMem(pid);

		Unique();
		Common();

		cout <<"Print result? 1/0:" <<endl;
		int i = -1,j = -1;
		cin >>i;
		if(i == 1)
			pt();
		cout <<"Modify ??? 1/0 :" <<endl;
		cin >>j;
		if(j == 1)
			Modify(pid);
	}
	//Modify(pid);
	system("pause");
	return 0;
}