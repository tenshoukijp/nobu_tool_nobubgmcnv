
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

// Ktsl2asbinファイル名から、Kvsのファイル名を得る
static void GetSequenceKvsFileName(char *pDstFile, char *pSrcFile, int iNo) {
	strcpy(pDstFile, pSrcFile);

	strcpy(pDstFile + strlen(pDstFile) - (strlen("Ktsl2asbin")+1), "");

	// .kvsと変更する。
	wsprintf(pDstFile, "%s_%03d.kvs", pDstFile, iNo);
}

static long long intgetFileSize(const char* fileName)
{
	FILE* fp = fopen(fileName, "rb");
	if (fp == NULL) {
		return -1LL;
	}

	if (fseek(fp, 0L, SEEK_END) == 0) {
		fpos_t pos;

		if (fgetpos(fp, &pos) == 0) {
			fclose(fp);
			return (long long)pos;
		}
	}

	fclose(fp);
	return -1LL;
}

int Ktsl2asbinToKvs(char *szKtsl2asbinFileName) {


	string filepath = szKtsl2asbinFileName;
	_int64 data_size = intgetFileSize(szKtsl2asbinFileName);

	ifstream fin(filepath, ios::in | ios::binary);

	char* pdata = new char[data_size];
	try {
		fin.read(pdata, data_size);

		vector<char> kovs;
		int find_count = 0;
		for (int pix = 0; pix < data_size; pix++) {
			// KOVS という文字列相当を検索する
			if (pix < data_size - strlen("KOVS")) {
				// 次のファイルを見つけた
				if (pdata[pix] == 'K' && pdata[pix + 1] == 'O' && pdata[pix + 2] == 'V' && pdata[pix + 3] == 'S') {

					if (!kovs.empty() && kovs[0] == 'K' && kovs[1] == 'O' && kovs[2] == 'V' && kovs[3] == 'S') {
						char newfilename[1024] = "";
						GetSequenceKvsFileName(newfilename, szKtsl2asbinFileName, find_count);
						ofstream fout(newfilename, ios::out | ios::binary);
						fout.write(&kovs[0], kovs.size());
						fout.close();

						kovs.clear();

						// 曲の先頭を見つけたよのカウント
						find_count++;
					}

					kovs.push_back(pdata[pix]);
				}
				else {
					kovs.push_back(pdata[pix]);

					// ファイルの最後だ
					if (pix == data_size - 1) {
						char newfilename[1024] = "";
						GetSequenceKvsFileName(newfilename, szKtsl2asbinFileName, find_count);
						ofstream fout(newfilename, ios::out | ios::binary);
						fout.write(&kovs[0], kovs.size());
						fout.close();

					}
				}
			}
		}

		fin.close(); // コピーしたらすぐ閉じる

		if (pdata) {
			delete[] pdata;
		}
	}
	catch (exception) {
		if (pdata) {
			delete[] pdata;
		}
	}

	return 0;
}

