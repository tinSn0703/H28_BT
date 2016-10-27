
/******************************************************************************
 H28 10 / 23 (Sun)
�E������
	�EC_BT_304�Ƃ��̔h������폜�B�S�Ċ֐��ɕς���B
	�E__CTS__�Ȃǂ�#ifndef��p���邱�ƂŁA�v���W�F�N�g�ɂ���Ē����\�ɂ���B
	�EC_BT�̓����̑啝�Ȍ������B
		�E__RTS__��Timer�ł̃`�F�b�N���s��Ȃ������o�̒ǉ��B-> �߂�ǂ��������ς肢���B
		�E��M�t���O��Timer�ł̃`�F�b�N���s��Ȃ������o�̒ǉ��B-> ����
		�E���荞�ݏ����ɑ΂���H�v�B
		�E�ǂ����A�߂�l�g���ĂȂ����A�߂�l�ŃG���[�f������B
		�E�����o�ϐ���__CTS__�Ȃǂ��폜���A�O����#define�ɒu��������B
	�EC_BT�̐�p�̊֐��̍쐬�BROBOBA��BlueMaster������Ŏg��������B
	�E�s����#define�ɂ��邱�ƂŁAC_BT�O���ł����������֐��ł������悤�ɗp���邱�Ƃ��ł���悤�ɂ���B
	
 �E�����_
	�EC_BT��CTS�p�Ȃǂ̃����o�ϐ����폜�B�O����#define��p����\���ɁB
	�E__CTS__�ȂǁA��H�ɂ���ĕύX���镔����#ifndef��p���ăv���W�F�N�g���ƂɕύX�\�ɁB
	�E__XXX_ADDR__�n��ǉ����āA�ύX�\�ӏ������炵�A���S�ƃ~�X�̌y���B
	�E__XXX__�n�ŃV�t�g���Ă������A�g�����肪�����̂ŃV�t�g����__XXX_BIT__�n��p�ӁB
	�E__XXX_READ__�n��BOOL�^�ɕϊ��B
	�E�V����__XXX_SET__�n��ǉ��BDDR�ւ̓��̓~�X�𖳂����B
	�EC_BT::Out()��C_BT::In()��BOOL�^�̖߂�l��ǉ��B����ŁA�G���[�̃`�F�b�N���\�B
	
 H28 10 / 24 (Mon)
	�F�X����Ă݂����ǁA�����s���̃o�O���������܂����B�Ȃ񂩁A�R���g���[������̃f�[�^�̃f�[�^���󂯎�
�@��܂���ł����B�m�F���Ă݂��UART���̂��@�\���ĂȂ������BTIMER�������Ȃ��B�P�̂ł͕��ʂɓ������B�ł��A
�@BT�̂ƍ��킹��Ƌ@�\���Ȃ��Ȃ�B������ƈӖ���������Ȃ��Bfuck
�@�@C_BT���֌W���邩�͕s���BC_BT�p�֐����͖̂��Ȃ�����(����������)�B���̃t�@�C���Q�ɒu���v���O�����́A
�@BT�Ƃ̒ʐM�p�̂��̂����ɂ��Ă����܂��B���܂�悭�΂�Ȃ��ق��������̂��˂��Bfuck	
	
******************************************************************************/

#pragma once

#include <H28_AVR/H28_AVR.h>
#include <string.h>

/************************************************************************/

#define __BT_DATA_NUM__		4
#define __BT_ADDR_BYTE__	12

#define BT_DATA_NUM		4
#define BT_ADDR_BYTE	12

#ifndef __CTS_ADDR__
#	define __CTS_ADDR__ EI_PORTD
#endif
#ifndef __RTS_ADDR__
#	define __RTS_ADDR__ EI_PORTD
#endif
#ifndef __RSE_ADDR__
#	define __RSE_ADDR__ EI_PORTD
#endif

#ifndef __CTS__
#	define __CTS__ EI_IO4
#endif
#ifndef __RTS__
#	define __RTS__ EI_IO5
#endif
#ifndef __RSE__
#	define __RSE__ EI_IO6
#endif

#define __CTS_BIT__ (1 << __CTS__)
#define __RTS_BIT__ (1 << __RTS__)
#define __RSE_BIT__ (1 << __RSE__)

#define __CTS_PIN__ _SFR_IO8(__CTS_ADDR__ + 0)
#define __RTS_PIN__ _SFR_IO8(__RTS_ADDR__ + 0)
#define __RSE_PIN__ _SFR_IO8(__RSE_ADDR__ + 0)

#define __CTS_DDR__ _SFR_IO8(__CTS_ADDR__ + 1)
#define __RTS_DDR__ _SFR_IO8(__RTS_ADDR__ + 1)
#define __RSE_DDR__ _SFR_IO8(__RSE_ADDR__ + 1)

#define __CTS_PORT__ _SFR_IO8(__CTS_ADDR__ + 2)
#define __RTS_PORT__ _SFR_IO8(__RTS_ADDR__ + 2)
#define __RSE_PORT__ _SFR_IO8(__RSE_ADDR__ + 2)

#define __CTS_SET__ (__CTS_DDR__ |=  __CTS_BIT__)
#define __RTS_SET__ (__RTS_DDR__ &= ~__RTS_BIT__)
#define __RSE_SET__ (__RSE_DDR__ |=  __RSE_BIT__)

