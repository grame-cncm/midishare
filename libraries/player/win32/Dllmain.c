
#include <windows.h>

#ifndef WIN32
//_________________________________________________________

int CALLBACK LibMain(HINSTANCE hInst,WORD wDataSeg,WORD wHeapSize,LPSTR cmdLine)
{
	return 1;
}

//_________________________________________________________

int CALLBACK WEP(int nParam)
{
	return 1;
}
#else
//_________________________________________________________

BOOL WINAPI DllEntryPoint( HINSTANCE  hinstDLL, DWORD  fdwReason,LPVOID lpvReserved )
{
	return 1;
}

#endif

