
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <vector>

using namespace std;

// Vfpファイル名から、Kvsのファイル名を得る
static void GetSequenceKvsFileName(char *pDstFile, char *pSrcFile, int iNo ) {
	strcpy(pDstFile, pSrcFile);

	strcpy(pDstFile+strlen(pDstFile)-4, "");

	// .kvsと変更する。
	wsprintf(pDstFile, "%s_%03d.kvs", pDstFile, iNo);
}

int VfpToKvs(char *szVpsFileName ) {

	FILE *fpVFP;

	fpVFP = fopen(szVpsFileName, "rb");

	int iKovsCnt = 100;
	vector<int> vArray; // １つのVPSファイルの中には、複数のKOVSコンテンツが詰まってる。それぞれのKOVSコンテンツの先頭のアドレスをvectorとして集める。

	int i = 0;
	if ( fpVFP ) {

		// まずはKovsコンポーネントの数を確保
		int inbuf;
		while ( fread(&inbuf, sizeof(int), 1, fpVFP ) ) { // 4バイト=int で１つずつのアドレス

			// index1はKovsコンテンツの個数
			if ( i==1 ) {
				iKovsCnt = inbuf;
			}

			if ( i==2 ) {
				break;
			}

			i++;
		}

		i = 0;
		while ( fread(&inbuf, sizeof(int), 1, fpVFP ) ) { // 4バイト=int で１つずつのアドレス

			if ( i < iKovsCnt ) {
				vArray.push_back(inbuf);
			}

			else {
				break;
			}

			i++;
		}

		fseek( fpVFP, 0, SEEK_END );
		vArray.push_back( (int)(ftell( fpVFP )) ); // ファイルのサイズを最後のアドレスとする。

		// 格アドレスのKOVSコンポーネントをファイルに吐き出してゆく
		for( int v=0; v<(int)vArray.size()-1; v++ ) {

			FILE *fpOutKvs;
			char szKvsFileName[256] = "";
			GetSequenceKvsFileName(szKvsFileName, szVpsFileName, v+1);

			fpOutKvs = fopen(szKvsFileName, "wb");

			int iKovsSize = vArray[v+1]-vArray[v]; // Kovsコンポーネントのサイズ
			fseek( fpVFP, vArray[v], SEEK_SET); // 該当のKovsコンポーネントの位置に移動

			// Kovsコンポーネントのサイズ分確保
			char *pBuf = (char *)malloc(iKovsSize);

			// サイズ分読み込み
			fread(pBuf, sizeof(char), iKovsSize, fpVFP );

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

	if ( fpVFP ) {
		fclose(fpVFP);
	}

	return 0;
}

	