#define __CTS_READ__ ((BOOL)((__CTS_PIN__ >> __CTS__) & 1))
#define __RTS_READ__ ((BOOL)((__RTS_PIN__ >> __RTS__) & 1))
#define __RSE_READ__ ((BOOL)((__RSE_PIN__ >> __RSE__) & 1))

#define __CTS_HIGH__ (__CTS_PORT__ |=  __CTS_BIT__)
#define __RSE_HIGH__ (__RSE_PORT__ |=  __RSE_BIT__)

#define __CTS_LOW___ (__CTS_PORT__ &= ~__CTS_BIT__)
#define __RSE_LOW___ (__RSE_PORT__ &= ~__RSE_BIT__)

enum E_BT_NUM
{
	EB_BT0 = 0x00,
	EB_BT1 = 0x01,
	EB_BT2 = 0x02,
	EB_BT3 = 0x03,
	EB_BT4 = 0x04,
	EB_BT5 = 0x05,
	EB_BT6 = 0x06,
	EB_BT7 = 0x07,
	EB_BT8 = 0x08,
	EB_BT9 = 0x09,
	EB_BTA = 0x0a,
	EB_BTB = 0x0b,
	EB_BTC = 0x0c,
	EB_BTD = 0x0d,
	EB_BTE = 0x0e,
	EB_BTF = 0x0f,
};

/************************************************************************/

/**
 * \brief 
 * _arg_str_f��_arg_str���r���� 
 * _arg_str��_arg_str_f�̂��ׂĂ��܂܂�Ă���Έ�v�ƂȂ�
 *
 * \param _arg_str_base : ��r���镶����
 * \param _arg_str_comp : ��r����镶����
 * 
 * \return BOOL : ��r�̌���
 *	�s��v	-> FALSE
 *	��v	-> TRUE
 */
BOOL F_Str_comp (const char _arg_str_base[], char _arg_str_comp[]);

/**
 * \brief 
 *	_arg_str_base��_arg_str�ɏ��������܂��B
 *	�ǂ��炩���A�Ō�ɂȂ�܂ŏ��������܂��B����ȍ~�͂��̂܂܁B
 *
 * \param _arg_str_base	: �����������镶����
 * \param _arg_str_copy	: ���������镶����
 * 
 * \return char * _arg_str_base��return����܂�
 */
char * F_Str_copy (char _arg_str_base[], const char _arg_str_copy[]);

/**
 * \brief 
 *	_arg_str_base�̌���_arg_str_add��ǉ����܂�
 * 
 * \param _arg_str_base : �ǉ�����镶����
 * \param _arg_str_add  : �ǉ����镶����
 */
void F_Str_add(char _arg_str_base[], const char _arg_str_add);

/**
 * \brief 
 *	Bluetooth��_arg_data_out�𑗐M����B
 *	���M��������܂ő����܂��B�v���O��������~������^���Ă݂ĂˁB
 *
 * \param _arg_uart_addr_t	: ���M�ɗp����UART
 * \param _arg_data_out		: ���M����data
 */
void F_BT_Out (E_UART_ADDR _arg_uart_addr_t, const char _arg_data_out[]);

/**
 * \brief 
 *	Bluetooth�����M����B
 *	��M����data��_arg_re_data_in�Ɋi�[�����B
 *	��M��������܂ő����܂��B�v���O��������~������^���Ă݂ĂˁB 
 *
 * \param _arg_uart_addr_r	: ��M�ɗp����UART
 * \param _arg_re_data_in	: ��M����data���i�[���鏊
 */
void F_BT_In (E_UART_ADDR _arg_uart_addr_r, char _arg_re_data_in[]);

/**
 * \brief 
 *	��M�����f�[�^��_arg_data_comp�Ɉ�v����܂Ŏ�M��������B
 *	�d�l��A����Ȃ������疳�����[�v��ԂɂȂ�̂ŋC��t���ĂˁB 
 *
 * \param _arg_uart_addr_r	: �p����UART
 * \param _arg_data_comp	: ��r�Ώ�
 */
void F_BT_In_comp (E_UART_ADDR _arg_uart_addr_r, const char _arg_data_comp[]);

/**
 * \brief 
 *	��M�����f�[�^��_arg_data_comp�Ɉ�v����܂Ŏ�M��������B
 *	��M�����f�[�^��_arg_re_data_in�Ɋi�[�����B
 *	�d�l��A����Ȃ������疳�����[�v��ԂɂȂ�̂ŋC��t���ĂˁB 
 *
 * \param _arg_uart_addr_r	: �p����UART
 * \param _arg_re_data_in	: ��M�����f�[�^���i�[����ꏊ
 * \param _arg_data_comp	: ��r�Ώ�
 */
void F_BT_In_comp (E_UART_ADDR _arg_uart_addr_r, char _arg_re_data_in[], const char _arg_data_comp[]);

/**
 * \brief 
 *	Bluetooth���R�}���h���[�h�ɂ���B
 */
void F_BT_Cut();

/************************************************************************/

#include "H28_C_BT.cpp"

#include "H28_BT.cpp"


//#include "H28_C_BT_304.cpp"
//#include "H28_C_BT_304_MASTER.cpp"
//#include "H28_C_BT_304_SLAVE.cpp"