#include <tchar.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <map>
#include "file.h"
using namespace std;
using namespace std;
template <class T>
class Mem
{
private:
	vector<T> v1;
	vector<T> v2;
	vector<T> v;
	DWORD dwPid;
	T num;		//要查找的目标值

	File file;

	void save_part_address(LPVOID lpBuff,SIZE_T read,DWORD startAddress);	//保存一次读取的数据到数组中，目标数值的地址
	void unique_address();					//去重
	void common_address();					//求交集
	BOOL ReadMem();							//读取内存
public:
	Mem();
	void setPid(DWORD dwPid){this ->dwPid = dwPid;}
	DWORD getPid(){return dwPid;}
	T getNum() {return num;}				//获取要查找的数值
	void setNum(T num) {this ->num = num;}
	BOOL WriteMem(DWORD addr,T dw);			//写入修改数据
	vector<T> OnceFinalResult();			//返回结果
	BOOL ShowMemState(map<DWORD,DWORD>&);	//内存状况
	BOOL SaveAsFile();
};
template <class T>
BOOL Mem<T>::SaveAsFile()
{
	file.EditFileName();

	HANDLE hProcess = NULL;
	HMODULE hMod = NULL;
	LPVOID pRemoteBuf = NULL;
	//进程打开失败
	if(!(hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPid)))
	{
		wprintf(L"OpenProcess(%d) failed!!! [%d]\n",dwPid,GetLastError());
		return FALSE;
	}

	DWORD startAddress = 0;		//起始地址
	//查询内存
	while(startAddress >= 0 && startAddress < 0x7fff0000)
	{
		MEMORY_BASIC_INFORMATION mbi = {0};
		SIZE_T size = 0;
		size = VirtualQueryEx(hProcess,(LPVOID)startAddress,&mbi,sizeof(MEMORY_BASIC_INFORMATION));
		//查询到的内存块信息大小错误
		if(size != sizeof(MEMORY_BASIC_INFORMATION))
		{
			wprintf(L"0x%x\n",startAddress);
			wprintf(L"Query Error: %d\n",GetLastError());
			CloseHandle(hProcess);
			return FALSE;
		}
		if(mbi.RegionSize == 0)			//内存块大小为0，结束循环
		{
			WCHAR s[20] ={0,};
			wsprintf(s,L"0x%x\n",startAddress);
			OutputDebugStringW(s);
			break;
		}
		//内存块可读可写，已占用
		if(mbi.Protect == PAGE_READWRITE && mbi.State == MEM_COMMIT)
		{
			LPVOID lpBuf = (LPVOID)malloc(mbi.RegionSize);		//分配缓冲
			SIZE_T read = 0;
			BOOL b = FALSE;
			b = ReadProcessMemory(hProcess,(LPVOID)startAddress,lpBuf,mbi.RegionSize,&read);
			//读出的大小与内存大小不相等或是读取失败
			if(read != mbi.RegionSize || !b)
			{
				wprintf(L"%d %d\n",read,mbi.RegionSize);
				wprintf(L"Read Error: %d\n",GetLastError());
				CloseHandle(hProcess);
				free(lpBuf);
				return FALSE;
			}
			if(!file.Write(lpBuf,read))
			{
				free(lpBuf);
				return FALSE;
			}
		}
		startAddress += mbi.RegionSize;		//读下一块内存
	}
	CloseHandle(hProcess);
	return TRUE;
}
template <class T>
BOOL Mem<T>::ShowMemState(map<DWORD,DWORD>& mp)
{
	HANDLE hProcess = NULL;
	HMODULE hMod = NULL;
	LPVOID pRemoteBuf = NULL;
	//进程打开失败
	if(!(hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPid)))
	{
		wprintf(L"OpenProcess(%d) failed!!! [%d]\n",dwPid,GetLastError());
		return FALSE;
	}

	DWORD startAddress = 0;		//起始地址
	//查询内存
	while(startAddress >= 0 && startAddress < 0x7fff0000)
	{
		MEMORY_BASIC_INFORMATION mbi = {0};
		SIZE_T size = 0;
		size = VirtualQueryEx(hProcess,(LPVOID)startAddress,&mbi,sizeof(MEMORY_BASIC_INFORMATION));
		//查询到的内存块信息大小错误
		if(size != sizeof(MEMORY_BASIC_INFORMATION))
		{
			wprintf(L"0x%x\n",startAddress);
			wprintf(L"Query Error: %d\n",GetLastError());
			CloseHandle(hProcess);
			return FALSE;
		}
		if(mbi.RegionSize == 0)			//内存块大小为0，结束循环
		{
			WCHAR s[20] ={0,};
			wsprintf(s,L"0x%x\n",startAddress);
			OutputDebugStringW(s);
			break;
		}
		//内存块可读可写，已占用
		if(mbi.Protect == PAGE_READWRITE && mbi.State == MEM_COMMIT)
		{
			LPVOID lpBuf = (LPVOID)malloc(mbi.RegionSize);		//分配缓冲
			SIZE_T read = 0;
			BOOL b = FALSE;
			b = ReadProcessMemory(hProcess,(LPVOID)startAddress,lpBuf,mbi.RegionSize,&read);
			//读出的大小与内存大小不相等或是读取失败
			if(read != mbi.RegionSize || !b)
			{
				wprintf(L"%d %d\n",read,mbi.RegionSize);
				wprintf(L"Read Error: %d\n",GetLastError());
				CloseHandle(hProcess);
				free(lpBuf);
				return FALSE;
			}
			//位置和大小
			mp[startAddress] = read;
			free(lpBuf);
		}
		startAddress += mbi.RegionSize;		//读下一块内存
	}
	CloseHandle(hProcess);
	return TRUE;
}

