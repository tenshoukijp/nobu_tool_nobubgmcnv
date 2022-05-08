
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>


// �����L�^�C�v�̂���

// �M���V���[�Y�̂��̂���Awav�̃t�@�C�����𓾂�
static void GetWavFileName(char *pDstFile, char *pSrcFile ) {
	strcpy(pDstFile, pSrcFile);

	// .wma�ƕύX����B
	strcpy(pDstFile+strlen(pDstFile)-4, ".wav");
}


BYTE WavHeader[]={0x52,0x49,0x46,0x46,0xC8,0xB6,0xC6,0x00,0x57,0x41,0x56,0x45,0x66,0x6D,0x74,0x20,0x10,0x00,0x00,0x00,0x01,0x00,0x02,0x00,0x44,0xAC,0x00,0x00,0x10,0xB1,0x02,0x00,0x04,0x00,0x10,0x00,0x64,0x61,0x74,0x61};
void BgmToWav(char *szBgmFileName) {
	// ���̓t�@�C��������o�̓t�@�C�������߂�
	char szWavFileName[256] = "";
	GetWavFileName( szWavFileName, szBgmFileName );

	// ���̓t�@�C���Əo�̓t�@�C���B�����o�C�i��
	FILE *fpBGM;
	fpBGM = fopen(szBgmFileName, "rb");
	FILE *fpWAV;
	fpWAV = fopen(szWavFileName, "wb");

	// �Ȃ񂩎��s���Ă�����ă��^�[��
	if ( !fpBGM || !fpWAV ) {
		if ( fpBGM ) { fclose(fpBGM); }
		if ( fpWAV ) { fclose(fpWAV); }
		return;
	}

	// �w�b�_�[�̃T�C�Y
	int iWaveHeaderSize = sizeof(WavHeader)/sizeof(WavHeader[0]);

	// �o�̓t�@�C���Ƀw�b�_�[���������o��
	fwrite(WavHeader, sizeof(BYTE), iWaveHeaderSize, fpWAV);

	// ���̓t�@�C���̂a�f�l�́A�w�b�_�[�̃T�C�Y����΂�
	fseek(fpBGM, iWaveHeaderSize, SEEK_SET);

   /* binary�t�@�C���̃R�s�[ */
	char charbuf[4096];
	int readItems;
	while((readItems = fread(&charbuf, 1, sizeof(charbuf)/sizeof(charbuf[0]), fpBGM)) != 0) {
		fwrite(&charbuf, 1, readItems, fpWAV);
   }
	
	// �����N���[�X
    fclose(fpBGM);
    fclose(fpWAV);
}