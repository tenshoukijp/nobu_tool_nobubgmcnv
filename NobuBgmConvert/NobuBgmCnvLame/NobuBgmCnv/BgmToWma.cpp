
#include <stdlib.h>
#include <string.h>


// 信長シリーズのものから、wmaのファイル名を得る
void GetWmaFileName(char *pDstFile, char *pSrcFile ) {
	strcpy(pDstFile, pSrcFile);

	// .wmaと変更する。
	strcpy(pDstFile+strlen(pDstFile)-4, ".wma");
}
