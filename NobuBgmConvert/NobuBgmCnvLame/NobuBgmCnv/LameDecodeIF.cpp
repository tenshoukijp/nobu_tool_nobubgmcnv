#include <windows.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "SrcToMp3.h"
#include "LameIF.h"


// 信長シリーズのものから、wavのファイル名を得る
static void GetWavFileName(char *pDstFile, char *pSrcFile ) {
	strcpy(pDstFile, pSrcFile);

	// .wmaと変更する。
	strcpy(pDstFile+strlen(pDstFile)-4, ".wav");
}

static char *pargv[8] = {NULL};
void CommandDecodeLame( char *szMySelf, char *szSrcFileName ) {

	char *szDecode = "--decode";

	pargv[0] = szMySelf;
	pargv[1] = szDecode;
	pargv[2] = szSrcFileName ;

	char szDstFileName[MAX_PATH];
	GetWavFileName( szDstFileName, szSrcFileName );
	pargv[3] = szDstFileName;

	// 有効な要素をカウント
	int argc = 0;
	for ( int i=0; i < sizeof(pargv)/sizeof(pargv[0]); i++ ) {
		if ( pargv[i] ) {
			argc++;
		}
	}
	lame_wmain(argc, pargv);
}
