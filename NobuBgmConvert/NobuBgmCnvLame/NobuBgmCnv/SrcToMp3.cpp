
#include <stdlib.h>
#include <string.h>


// �M���V���[�Y�̂��̂���Amp3�̃t�@�C�����𓾂�
void GetMp3FileName(char *pDstFile, char *pSrcFile ) {
	strcpy(pDstFile, pSrcFile);

	// .wma�ƕύX����B
	strcpy(pDstFile+strlen(pDstFile)-4, ".mp3");
}

