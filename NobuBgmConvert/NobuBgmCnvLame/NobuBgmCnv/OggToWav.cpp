/*
vdec.c oggvorsis�f�R�[�h
libogg,libvorbis�̒��쌠��Xiphophorus�ɂ���܂��B
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>

//���C�u�����̃����N
#pragma comment(lib , "libogg_static_vc2013xp")
#pragma comment(lib , "libvorbis_static_vc2013xp")
#pragma comment(lib , "libvorbisfile_static_vc2013xp")

// kvs�t�@�C���̖��O����ɁA�o�͂���ogg�t�@�C���̖��O�����߂�B
static void GetWavFileName(char *pBufWavFileName, char *pBufOggFileName ) {
	strcpy(pBufWavFileName, pBufOggFileName);

	// .ogg�ƕύX����B
	strcpy(pBufWavFileName+strlen(pBufWavFileName)-4, ".wav");
}


/* WAVE�t�@�C���̃w�b�_ */
typedef struct{
	char ckidRIFF[4];
	int ckSizeRIFF;
	char fccType[4];
	char ckidFmt[4];
	int ckSizeFmt;
	WAVEFORMATEX WaveFmt;
	char ckidData[4];
	int ckSizeData;
} WAVEFILEHEADER;


//##############################################################################
// long LoadOggVorbis(char *file_name, int word, char **ogg)
//
// �T�v
//     �t�@�C������OggVorbis�`���̃f�[�^��ǂݍ��݁A���������WAVE�`���Ńf�R�[
//   �h����B
//     �f�[�^���s�v�ɂȂ����� ReleaseOggVorbis ��p���ė̈���J�����Ȃ��Ă͂Ȃ�
//   �Ȃ��B
//
// ����
//     *file_name : �ǂݍ��ރt�@�C����
//     word       : �ʎq���o�C�g��
//                  1 �� 2 ���w�肷��B
//     **ogg      : �f�[�^���i�[����char�^�̃|�C���^�ւ̃|�C���^
//
// �߂�l
//     0 ��菬���� : �G���[
//     0 ���傫�� : �T�C�Y
//##############################################################################
int OggToWav(char *szOggFileName ) {

	char *ogg = NULL;

	OggVorbis_File vf;
	vorbis_info *vi;
	int eof=0;
	int current_section;
	FILE *fp;
	WAVEFILEHEADER wh;
	long whsize;
	long size = 0;
	long data_size;
	int ret;
	int word = 2;

	char szWavFileName[256] = "";
	GetWavFileName( szWavFileName, szOggFileName );
	/* �t�@�C�����J�� */
	FILE *pwav = fopen(szWavFileName, "wb");

	/* �t�@�C�����J�� */
	fp = fopen(szOggFileName, "rb");
	if(fp == NULL){
		fprintf(stderr, "Open Error : %s", szOggFileName);
		return -1;
	}

	/* OggVorbis������ */
	if(ov_open(fp, &vf, NULL, 0) < 0) {
		fprintf(stderr,"Input does not appear to be an Ogg bitstream.\n");
		fclose(fp);
		return -1;
	}

	/* �ڍ׎��� */
	vi = ov_info(&vf,-1);
	if(vi == NULL){
		fprintf(stderr, "Get Info Error\n");
		return -1;
	}

	/* �w�b�_�T�C�Y�̎��� */
	/* sizeof(wh) ���ƑS�Ẵ����o�̃T�C�Y�̍��v���傫���Ȃ��Ă��܂� */
	whsize = sizeof(wh.ckidRIFF) + sizeof(wh.ckSizeRIFF) + sizeof(wh.fccType) +
		sizeof(wh.ckidFmt)  + sizeof(wh.ckSizeFmt)  + sizeof(wh.WaveFmt) +
		sizeof(wh.ckidData) + sizeof(wh.ckSizeData);

	/* �f�R�[�h��̃f�[�^�T�C�Y�����߁A�������m�� */
	data_size = (long)ceil(vi->channels * vi->rate * ov_time_total(&vf,-1) * word );

	ogg = (char *)malloc(data_size + whsize);
	if(ogg == NULL){
		free(ogg);
		ov_clear(&vf);
		fclose(fp);
		fprintf(stderr, "malloc Error\n");
		return -1;
	}


	/* �f�R�[�h */
	/* ���Ńo�b�t�@�S�ĂɃf�R�[�h�����Ƃ͌���Ȃ� */
	/* ���ɂ� 256�`4096Byte �f�R�[�h�����悤��  */

	while(!eof){
		ret = ov_read(&vf, ogg + size + whsize, data_size - size, 0, word, 1, &current_section);
		if (ret == 0) {
			eof=1;
		} else if (ret < 0) {
			free(ogg);
			ov_clear(&vf);
			fclose(fp);
			fprintf(stderr, "Decode Error\n");
			return -1;
		} else {
			size += ret;
		}
	}


	/* �w�b�_�̏����� */
	memcpy(wh.ckidRIFF, "RIFF", 4);
	wh.ckSizeRIFF = whsize + size - 8;
	memcpy(wh.fccType, "WAVE", 4);
	memcpy(wh.ckidFmt, "fmt ", 4);
	wh.ckSizeFmt = sizeof(WAVEFORMATEX);

	wh.WaveFmt.cbSize          = sizeof(WAVEFORMATEX);
	wh.WaveFmt.wFormatTag      = WAVE_FORMAT_PCM;
	wh.WaveFmt.nChannels       = (WORD)(vi->channels);
	wh.WaveFmt.nSamplesPerSec  = vi->rate;
	wh.WaveFmt.nAvgBytesPerSec = vi->rate * vi->channels * word;
	wh.WaveFmt.nBlockAlign     = (WORD)(vi->channels * word);
	wh.WaveFmt.wBitsPerSample  = (WORD)(word * 8);

	memcpy(wh.ckidData, "data", 4);
	wh.ckSizeData = size;


	/* �������ւ̃w�b�_�̏������� */
	int s = 0;
	memcpy(ogg, &wh.ckidRIFF, sizeof(wh.ckidRIFF));
	s += sizeof(wh.ckidRIFF);
	memcpy(ogg + s, &wh.ckSizeRIFF, sizeof(wh.ckSizeRIFF));
	s += sizeof(wh.ckSizeRIFF);
	memcpy(ogg + s, &wh.fccType, sizeof(wh.fccType));
	s += sizeof(wh.fccType);
	memcpy(ogg + s, &wh.ckidFmt, sizeof(wh.ckidFmt));
	s += sizeof(wh.ckidFmt);
	memcpy(ogg + s, &wh.ckSizeFmt, sizeof(wh.ckSizeFmt));
	s += sizeof(wh.ckSizeFmt);
	memcpy(ogg + s, &wh.WaveFmt, sizeof(wh.WaveFmt));
	s += sizeof(wh.WaveFmt);
	memcpy(ogg + s, &wh.ckidData, sizeof(wh.ckidData));
	s += sizeof(wh.ckidData);
	memcpy(ogg + s, &wh.ckSizeData, sizeof(wh.ckSizeData));

	/* cleanup */
	ov_clear(&vf);

	if ( pwav ) {
		fwrite( ogg, data_size + whsize, 1, pwav );
		fclose(pwav);
	}

	free(ogg);

	return 0;
}


