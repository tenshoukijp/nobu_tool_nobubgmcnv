#include <windows.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "SrcToMp3.h"
#include "LameIF.h"


static char *pargv[8] = {NULL};
void CommandLame( char *szMySelf, char *szSrcFileName ) {
	char *szBitrate = "-V0";

	pargv[0] = szMySelf;
	pargv[1] = szBitrate;
	pargv[2] = szSrcFileName ;

	char szDstFileName[MAX_PATH];
	GetMp3FileName( szDstFileName, szSrcFileName );
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
