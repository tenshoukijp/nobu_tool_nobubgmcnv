
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <windows.h>

#include "OggToWav.h"

// kvs�t�@�C���̖��O����ɁA�o�͂���ogg�t�@�C���̖��O�����߂�B
void GetOggFileName(char *pBufOggFileName, char *pBufKvsFileName ) {
	strcpy(pBufOggFileName, pBufKvsFileName);

	// �g���q��.kvs���܂܂�Ă�����c
	if ( strstr(pBufKvsFileName, ".kvs") ) {
		// .ogg�ƕύX����B
		strcpy(pBufOggFileName+strlen(pBufOggFileName)-4, ".ogg");

	}
	// �܂܂�Ă��Ȃ����
	else {
		// �g���q��t��������B
		strcat(pBufOggFileName, ".ogg");
	}
}

// Kvs2Ogg�̃T���v���\�[�X�قڂ܂��
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

	// �ǂ߂Ȃ�������o�Ă�
	if(!input || !output) {
		if ( input  ) { fclose(input); }
		if ( output ) { fclose(output); }
		return -1;
	}

	// �ŏ���16�o�C�g���������m��
	buf = (char *)malloc(16);

	// 16�o�C�g���A.kvs�t�@�C������R�s�[
	fread(buf, sizeof(char), 16, input);

	// �ŏ��̂S�o�C�g��KOVS�łȂ���ΏI��
	if(memcmp(buf,"KOVS",4) != 0) {
		// malloc �̕����
		free(buf);
		return 1;
	}

	//4�o�C�g�ڂ���A�t�@�C���T�C�Y������Bget file size
	ogg_size = *(unsigned long*)(buf+4);

	// ogg_size��256�o�C�g��菬����������悭�킩��Ȃ��̂ŁA�I��
	if(ogg_size < 0xFF) {
		// malloc �̕����
		free(buf);
		return 1;
	}

	//get loop point
	loop_point = *(unsigned long*)(buf+8);

	// 32�o�C�g�ڂɈړ�
	fseek(input,0x20,SEEK_SET);

	// �ŏ���256�o�C�g�ڂ܂ŕϊ������K�v
	for(i=0 ; i <= 0xFF ; i++)
	{
		// �o�C�g���K���ɏ]���ĕϊ�
		fread(buf, sizeof(char), 1, input);
		out = (char)(*buf^i);
		// ��������
		fwrite(&out, sizeof(char), 1, output);
	}

	// ����ȍ~�͒P���ɃR�s�[
	while((i = fread(buf, sizeof(char), 16, input )) > 0)
	{
		fwrite(buf, sizeof(char), i, output );
	}

	// malloc �̕����
	free(buf);

	// �����Ƃ���
	fclose(input);
	fclose(output);

	return 0;

}
