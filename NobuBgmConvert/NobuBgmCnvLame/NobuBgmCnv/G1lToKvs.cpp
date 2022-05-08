
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



// 信長シリーズのものから、mp2のファイル名を得る
static void GetKvsFileName(char *pDstFile, char *pSrcFile ) {
	strcpy(pDstFile, pSrcFile);

	// .kvsと変更する。
	strcpy(pDstFile+strlen(pDstFile)-4, ".kvs");
}

// G1lToKvsへ 
// 先頭の28バイト、つまりKOVSという文字列の前をばっさり削除するだけでKVSファイルとなる。
int G1lToKvs (char *szG1lFileName) {
	FILE *input;
	FILE *output;
	char buf[4096];
	int i;


	//Open G1l File
	input = fopen(szG1lFileName, "rb");

	char szKvsFileName[256] = "";
	GetKvsFileName( szKvsFileName, szG1lFileName );

	//Open Kvs File
	output = fopen(szKvsFileName , "wb");

	// 読めなかったら出てく
	if(!input || !output) {
		if ( input  ) { fclose(input); }
		if ( output ) { fclose(output); }
		return -1;
	}

	// 28バイト目に移動
	fseek(input,28,SEEK_SET);

	// それ以降は単純にコピー
	while((i = fread(buf, sizeof(char), sizeof(buf), input )) > 0)
	{
		fwrite(buf, sizeof(char), i, output );
	}

	// 両方とじる
	fclose(input);
	fclose(output);

	return 0;

}
