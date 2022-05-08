
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <vector>

using namespace std;

// Atslw�t�@�C��������AKvs�̃t�@�C�����𓾂�
static void GetSequenceKvsFileName(char *pDstFile, char *pSrcFile, int iNo ) {
	strcpy(pDstFile, pSrcFile);

	strcpy(pDstFile+strlen(pDstFile)-6, "");

	// .kvs�ƕύX����B
	wsprintf(pDstFile, "%s_%03d.kvs", pDstFile, iNo);
}

int AtslwToKvs(char *szAtslwFileName ) {

	FILE *fpATSLW;

	fpATSLW = fopen(szAtslwFileName, "rb");

	int iKovsCnt = 2;
	vector<int> vArray; // �P��VPS�t�@�C���̒��ɂ́A������KOVS�R���e���c���l�܂��Ă�B���ꂼ���KOVS�R���e���c�̐擪�̃A�h���X��vector�Ƃ��ďW�߂�B

	int i = 0;
	if ( fpATSLW ) {

		// ATSLW�ɂ́A�Ƃ肠�����AKovs�R���|�[�l���g��2�ŌŒ�I�Ȗ͗l
		int inbuf;

		fseek( fpATSLW, 0x2C, SEEK_SET);
		fread(&inbuf, sizeof(int), 1, fpATSLW ); // 4�o�C�g=int �łP���̃A�h���X
		vArray.push_back(inbuf);

		fseek( fpATSLW, 0x54, SEEK_SET);
		fread(&inbuf, sizeof(int), 1, fpATSLW ); // 4�o�C�g=int �łP���̃A�h���X
		vArray.push_back(inbuf);

		fseek( fpATSLW, 0, SEEK_END );
		vArray.push_back( (int)(ftell( fpATSLW )) ); // �t�@�C���̃T�C�Y���Ō�̃A�h���X�Ƃ���B

		// �i�A�h���X��KOVS�R���|�[�l���g���t�@�C���ɓf���o���Ă䂭
		for( int v=0; v<(int)vArray.size()-1; v++ ) {

			FILE *fpOutKvs;
			char szKvsFileName[256] = "";
			GetSequenceKvsFileName(szKvsFileName, szAtslwFileName, v+1);

			fpOutKvs = fopen(szKvsFileName, "wb");

			int iKovsSize = vArray[v+1]-vArray[v]; // Kovs�R���|�[�l���g�̃T�C�Y
			fseek( fpATSLW, vArray[v], SEEK_SET); // �Y����Kovs�R���|�[�l���g�̈ʒu�Ɉړ�

			// Kovs�R���|�[�l���g�̃T�C�Y���m��
			char *pBuf = (char *)malloc(iKovsSize);

			// �T�C�Y���ǂݍ���
			fread(pBuf, sizeof(char), iKovsSize, fpATSLW );

			// ���̂܂܃t�@�C���Ƃ��ēf���o��
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

	