template <class T>
BOOL Mem<T>::WriteMem(DWORD addr,T dw)
{
	HANDLE hProcess = NULL;
	if(!(hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPid)))
	{
		wprintf(L"OpenProcess(%d) failed!!! [%d]\n",dwPid,GetLastError());
		return FALSE;
	}

	SIZE_T size = 0;
	BOOL b = WriteProcessMemory(hProcess,(LPVOID)addr,(LPVOID)&dw,sizeof(T),&size);
	if(!b)
	{
		wprintf(L"Write Error : %d\n",GetLastError());
		return FALSE;
	}
	CloseHandle(hProcess);
	return TRUE;
}

template <class T>
vector<T> Mem<T>::OnceFinalResult()
{
	ReadMem();
	unique_address();
 	common_address();
	return v2;
}

template <class T>
Mem<T>::Mem()
{
	v1.clear();
	v2.clear();
	v.clear();
	dwPid = 0;
}
template <class T>
BOOL Mem<T>::ReadMem()
{
	HANDLE hProcess = NULL;
	HMODULE hMod = NULL;
	LPVOID pRemoteBuf = NULL;
	//进程打开失败
	if(!(hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPid)))
	{
		wprintf(L"OpenProcess(%d) failed!!! [%d]\n",dwPid,GetLastError());
		return FALSE;
	}

	DWORD startAddress = 0;		//起始地址
	//查询内存
	while(startAddress >= 0 && startAddress < 0x7fff0000)
	{
		MEMORY_BASIC_INFORMATION mbi = {0};
		SIZE_T size = 0;
		size = VirtualQueryEx(hProcess,(LPVOID)startAddress,&mbi,sizeof(MEMORY_BASIC_INFORMATION));
		//查询到的内存块信息大小错误
		if(size != sizeof(MEMORY_BASIC_INFORMATION))
		{
			WCHAR s[50] = {0,};
			wsprintf(s,L"0x%x,%d,%d\n",startAddress,size,sizeof(MEMORY_BASIC_INFORMATION));
			::OutputDebugStringW(s);
			CloseHandle(hProcess);
			return FALSE;
		}
		//wprintf(L"%d\n",mbi.RegionSize);
		if(mbi.RegionSize == 0) break;			//内存块大小为0，结束循环
		//内存块可读可写，已占用
		if(mbi.Protect == PAGE_READWRITE && mbi.State == MEM_COMMIT)
		{
			LPVOID lpBuf = (LPVOID)malloc(mbi.RegionSize);		//分配缓冲
			SIZE_T read = 0;
			BOOL b = FALSE;
			b = ReadProcessMemory(hProcess,(LPVOID)startAddress,lpBuf,mbi.RegionSize,&read);
			//读出的大小与内存大小不相等或是读取失败
			if(read != mbi.RegionSize || !b)
			{
				wprintf(L"%d %d\n",read,mbi.RegionSize);
				wprintf(L"Read Error: %d\n",GetLastError());
				CloseHandle(hProcess);
				free(lpBuf);
				return FALSE;
			}
			//位置和大小
			WCHAR s[100] = {};
			//wsprintf(s,L"0x%x,%x\n",startAddress,read);
			wsprintf(s,L"%d\n",read);
			::OutputDebugStringW(s);
			//保留一次扫描结果
			save_part_address(lpBuf,read,startAddress);
			free(lpBuf);
		}
		startAddress += mbi.RegionSize;		//读下一块内存
	}

	//riteProcessMemory(hProcess,pRemoteBuf,(LPVOID)szDllPath,dwBufSize,NULL);
	CloseHandle(hProcess);
	return TRUE;
}

template<class T>
void Mem<T>::save_part_address(LPVOID lpBuf,SIZE_T size,DWORD startAddress)
{
	//WCHAR w[50] = {0,};
	//wsprintf(w,L"%d\n",startAddress);
	//OutputDebugString(w);
	PUCHAR p = (PUCHAR)lpBuf;
	T* q = NULL;
	for(int i=0;i < size-sizeof(T);i++)
	{
		q = (T*)(p+i);
		if(*q == num)
		{
			v1.push_back(startAddress+i);
		}
	}
}
template<class T>
void Mem<T>::unique_address()
{
	vector<T>::iterator iter;
    sort(v1.begin(),v1.end());				//此次读取的目标地址数据
	//去重
    iter = unique(v1.begin(),v1.end());
    if(iter != v1.end())
	{
        v1.erase(iter,v.end());
    }
}

template <class T>
void Mem<T>::common_address()
{
    sort(v1.begin(),v1.end());
	if(!v2.empty())					//v2有数据，即不是第一次读取
	{
		sort(v2.begin(),v2.end());
		set_intersection(v1.begin(),v1.end(),v2.begin(),v2.end(),back_inserter(v));			//求交集
	}
	else							//v2没有数据，即第一次读取，只有v1有数据
		v.assign(v1.begin(),v1.end());
	//把结果存到v2，留待下一次读取后，与v1里的新地址交集
	v2.clear();
	v2.assign(v.begin(),v.end());
	v1.clear();
	v.clear();
}