
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <vector>

using namespace std;

// Atslwファイル名から、Kvsのファイル名を得る
static void GetSequenceKvsFileName(char *pDstFile, char *pSrcFile, int iNo ) {
	strcpy(pDstFile, pSrcFile);

	strcpy(pDstFile+strlen(pDstFile)-6, "");

	// .kvsと変更する。
	wsprintf(pDstFile, "%s_%03d.kvs", pDstFile, iNo);
}

int AtslwToKvs(char *szAtslwFileName ) {

	FILE *fpATSLW;

	fpATSLW = fopen(szAtslwFileName, "rb");

	int iKovsCnt = 2;
	vector<int> vArray; // １つのVPSファイルの中には、複数のKOVSコンテンツが詰まってる。それぞれのKOVSコンテンツの先頭のアドレスをvectorとして集める。

	int i = 0;
	if ( fpATSLW ) {

		// ATSLWには、とりあえず、Kovsコンポーネントが2個で固定的な模様
		int inbuf;

		fseek( fpATSLW, 0x2C, SEEK_SET);
		fread(&inbuf, sizeof(int), 1, fpATSLW ); // 4バイト=int で１つずつのアドレス
		vArray.push_back(inbuf);

		fseek( fpATSLW, 0x54, SEEK_SET);
		fread(&inbuf, sizeof(int), 1, fpATSLW ); // 4バイト=int で１つずつのアドレス
		vArray.push_back(inbuf);

		fseek( fpATSLW, 0, SEEK_END );
		vArray.push_back( (int)(ftell( fpATSLW )) ); // ファイルのサイズを最後のアドレスとする。

		// 格アドレスのKOVSコンポーネントをファイルに吐き出してゆく
		for( int v=0; v<(int)vArray.size()-1; v++ ) {

			FILE *fpOutKvs;
			char szKvsFileName[256] = "";
			GetSequenceKvsFileName(szKvsFileName, szAtslwFileName, v+1);

			fpOutKvs = fopen(szKvsFileName, "wb");

			int iKovsSize = vArray[v+1]-vArray[v]; // Kovsコンポーネントのサイズ
			fseek( fpATSLW, vArray[v], SEEK_SET); // 該当のKovsコンポーネントの位置に移動

			// Kovsコンポーネントのサイズ分確保
			char *pBuf = (char *)malloc(iKovsSize);

			// サイズ分読み込み
			fread(pBuf, sizeof(char), iKovsSize, fpATSLW );

			// そのままファイルとして吐き出す
			fwrite(pBuf, sizeof(char), iKovsSize, fpOutKvs );

			if ( pBuf ) {
				free(pBuf);
			}

			if ( fpOutKvs ) {
				fclose(fpOutKvs);
			}
		}
	}

	if ( fpATSLW ) {
		fclose(fpATSLW);
	}

	return 0;
}

	