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
	T num;		//Ҫ���ҵ�Ŀ��ֵ

	File file;

	void save_part_address(LPVOID lpBuff,SIZE_T read,DWORD startAddress);	//����һ�ζ�ȡ�����ݵ������У�Ŀ����ֵ�ĵ�ַ
	void unique_address();					//ȥ��
	void common_address();					//�󽻼�
	BOOL ReadMem();							//��ȡ�ڴ�
public:
	Mem();
	void setPid(DWORD dwPid){this ->dwPid = dwPid;}
	DWORD getPid(){return dwPid;}
	T getNum() {return num;}				//��ȡҪ���ҵ���ֵ
	void setNum(T num) {this ->num = num;}
	BOOL WriteMem(DWORD addr,T dw);			//д���޸�����
	vector<T> OnceFinalResult();			//���ؽ��
	BOOL ShowMemState(map<DWORD,DWORD>&);	//�ڴ�״��
	BOOL SaveAsFile();
};
template <class T>
BOOL Mem<T>::SaveAsFile()
{
	file.EditFileName();

	HANDLE hProcess = NULL;
	HMODULE hMod = NULL;
	LPVOID pRemoteBuf = NULL;
	//���̴�ʧ��
	if(!(hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPid)))
	{
		wprintf(L"OpenProcess(%d) failed!!! [%d]\n",dwPid,GetLastError());
		return FALSE;
	}

	DWORD startAddress = 0;		//��ʼ��ַ
	//��ѯ�ڴ�
	while(startAddress >= 0 && startAddress < 0x7fff0000)
	{
		MEMORY_BASIC_INFORMATION mbi = {0};
		SIZE_T size = 0;
		size = VirtualQueryEx(hProcess,(LPVOID)startAddress,&mbi,sizeof(MEMORY_BASIC_INFORMATION));
		//��ѯ�����ڴ����Ϣ��С����
		if(size != sizeof(MEMORY_BASIC_INFORMATION))
		{
			wprintf(L"0x%x\n",startAddress);
			wprintf(L"Query Error: %d\n",GetLastError());
			CloseHandle(hProcess);
			return FALSE;
		}
		if(mbi.RegionSize == 0)			//�ڴ���СΪ0������ѭ��
		{
			WCHAR s[20] ={0,};
			wsprintf(s,L"0x%x\n",startAddress);
			OutputDebugStringW(s);
			break;
		}
		//�ڴ��ɶ���д����ռ��
		if(mbi.Protect == PAGE_READWRITE && mbi.State == MEM_COMMIT)
		{
			LPVOID lpBuf = (LPVOID)malloc(mbi.RegionSize);		//���仺��
			SIZE_T read = 0;
			BOOL b = FALSE;
			b = ReadProcessMemory(hProcess,(LPVOID)startAddress,lpBuf,mbi.RegionSize,&read);
			//�����Ĵ�С���ڴ��С����Ȼ��Ƕ�ȡʧ��
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
		startAddress += mbi.RegionSize;		//����һ���ڴ�
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
	//���̴�ʧ��
	if(!(hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPid)))
	{
		wprintf(L"OpenProcess(%d) failed!!! [%d]\n",dwPid,GetLastError());
		return FALSE;
	}

	DWORD startAddress = 0;		//��ʼ��ַ
	//��ѯ�ڴ�
	while(startAddress >= 0 && startAddress < 0x7fff0000)
	{
		MEMORY_BASIC_INFORMATION mbi = {0};
		SIZE_T size = 0;
		size = VirtualQueryEx(hProcess,(LPVOID)startAddress,&mbi,sizeof(MEMORY_BASIC_INFORMATION));
		//��ѯ�����ڴ����Ϣ��С����
		if(size != sizeof(MEMORY_BASIC_INFORMATION))
		{
			wprintf(L"0x%x\n",startAddress);
			wprintf(L"Query Error: %d\n",GetLastError());
			CloseHandle(hProcess);
			return FALSE;
		}
		if(mbi.RegionSize == 0)			//�ڴ���СΪ0������ѭ��
		{
			WCHAR s[20] ={0,};
			wsprintf(s,L"0x%x\n",startAddress);
			OutputDebugStringW(s);
			break;
		}
		//�ڴ��ɶ���д����ռ��
		if(mbi.Protect == PAGE_READWRITE && mbi.State == MEM_COMMIT)
		{
			LPVOID lpBuf = (LPVOID)malloc(mbi.RegionSize);		//���仺��
			SIZE_T read = 0;
			BOOL b = FALSE;
			b = ReadProcessMemory(hProcess,(LPVOID)startAddress,lpBuf,mbi.RegionSize,&read);
			//�����Ĵ�С���ڴ��С����Ȼ��Ƕ�ȡʧ��
			if(read != mbi.RegionSize || !b)
			{
				wprintf(L"%d %d\n",read,mbi.RegionSize);
				wprintf(L"Read Error: %d\n",GetLastError());
				CloseHandle(hProcess);
				free(lpBuf);
				return FALSE;
			}
			//λ�úʹ�С
			mp[startAddress] = read;
			free(lpBuf);
		}
		startAddress += mbi.RegionSize;		//����һ���ڴ�
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
	//���̴�ʧ��
	if(!(hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPid)))
	{
		wprintf(L"OpenProcess(%d) failed!!! [%d]\n",dwPid,GetLastError());
		return FALSE;
	}

	DWORD startAddress = 0;		//��ʼ��ַ
	//��ѯ�ڴ�
	while(startAddress >= 0 && startAddress < 0x7fff0000)
	{
		MEMORY_BASIC_INFORMATION mbi = {0};
		SIZE_T size = 0;
		size = VirtualQueryEx(hProcess,(LPVOID)startAddress,&mbi,sizeof(MEMORY_BASIC_INFORMATION));
		//��ѯ�����ڴ����Ϣ��С����
		if(size != sizeof(MEMORY_BASIC_INFORMATION))
		{
			WCHAR s[50] = {0,};
			wsprintf(s,L"0x%x,%d,%d\n",startAddress,size,sizeof(MEMORY_BASIC_INFORMATION));
			::OutputDebugStringW(s);
			CloseHandle(hProcess);
			return FALSE;
		}
		//wprintf(L"%d\n",mbi.RegionSize);
		if(mbi.RegionSize == 0) break;			//�ڴ���СΪ0������ѭ��
		//�ڴ��ɶ���д����ռ��
		if(mbi.Protect == PAGE_READWRITE && mbi.State == MEM_COMMIT)
		{
			LPVOID lpBuf = (LPVOID)malloc(mbi.RegionSize);		//���仺��
			SIZE_T read = 0;
			BOOL b = FALSE;
			b = ReadProcessMemory(hProcess,(LPVOID)startAddress,lpBuf,mbi.RegionSize,&read);
			//�����Ĵ�С���ڴ��С����Ȼ��Ƕ�ȡʧ��
			if(read != mbi.RegionSize || !b)
			{
				wprintf(L"%d %d\n",read,mbi.RegionSize);
				wprintf(L"Read Error: %d\n",GetLastError());
				CloseHandle(hProcess);
				free(lpBuf);
				return FALSE;
			}
			//λ�úʹ�С
			WCHAR s[100] = {};
			//wsprintf(s,L"0x%x,%x\n",startAddress,read);
			wsprintf(s,L"%d\n",read);
			::OutputDebugStringW(s);
			//����һ��ɨ����
			save_part_address(lpBuf,read,startAddress);
			free(lpBuf);
		}
		startAddress += mbi.RegionSize;		//����һ���ڴ�
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
    sort(v1.begin(),v1.end());				//�˴ζ�ȡ��Ŀ���ַ����
	//ȥ��
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
	if(!v2.empty())					//v2�����ݣ������ǵ�һ�ζ�ȡ
	{
		sort(v2.begin(),v2.end());
		set_intersection(v1.begin(),v1.end(),v2.begin(),v2.end(),back_inserter(v));			//�󽻼�
	}
	else							//v2û�����ݣ�����һ�ζ�ȡ��ֻ��v1������
		v.assign(v1.begin(),v1.end());
	//�ѽ���浽v2��������һ�ζ�ȡ����v1����µ�ַ����
	v2.clear();
	v2.assign(v.begin(),v.end());
	v1.clear();
	v.clear();
}