#ifndef __WINDOWS_H
#include <windows.h>
#endif



int FAR PASCAL LibMain(HINSTANCE hInst,WORD wDataSeg,WORD wHeapSize,LPSTR cmdLine)
{
	static BOOL hinstLib = FALSE;

	if (hinstLib == FALSE) {
		hinstLib = TRUE;
	}

	return TRUE;
}


//_________________________________________________________
// Fonction de desinitialisation de la DLL
int  FAR PASCAL _export WEP(int nParam)
{
	return TRUE;
}


#ifdef __IncludeThunking__
//_________________________________________________________
BOOL FAR PASCAL __export PLAYER_ThunkConnect16(LPSTR pszDll16,
    LPSTR pszDll32, WORD hInst, DWORD dwReason); 
 
//_________________________________________________________
BOOL FAR PASCAL __export DllEntryPoint(DWORD dwReason, WORD hInst,
		  WORD wDS, WORD wHeapSize, DWORD dwReserved1,
		  WORD wReserved2) {

	 if (!(PLAYER_ThunkConnect16("PLAYER.DLL", // name of 16-bit DLL
            "PLAYER32.DLL",                 // name of 32-bit DLL 
            hInst, dwReason))) { 
		  return FALSE;
	 }
	 return TRUE;
}

#endif

