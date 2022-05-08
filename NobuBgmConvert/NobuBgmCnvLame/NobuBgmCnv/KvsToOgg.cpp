
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <windows.h>

#include "OggToWav.h"

// kvsファイルの名前を基に、出力するoggファイルの名前を決める。
void GetOggFileName(char *pBufOggFileName, char *pBufKvsFileName ) {
	strcpy(pBufOggFileName, pBufKvsFileName);

	// 拡張子に.kvsが含まれていたら…
	if ( strstr(pBufKvsFileName, ".kvs") ) {
		// .oggと変更する。
		strcpy(pBufOggFileName+strlen(pBufOggFileName)-4, ".ogg");

	}
	// 含まれていなければ
	else {
		// 拡張子を付け加える。
		strcat(pBufOggFileName, ".ogg");
	}
}

// Kvs2Oggのサンプルソースほぼまんま
int KvsToOgg (char *szKvsFileName) {
	FILE *input;
	FILE *output;
	char *buf,out;
	unsigned long ogg_size;
	unsigned long loop_point;
	int i;

	//Open Kvs File
	input = fopen(szKvsFileName, "rb");

	char szOggFileName[256] = "";
	GetOggFileName( szOggFileName, szKvsFileName );
	//Open Ogg File
	output = fopen(szOggFileName , "wb");

	// 読めなかったら出てく
	if(!input || !output) {
		if ( input  ) { fclose(input); }
		if ( output ) { fclose(output); }
		return -1;
	}

	// 最初の16バイト分メモリ確保
	buf = (char *)malloc(16);

	// 16バイト文、.kvsファイルからコピー
	fread(buf, sizeof(char), 16, input);

	// 最初の４バイトがKOVSでなければ終了
	if(memcmp(buf,"KOVS",4) != 0) {
		// malloc の分解放
		free(buf);
		return 1;
	}

	//4バイト目から、ファイルサイズがある。get file size
	ogg_size = *(unsigned long*)(buf+4);

	// ogg_sizeが256バイトより小さかったらよくわからないので、終了
	if(ogg_size < 0xFF) {
		// malloc の分解放
		free(buf);
		return 1;
	}

	//get loop point
	loop_point = *(unsigned long*)(buf+8);

	// 32バイト目に移動
	fseek(input,0x20,SEEK_SET);

	// 最初の256バイト目まで変換処理必要
	for(i=0 ; i <= 0xFF ; i++)
	{
		// バイトを規則に従って変換
		fread(buf, sizeof(char), 1, input);
		out = (char)(*buf^i);
		// 書き込み
		fwrite(&out, sizeof(char), 1, output);
	}

	// それ以降は単純にコピー
	while((i = fread(buf, sizeof(char), 16, input )) > 0)
	{
		fwrite(buf, sizeof(char), i, output );
	}

	// malloc の分解放
	free(buf);

	// 両方とじる
	fclose(input);
	fclose(output);

	return 0;

}
