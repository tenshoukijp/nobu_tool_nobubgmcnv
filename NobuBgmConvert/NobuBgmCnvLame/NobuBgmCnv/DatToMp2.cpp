
#include <stdlib.h>
#include <string.h>


// �M���V���[�Y�̂��̂���Amp2�̃t�@�C�����𓾂�
void GetMp2FileName(char *pDstFile, char *pSrcFile ) {
	strcpy(pDstFile, pSrcFile);

	// .wma�ƕύX����B
	strcpy(pDstFile+strlen(pDstFile)-4, ".mp2");
}
