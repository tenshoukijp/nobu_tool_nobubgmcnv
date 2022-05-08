
#include <stdlib.h>
#include <string.h>


// 信長シリーズのものから、mp2のファイル名を得る
void GetMp2FileName(char *pDstFile, char *pSrcFile ) {
	strcpy(pDstFile, pSrcFile);

	// .wmaと変更する。
	strcpy(pDstFile+strlen(pDstFile)-4, ".mp2");
}
