
#include <stdlib.h>
#include <string.h>


// �M���V���[�Y�̂��̂���Awma�̃t�@�C�����𓾂�
void GetWmaFileName(char *pDstFile, char *pSrcFile ) {
	strcpy(pDstFile, pSrcFile);

	// .wma�ƕύX����B
	strcpy(pDstFile+strlen(pDstFile)-4, ".wma");
}
