
#include <stdlib.h>
#include <string.h>


// 信長シリーズのものから、mp3のファイル名を得る
void GetMp3FileName(char *pDstFile, char *pSrcFile ) {
	strcpy(pDstFile, pSrcFile);

	// .wmaと変更する。
	strcpy(pDstFile+strlen(pDstFile)-4, ".mp3");
}

