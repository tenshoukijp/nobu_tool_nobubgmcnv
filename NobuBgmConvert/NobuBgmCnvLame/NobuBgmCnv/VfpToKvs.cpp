
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <vector>

using namespace std;

// Vfp�t�@�C��������AKvs�̃t�@�C�����𓾂�
static void GetSequenceKvsFileName(char *pDstFile, char *pSrcFile, int iNo ) {
	strcpy(pDstFile, pSrcFile);

	strcpy(pDstFile+strlen(pDstFile)-4, "");

	// .kvs�ƕύX����B
	wsprintf(pDstFile, "%s_%03d.kvs", pDstFile, iNo);
}

int VfpToKvs(char *szVpsFileName ) {

	FILE *fpVFP;

	fpVFP = fopen(szVpsFileName, "rb");

	int iKovsCnt = 100;
	vector<int> vArray; // �P��VPS�t�@�C���̒��ɂ́A������KOVS�R���e���c���l�܂��Ă�B���ꂼ���KOVS�R���e���c�̐擪�̃A�h���X��vector�Ƃ��ďW�߂�B

	int i = 0;
	if ( fpVFP ) {

		// �܂���Kovs�R���|�[�l���g�̐����m��
		int inbuf;
		while ( fread(&inbuf, sizeof(int), 1, fpVFP ) ) { // 4�o�C�g=int �łP���̃A�h���X

			// index1��Kovs�R���e���c�̌�
			if ( i==1 ) {
				iKovsCnt = inbuf;
			}

			if ( i==2 ) {
				break;
			}

			i++;
		}

		i = 0;
		while ( fread(&inbuf, sizeof(int), 1, fpVFP ) ) { // 4�o�C�g=int �łP���̃A�h���X

			if ( i < iKovsCnt ) {
				vArray.push_back(inbuf);
			}

			else {
				break;
			}

			i++;
		}

		fseek( fpVFP, 0, SEEK_END );
		vArray.push_back( (int)(ftell( fpVFP )) ); // �t�@�C���̃T�C�Y���Ō�̃A�h���X�Ƃ���B

		// �i�A�h���X��KOVS�R���|�[�l���g���t�@�C���ɓf���o���Ă䂭
		for( int v=0; v<(int)vArray.size()-1; v++ ) {

			FILE *fpOutKvs;
			char szKvsFileName[256] = "";
			GetSequenceKvsFileName(szKvsFileName, szVpsFileName, v+1);

			fpOutKvs = fopen(szKvsFileName, "wb");

			int iKovsSize = vArray[v+1]-vArray[v]; // Kovs�R���|�[�l���g�̃T�C�Y
			fseek( fpVFP, vArray[v], SEEK_SET); // �Y����Kovs�R���|�[�l���g�̈ʒu�Ɉړ�

			// Kovs�R���|�[�l���g�̃T�C�Y���m��
			char *pBuf = (char *)malloc(iKovsSize);

			// �T�C�Y���ǂݍ���
			fread(pBuf, sizeof(char), iKovsSize, fpVFP );

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

	if ( fpVFP ) {
		fclose(fpVFP);
	}

	return 0;
}

	