/*
*�ļ�:Des.h
*����:Des����㷨

*ʱ��:2008-09-23
*/
// #include <stdlib.h>
#ifndef __DES_H__
#define __DES_H__
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//�ӽ���ģʽ
// enum CryptModel{DES,DES3,OTHER};
// //MACģʽ
// enum MACModel{ANSI_DATAXNOR_DES,ANSI_DATAXNOR_3DES,ANSI_XNOR_DES,ANSI_XNOR_3DES,
// 			PBOC_DATAXNOR_DES,PBOC_DATAXNOR_3DES,PBOC_XNOR_DES,PBOC_XNOR_3DES};
/*des�����㷨*/
/*������m:8�ֽ����� k���ֽ���Կ e���ֽ�����*/
extern void endes(unsigned char m[8],unsigned char k[8],unsigned char e[8]);

/*des�����㷨*/
/*������e:8�ֽ����� k���ֽ���Կ m���ֽ�����*/
extern void undes(unsigned char e[8],unsigned char k[8],unsigned char m[8]);

///*3des�����㷨*/
///*������m:8�ֽ����� k���ֽ���Կ e���ֽ�����*/
//extern void en3des(unsigned char m[8],unsigned char k[16],unsigned char e[8]);
//
///*3des�����㷨*/
///*������m:8�ֽ����� k���ֽ���Կ e���ֽ�����*/
//extern void un3des(unsigned char e[8],unsigned char k[16],unsigned char m[8]);

#endif
