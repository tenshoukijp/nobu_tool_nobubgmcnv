#include <windows.h>
#include <stdio.h>

struct TKVSHEADER {
	BYTE kovs[4];
	DWORD iDataSize;
	DWORD iLoopPoint;
	BYTE arrNullData[20];
};

int getFileSize(char *szName) {
	FILE *fp;
	char *fname = szName;
	long sz;

	fp = fopen(fname, "rb");
	if (fp == NULL) {
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	sz = ftell(fp);

	fclose(fp);

	return sz;
}


static void GetKvsFileName(char *pDstFile, char *pSrcFile) {
	strcpy(pDstFile, pSrcFile);

	// .kvsと変更する。
	strcpy(pDstFile + strlen(pDstFile) - 4, ".kvs");
}


int OggToKvs(char *src, char *dst) {
	char szSrc[MAX_PATH] = "";
	strcpy(szSrc, src);

	char szDst[MAX_PATH] = "";
	GetKvsFileName(szDst, szSrc); // ファイルの名前。ogg⇒kvs。Src⇒Dst

	//まずファイルがあるか、ファイルサイズはどう？
	int size = getFileSize(szSrc);
	if (size <= 0) {
		return -1;
	}
	
	//-------------------------------------------------------------------- -
	//kvs用のヘッダの構築
	TKVSHEADER header;
	memcpy(header.kovs, "KOVS", sizeof(header.kovs));
	header.iDataSize = size;
	header.iLoopPoint = 0; // 繰り返し位置。とりあえず0
	ZeroMemory(header.arrNullData, sizeof(header.arrNullData));

	BYTE *pData = new BYTE[size];

	FILE *ip = fopen(szSrc, "rb");
	if (!ip) { 
		delete[] pData;
		return -1;
	}
	fread(pData, size, 1, ip);
	fclose(ip);

	// 先頭データから0xFFまでの255分をXORする。
	for (int i = 0; i <= 0xFF; i++) {
		pData[i] ^= i;
	}

	FILE *op = fopen(szDst, "wb");
	if (!op) {
		delete[] pData;
		return -1;
	}
	fwrite(&header, sizeof(header), 1, op);
	fwrite(pData, size, 1, op);
	fclose(op);

	delete[] pData;

	return 0;
}





