
#pragma once

#include "H28_C_BT_304.h"

/**
 * Bluetooth��Master�Ƃ��Ĉ����N���X
 */
class C_BT_304_MASTER : public C_BT_304
{
public:

	/**
	 * \brief 
	 * ��̃R���X�g���N�^�B
	 */
	C_BT_304_MASTER()	{}
		
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
	C_BT_304_MASTER(E_UART_ADDR _arg_uart_addr,E_IO_PORT_ADDR _arg_io_addr_rts,E_IO_NUM _arg_bit_rts,E_IO_PORT_ADDR _arg_io_addr_cts,E_IO_NUM _arg_bit_cts,E_IO_PORT_ADDR _arg_io_addr_rse,E_IO_NUM _arg_bit_rse);
	
	/**
	 * \brief Master�Ɛڑ�����B�A�h���X����͂���^�C�v�B
	 * 
	 * \param _arg_bt_addr : �A�h���X�B�`�F�b�N�n�͂Ȃ��̂�
	 */
	void Connect(const char _arg_bt_addr[]);
	
	/**
	 * \brief Master�Ɛڑ�����B�A�h���X�͂��炩���ߓ��͂��ꂽ���̂��g��
	 */
	void Connect();
	
	/**
	 * \brief �Đڑ��B
	 */
	void Re_Connect();
	
	/**
	 * \brief 
	 * Slave�֕ϊ����Ă��瑗��
	 * ��̓I�ɂ͐��l�𕶎���ɕϊ�����
	 * 
	 * \param _arg_out_data : ���M����f�[�^
	 */
	void Out(const char _arg_out_data[]);
	
	/**
	 * \brief 
	 * Slave�փf�[�^�𑗐M����B
	 * �@�\�I�ɂ�C_BT_304_MASTER::Out()�Ɠ���
	 * 
	 * \param _arg_bt_master
	 * \param _arg_out_data : ���M����f�[�^
	 */
	friend void operator << (C_BT_304_MASTER &_arg_bt_master, const char _arg_out_data[]);
	
	/**
	 * \brief 
	 * Slave�ƒʐM���Ă��邩���m�F����p�̉��Z�q
	 * TRUE��\���Ƃ��ڑ��BFALES�̏ꍇ�ؒf�B
	 * 
	 * \param _arg_bt_master
	 * \param _arg_flag_comp : ��r
	 * 
	 * \return bool 
	 * _arg_bt_master��_arg_flag_comp���������Ƃ�true
	 */
	friend bool operator == (C_BT_304_MASTER &_arg_bt_master, BOOL _arg_flag_comp);
	
	/**
	 * \brief 
	 * Slave�ƒʐM���Ă��邩���m�F����p�̉��Z�q
	 * TRUE��\���Ƃ��ڑ��BFALES�̏ꍇ�ؒf�B
	 * 
	 * \param _arg_bt_master
	 * \param _argr_flag_comp : ��r
	 * 
	 * \return bool 
	 * _arg_bt_master��_arg_flag_comp���������Ȃ��Ƃ�true
	 */
	friend bool operator != (C_BT_304_MASTER &_arg_bt_master, BOOL _arg_flag_comp);
};

#include "H28_C_BT_304_MASTER.cpp"