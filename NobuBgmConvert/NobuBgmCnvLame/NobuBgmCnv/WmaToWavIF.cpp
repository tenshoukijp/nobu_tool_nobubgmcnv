#include <windows.h>

#include <stdlib.h>
#include <string.h>
#include <cstdlib> // for mbstowcs()
#include <clocale> // for setlocale()
#include <cstring>
#include <iostream>

#include "Wma2WavIF.h"


// 信長シリーズのものから、wavのファイル名を得る
static void GetWavFileName(char *pDstFile, char *pSrcFile ) {
	strcpy(pDstFile, pSrcFile);

	// .wmaと変更する。
	strcpy(pDstFile+strlen(pDstFile)-4, ".wav");
}


static wchar_t *pargv[8] = {NULL};

extern bool safe_com_uninit(void);

void CommandWma2Wav( char *szMySelf, char *szSrcFileName ) {

	char szDstFileName[MAX_PATH];
	GetWavFileName( szDstFileName, szSrcFileName );

	wchar_t* wszMySelf = new wchar_t[MAX_PATH];
	mbstowcs( wszMySelf, szMySelf, MAX_PATH );

	wchar_t* wszSrcFileName = new wchar_t[MAX_PATH];
	mbstowcs( wszSrcFileName, szSrcFileName, MAX_PATH );

	wchar_t* wszDstFileName = new wchar_t[MAX_PATH];
	mbstowcs( wszDstFileName, szDstFileName, MAX_PATH );

	wchar_t* _i = L"-i";
	wchar_t* _o = L"-o";
	wchar_t* _f = L"-f";
	wchar_t* _s = L"-s";

	pargv[0] = wszMySelf;
	pargv[1] = _i;
	pargv[2] = wszSrcFileName;
	pargv[3] = _o;
	pargv[4] = wszDstFileName;
	pargv[5] = _f;
	pargv[6] = _s;

	// 有効な要素をカウント
	int argc = 0;
	for ( int i=0; i < sizeof(pargv)/sizeof(pargv[0]); i++ ) {
		if ( pargv[i] ) {
			argc++;
		}
	}

	wma2wav(argc, pargv);
	safe_com_uninit();
}
