
#pragma once

#include "H28_C_BT.h"

/**
 * Bluetooth�������N���X
 */
class C_BT_304
{
protected:
	
	char _mem_bt_addr[BT_ADDR_BYTE];
	
	C_BT _mem_bt;
	
public:
	
	/**
	 * \brief 
	 * ��̃R���X�g���N�^�B�������Ȃ�
	 */
	C_BT_304()	{}
		
	/**
	 * \brief �R���X�g���N�^�BBT�̎g�p�ɕK�v�Ȑݒ���s��
	 * 
	 * \param _arg_uart_addr	: Bluetooth�Ɛڑ�����UART�̃��W�X�^
	 * \param _arg_io_addr_rts  : RTS�s���̃��W�X�^
	 * \param _arg_bit_rts		: RTS�s���̃r�b�g
	 * \param _arg_io_addr_cts  : CTS�s���̃��W�X�^
	 * \param _arg_bit_cts		: CTS�s���̃r�b�g
	 * \param _arg_io_addr_rse	: RESET�s���̃��W�X�^
	 * \param _arg_bit_rse		: RESET�s���̃r�b�g
	 */
	C_BT_304(E_UART_ADDR _arg_uart_addr, E_IO_PORT_ADDR _arg_io_addr_rts, E_IO_NUM _arg_bit_rts, E_IO_PORT_ADDR _arg_io_addr_cts, E_IO_NUM _arg_bit_cts, E_IO_PORT_ADDR	_arg_io_addr_rse, E_IO_NUM _arg_bit_rse);
	
	/**
	 * \brief �ʐM���鑊���Bluetooth�̃A�h���X�̐ݒ�
	 * 
	 * \param _arg_bt_addr : �����ɃA�h���X����͂���
	 */
	void Set_bt_addr(const char _arg_bt_addr[]);
	
	/**
	 * \brief 
	 * �ڑ�����Ă���Bluetooth�̃A�h���X���擾����
	 * �����N���m��������͎g���܂���
	 * 
	 * \param _arg_re_bt_addr : �擾�����A�h���X�������Ɋi�[�����
	 */
	void Get_bt_addr(char _arg_re_bt_addr[]);
		
	/**
	 * \brief �ʐM���鑊��̃A�h���X���o�͂���
	 * 
	 * \param _arg_re_bt_addr : �ʐM���鑊��̃A�h���X�������Ɋi�[�����
	 */
	void Ret_bt_addr(char _arg_re_bt_addr[]);
};

#include "H28_C_BT_304.cpp"