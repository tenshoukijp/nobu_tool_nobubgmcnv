#include <windows.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "AtslwToKvs.h"
#include "VfpToKvs.h"
#include "G1lToKvs.h"
#include "KvsToOgg.h"
#include "OggToWav.h"
#include "BgmToWma.h"
#include "BgmToWav.h"
#include "DatToMp2.h"
#include "OggToKvs.h"
#include "Ktsl2asbinToKvs.h"

#include "LameIF.h"
#include "Wma2WavIF.h"



int main(int argc, char *argv[]) {

	// 引数がある場合は、lameへ
	if ( argc >= 2 ) {
		// kvs への変換である。
		if (argc >= 4 && strcmp(argv[1], "kvs") == 0) {
			OggToKvs(argv[2], argv[3]);

		// 他はLameのコマンドとみなす。
		}
		else {

			lame_wmain(argc, argv);
		}
	} else {

		WIN32_FIND_DATA ffd;

		// 信長の野望・大志のBGM系
		HANDLE hFileList = FindFirstFile("*.ktsl2stbin", &ffd);
		// 存在した場合、変換対象とする。
		if (hFileList != INVALID_HANDLE_VALUE) {
			do {
				Ktsl2asbinToKvs(ffd.cFileName);
			} while (FindNextFile(hFileList, &ffd)); // 次の.vpsへ
		}
		FindClose(hFileList); // ハンドル終了

		// 信長の野望・大志のVOICE系
		hFileList = FindFirstFile("*.ktsl2asbin", &ffd);
		// 存在した場合、変換対象とする。
		if (hFileList != INVALID_HANDLE_VALUE) {
			do {
				Ktsl2asbinToKvs(ffd.cFileName);
			} while (FindNextFile(hFileList, &ffd)); // 次の.vpsへ
		}
		FindClose(hFileList); // ハンドル終了

		// 信長の野望・創造のボイス系
		hFileList = FindFirstFile("*.vfp", &ffd);
		// 存在した場合、変換対象とする。
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				VfpToKvs( ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // 次の.vpsへ
		}
		FindClose(hFileList); // ハンドル終了

		// 信長の野望・天道のボイス系
		hFileList = FindFirstFile("*.g1l", &ffd);
		// 存在した場合、変換対象とする。
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				G1lToKvs( ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // 次の.g1lへ
		}
		FindClose(hFileList); // ハンドル終了

		// 信長の野望・創造で、１つに２つのKOVSファイルが入っているもの
		hFileList = FindFirstFile("*.atslw", &ffd);
		// 存在した場合、変換対象とする。
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				AtslwToKvs( ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // 次の.g1lへ
		}
		FindClose(hFileList); // ハンドル終了

		// *.kvsという名前のファイルのリストへのハンドラ
		hFileList = FindFirstFile("*.kvs", &ffd);

		// 存在した場合、変換対象とする。
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				KvsToOgg( ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // 次の.kvsへ
		}
		FindClose(hFileList); // ハンドル終了


		// *.oggという名前のファイルのリストへのハンドラ
		hFileList = FindFirstFile("*.ogg", &ffd);
		// 存在した場合、変換対象とする。
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				OggToWav( ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // 次の.oggへ
		}
		FindClose(hFileList); // ハンドル終了

		// 信長の野望・天翔記 DVD版
		hFileList = FindFirstFile("??.dat", &ffd);
		// 存在した場合、変換対象とする。
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				// ??部分は２ケタの数字であるはず。
				char szNo[3] = "";
				strncpy( szNo, ffd.cFileName, 2);
				// １以上の数時の場合のみ。
				if ( atoi(szNo) ) {
					char szMp2FileName[256];
					GetMp2FileName( szMp2FileName, ffd.cFileName );
					CopyFile( ffd.cFileName, szMp2FileName, false  );
				}				
			} while (FindNextFile(hFileList, &ffd)); // 次の.bgmへ
		}
		FindClose(hFileList); // ハンドル終了

		// 信長の野望・蒼天録
		hFileList = FindFirstFile("bgm??.nbx", &ffd);
		// 存在した場合、変換対象とする。
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				char szWmaFileName[256];
				GetWmaFileName( szWmaFileName, ffd.cFileName );
				CopyFile( ffd.cFileName, szWmaFileName, false  );
			} while (FindNextFile(hFileList, &ffd)); // 次の.nbxへ
		}
		FindClose(hFileList); // ハンドル終了
		

		// 信長の野望・天下創世
		hFileList = FindFirstFile("bgm??.n11", &ffd);
		// 存在した場合、変換対象とする。
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				char szWmaFileName[256];
				GetWmaFileName( szWmaFileName, ffd.cFileName );
				CopyFile( ffd.cFileName, szWmaFileName, false  );
			} while (FindNextFile(hFileList, &ffd)); // 次の.n11へ
		}
		FindClose(hFileList); // ハンドル終了
		
		// 信長の野望・嵐世記
		hFileList = FindFirstFile("m??.bgm", &ffd);
		// 存在した場合、変換対象とする。
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				// m?? の??部分は２ケタの数字であるはず。
				char szNo[3] = "";
				strncpy( szNo, ffd.cFileName+1, 2);
				// １以上の数時の場合のみ。
				if ( atoi(szNo) ) {
					BgmToWav(ffd.cFileName);
				}				
			} while (FindNextFile(hFileList, &ffd)); // 次の.bgmへ
		}
		FindClose(hFileList); // ハンドル終了

	}


	{
		// wmaを全部 wavへ変換
		WIN32_FIND_DATA ffd;
		// *.wavという名前のファイルのリストへのハンドラ
		HANDLE hFileList = FindFirstFile("*.wma", &ffd);
		if ( hFileList != INVALID_HANDLE_VALUE ) {

			do {
				CommandWma2Wav( argv[0], ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // 次の.wmaへ

		}
		FindClose(hFileList); // ハンドル終了

		/*
		// wavを全部 mp3へ変換
		// *.wavという名前のファイルのリストへのハンドラ
		hFileList = FindFirstFile("*.wav", &ffd);
		if ( hFileList != INVALID_HANDLE_VALUE ) {

			do {
				CommandLame( argv[0], ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // 次の.mp3へ

		}
		FindClose(hFileList); // ハンドル終了
		*/

		// 以下２つは要注意。あえて、mp2から直接wav, mp3 へとコンバートしている。
		// mp2を全部 wavへ変換
		// *.mp2という名前のファイルのリストへのハンドラ
		hFileList = FindFirstFile("*.mp2", &ffd);
		if ( hFileList != INVALID_HANDLE_VALUE ) {

			do {
				CommandDecodeLame( argv[0], ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // 次の.mp2へ

		}
		FindClose(hFileList); // ハンドル終了

		// mp2を全部 mp3へ変換
		// *.mp2という名前のファイルのリストへのハンドラ
		hFileList = FindFirstFile("*.mp2", &ffd);
		if ( hFileList != INVALID_HANDLE_VALUE ) {

			do {
				CommandLame( argv[0], ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // 次の.mp2へ

		}
		FindClose(hFileList); // ハンドル終了

	}
}
