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

	// ����������ꍇ�́Alame��
	if ( argc >= 2 ) {
		// kvs �ւ̕ϊ��ł���B
		if (argc >= 4 && strcmp(argv[1], "kvs") == 0) {
			OggToKvs(argv[2], argv[3]);

		// ����Lame�̃R�}���h�Ƃ݂Ȃ��B
		}
		else {

			lame_wmain(argc, argv);
		}
	} else {

		WIN32_FIND_DATA ffd;

		// �M���̖�]�E��u��BGM�n
		HANDLE hFileList = FindFirstFile("*.ktsl2stbin", &ffd);
		// ���݂����ꍇ�A�ϊ��ΏۂƂ���B
		if (hFileList != INVALID_HANDLE_VALUE) {
			do {
				Ktsl2asbinToKvs(ffd.cFileName);
			} while (FindNextFile(hFileList, &ffd)); // ����.vps��
		}
		FindClose(hFileList); // �n���h���I��

		// �M���̖�]�E��u��VOICE�n
		hFileList = FindFirstFile("*.ktsl2asbin", &ffd);
		// ���݂����ꍇ�A�ϊ��ΏۂƂ���B
		if (hFileList != INVALID_HANDLE_VALUE) {
			do {
				Ktsl2asbinToKvs(ffd.cFileName);
			} while (FindNextFile(hFileList, &ffd)); // ����.vps��
		}
		FindClose(hFileList); // �n���h���I��

		// �M���̖�]�E�n���̃{�C�X�n
		hFileList = FindFirstFile("*.vfp", &ffd);
		// ���݂����ꍇ�A�ϊ��ΏۂƂ���B
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				VfpToKvs( ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // ����.vps��
		}
		FindClose(hFileList); // �n���h���I��

		// �M���̖�]�E�V���̃{�C�X�n
		hFileList = FindFirstFile("*.g1l", &ffd);
		// ���݂����ꍇ�A�ϊ��ΏۂƂ���B
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				G1lToKvs( ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // ����.g1l��
		}
		FindClose(hFileList); // �n���h���I��

		// �M���̖�]�E�n���ŁA�P�ɂQ��KOVS�t�@�C���������Ă������
		hFileList = FindFirstFile("*.atslw", &ffd);
		// ���݂����ꍇ�A�ϊ��ΏۂƂ���B
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				AtslwToKvs( ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // ����.g1l��
		}
		FindClose(hFileList); // �n���h���I��

		// *.kvs�Ƃ������O�̃t�@�C���̃��X�g�ւ̃n���h��
		hFileList = FindFirstFile("*.kvs", &ffd);

		// ���݂����ꍇ�A�ϊ��ΏۂƂ���B
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				KvsToOgg( ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // ����.kvs��
		}
		FindClose(hFileList); // �n���h���I��


		// *.ogg�Ƃ������O�̃t�@�C���̃��X�g�ւ̃n���h��
		hFileList = FindFirstFile("*.ogg", &ffd);
		// ���݂����ꍇ�A�ϊ��ΏۂƂ���B
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				OggToWav( ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // ����.ogg��
		}
		FindClose(hFileList); // �n���h���I��

		// �M���̖�]�E�V�ċL DVD��
		hFileList = FindFirstFile("??.dat", &ffd);
		// ���݂����ꍇ�A�ϊ��ΏۂƂ���B
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				// ??�����͂Q�P�^�̐����ł���͂��B
				char szNo[3] = "";
				strncpy( szNo, ffd.cFileName, 2);
				// �P�ȏ�̐����̏ꍇ�̂݁B
				if ( atoi(szNo) ) {
					char szMp2FileName[256];
					GetMp2FileName( szMp2FileName, ffd.cFileName );
					CopyFile( ffd.cFileName, szMp2FileName, false  );
				}				
			} while (FindNextFile(hFileList, &ffd)); // ����.bgm��
		}
		FindClose(hFileList); // �n���h���I��

		// �M���̖�]�E���V�^
		hFileList = FindFirstFile("bgm??.nbx", &ffd);
		// ���݂����ꍇ�A�ϊ��ΏۂƂ���B
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				char szWmaFileName[256];
				GetWmaFileName( szWmaFileName, ffd.cFileName );
				CopyFile( ffd.cFileName, szWmaFileName, false  );
			} while (FindNextFile(hFileList, &ffd)); // ����.nbx��
		}
		FindClose(hFileList); // �n���h���I��
		

		// �M���̖�]�E�V���n��
		hFileList = FindFirstFile("bgm??.n11", &ffd);
		// ���݂����ꍇ�A�ϊ��ΏۂƂ���B
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				char szWmaFileName[256];
				GetWmaFileName( szWmaFileName, ffd.cFileName );
				CopyFile( ffd.cFileName, szWmaFileName, false  );
			} while (FindNextFile(hFileList, &ffd)); // ����.n11��
		}
		FindClose(hFileList); // �n���h���I��
		
		// �M���̖�]�E�����L
		hFileList = FindFirstFile("m??.bgm", &ffd);
		// ���݂����ꍇ�A�ϊ��ΏۂƂ���B
		if ( hFileList != INVALID_HANDLE_VALUE ) {
			do {
				// m?? ��??�����͂Q�P�^�̐����ł���͂��B
				char szNo[3] = "";
				strncpy( szNo, ffd.cFileName+1, 2);
				// �P�ȏ�̐����̏ꍇ�̂݁B
				if ( atoi(szNo) ) {
					BgmToWav(ffd.cFileName);
				}				
			} while (FindNextFile(hFileList, &ffd)); // ����.bgm��
		}
		FindClose(hFileList); // �n���h���I��

	}


	{
		// wma��S�� wav�֕ϊ�
		WIN32_FIND_DATA ffd;
		// *.wav�Ƃ������O�̃t�@�C���̃��X�g�ւ̃n���h��
		HANDLE hFileList = FindFirstFile("*.wma", &ffd);
		if ( hFileList != INVALID_HANDLE_VALUE ) {

			do {
				CommandWma2Wav( argv[0], ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // ����.wma��

		}
		FindClose(hFileList); // �n���h���I��

		/*
		// wav��S�� mp3�֕ϊ�
		// *.wav�Ƃ������O�̃t�@�C���̃��X�g�ւ̃n���h��
		hFileList = FindFirstFile("*.wav", &ffd);
		if ( hFileList != INVALID_HANDLE_VALUE ) {

			do {
				CommandLame( argv[0], ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // ����.mp3��

		}
		FindClose(hFileList); // �n���h���I��
		*/

		// �ȉ��Q�͗v���ӁB�����āAmp2���璼��wav, mp3 �ւƃR���o�[�g���Ă���B
		// mp2��S�� wav�֕ϊ�
		// *.mp2�Ƃ������O�̃t�@�C���̃��X�g�ւ̃n���h��
		hFileList = FindFirstFile("*.mp2", &ffd);
		if ( hFileList != INVALID_HANDLE_VALUE ) {

			do {
				CommandDecodeLame( argv[0], ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // ����.mp2��

		}
		FindClose(hFileList); // �n���h���I��

		// mp2��S�� mp3�֕ϊ�
		// *.mp2�Ƃ������O�̃t�@�C���̃��X�g�ւ̃n���h��
		hFileList = FindFirstFile("*.mp2", &ffd);
		if ( hFileList != INVALID_HANDLE_VALUE ) {

			do {
				CommandLame( argv[0], ffd.cFileName );
			} while (FindNextFile(hFileList, &ffd)); // ����.mp2��

		}
		FindClose(hFileList); // �n���h���I��

	}
}
