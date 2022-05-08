
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



// �M���V���[�Y�̂��̂���Amp2�̃t�@�C�����𓾂�
static void GetKvsFileName(char *pDstFile, char *pSrcFile ) {
	strcpy(pDstFile, pSrcFile);

	// .kvs�ƕύX����B
	strcpy(pDstFile+strlen(pDstFile)-4, ".kvs");
}

// G1lToKvs�� 
// �擪��28�o�C�g�A�܂�KOVS�Ƃ���������̑O���΂�����폜���邾����KVS�t�@�C���ƂȂ�B
int G1lToKvs (char *szG1lFileName) {
	FILE *input;
	FILE *output;
	char buf[4096];
	int i;


	//Open G1l File
	input = fopen(szG1lFileName, "rb");

	char szKvsFileName[256] = "";
	GetKvsFileName( szKvsFileName, szG1lFileName );

	//Open Kvs File
	output = fopen(szKvsFileName , "wb");

	// �ǂ߂Ȃ�������o�Ă�
	if(!input || !output) {
		if ( input  ) { fclose(input); }
		if ( output ) { fclose(output); }
		return -1;
	}

	// 28�o�C�g�ڂɈړ�
	fseek(input,28,SEEK_SET);

	// ����ȍ~�͒P���ɃR�s�[
	while((i = fread(buf, sizeof(char), sizeof(buf), input )) > 0)
	{
		fwrite(buf, sizeof(char), i, output );
	}

	// �����Ƃ���
	fclose(input);
	fclose(output);

	return 0;

}
