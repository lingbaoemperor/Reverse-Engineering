#include"file.h"

BOOL File::EditFileName()
{
	OPENFILENAMEW ofn = {0};
	WCHAR szFile[NAMESIZE] = {0,};
	ofn.lpstrFile = szFile;
	ofn.lStructSize = sizeof(OPENFILENAMEW);
	ofn.hwndOwner = NULL;	//非模态
	ofn.lpstrFilter = L"文本文件(*.txt)/0*.txt/0C/C++源文件(*.cpp;*.c;*.h)/0*.cpp;*.c;*.h/0All Files(*.*)/0*.*/0/0";
	ofn.nFilterIndex = 1;
	ofn.nMaxFile = sizeof(szFile);	//文件名缓冲
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = TEXT("Save File");
	ofn.Flags = OFN_PATHMUSTEXIST;
	if(!GetSaveFileNameW(&ofn))
		return FALSE;
	hFile = CreateFileW(szFile,GENERIC_WRITE | GENERIC_READ,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	return TRUE;
}

BOOL File::Write(LPVOID lpBuf,DWORD size)
{
	DWORD size_write = 0;
	if(!WriteFile(hFile,lpBuf,size,&size_write,NULL))
		return FALSE;
	if(size_write != size) return FALSE;
	FlushFileBuffers(hFile);
	CloseHandle(hFile);
	return TRUE;
}