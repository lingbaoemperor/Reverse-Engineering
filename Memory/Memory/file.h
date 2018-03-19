#include <Windows.h>
#define NAMESIZE 150
class File
{
public:
	HANDLE hFile;
public:
	BOOL EditFileName();
	BOOL Write(LPVOID,DWORD);
};