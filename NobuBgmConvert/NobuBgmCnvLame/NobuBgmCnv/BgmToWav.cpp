
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>


// 嵐世記タイプのもの

// 信長シリーズのものから、wavのファイル名を得る
static void GetWavFileName(char *pDstFile, char *pSrcFile ) {
	strcpy(pDstFile, pSrcFile);

	// .wmaと変更する。
	strcpy(pDstFile+strlen(pDstFile)-4, ".wav");
}


BYTE WavHeader[]={0x52,0x49,0x46,0x46,0xC8,0xB6,0xC6,0x00,0x57,0x41,0x56,0x45,0x66,0x6D,0x74,0x20,0x10,0x00,0x00,0x00,0x01,0x00,0x02,0x00,0x44,0xAC,0x00,0x00,0x10,0xB1,0x02,0x00,0x04,0x00,0x10,0x00,0x64,0x61,0x74,0x61};
void BgmToWav(char *szBgmFileName) {
	// 入力ファイル名から出力ファイル名を定める
	char szWavFileName[256] = "";
	GetWavFileName( szWavFileName, szBgmFileName );

	// 入力ファイルと出力ファイル。両方バイナリ
	FILE *fpBGM;
	fpBGM = fopen(szBgmFileName, "rb");
	FILE *fpWAV;
	fpWAV = fopen(szWavFileName, "wb");

	// なんか失敗してたら閉じてリターン
	if ( !fpBGM || !fpWAV ) {
		if ( fpBGM ) { fclose(fpBGM); }
		if ( fpWAV ) { fclose(fpWAV); }
		return;
	}

	// ヘッダーのサイズ
	int iWaveHeaderSize = sizeof(WavHeader)/sizeof(WavHeader[0]);

	// 出力ファイルにヘッダー分を書き出す
	fwrite(WavHeader, sizeof(BYTE), iWaveHeaderSize, fpWAV);

	// 入力ファイルのＢＧＭは、ヘッダーのサイズ分飛ばす
	fseek(fpBGM, iWaveHeaderSize, SEEK_SET);

   /* binaryファイルのコピー */
	char charbuf[4096];
	int readItems;
	while((readItems = fread(&charbuf, 1, sizeof(charbuf)/sizeof(charbuf[0]), fpBGM)) != 0) {
		fwrite(&charbuf, 1, readItems, fpWAV);
   }
	
	// 両方クロース
    fclose(fpBGM);
    fclose(fpWAV);
}