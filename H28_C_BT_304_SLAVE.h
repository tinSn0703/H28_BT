
#pragma once

#include "H28_C_BT_304.h"

/**
 * Bluetooth��Slave�Ŏg���N���X�B
 */
class C_BT_304_SLAVE : public C_BT_304
{
protected:
	
	T_NUM _mem_bt_slave_flag_count;
	BOOL _mem_bt_slave_falg :1;
	
public:
	
	/**
	 * \brief 
	 * ��̃R���X�g���N�^�B�������Ȃ�
	 */
	C_BT_304_SLAVE()	{}
		
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
	C_BT_304_SLAVE(E_UART_ADDR _arg_uart_addr, E_IO_PORT_ADDR _arg_io_addr_rts, E_IO_NUM _arg_bit_rts, E_IO_PORT_ADDR _arg_io_addr_cts, E_IO_NUM _arg_bit_cts, E_IO_PORT_ADDR	_arg_io_addr_rse, E_IO_NUM _arg_bit_rse);
	
	/**
	 * \brief Master�Ɛڑ�����B�A�h���X�͂��炩���ߓ��͂��ꂽ���̂��g��
	 */
	void Connect();
	
	/**
	 * \brief Master�Ɛڑ�����B�A�h���X����͂���^�C�v�B
	 * 
	 * \param _arg_bt_addr : �A�h���X�B�`�F�b�N�n�͂Ȃ��̂�
	 */
	void Connect(const char _arg_bt_addr[]);
	
	/**
	 * \brief �Đڑ��B
	 */
	void Re_Connect();
	
	/**
	 * \brief 
	 * Master����̃f�[�^��ϊ����Ă���󂯎��B
	 * ��̓I�ɂ͕�����ɕϊ����ꂽ�f�[�^�𐔒l�ɖ߂��B
	 * 
	 * \param _arg_re_data_in 
	 *		��M�����f�[�^�̊i�[�����ꏊ�B
	 *		�vBT_DATA_NUM�ȏ�̗v�f��
	 */
	void In(char _arg_re_data_In[]);
	
	/**
	 * \brief 
	 * Master����̃f�[�^���󂯎�鉉�Z�q
	 * �@�\�I�ɂ�C_BT_304_SLAVE::In()�Ɠ���
	 * 
	 * \param _arg_bt_slave : �݂��܂�
	 * \param _arg_re_data_in : �f�[�^���i�[�����ϐ�
	 */
	friend void operator >> (C_BT_304_SLAVE &_arg_bt_slave, char _arg_re_data_in[]);
	
	/**
	 * \brief 
	 * Master�ƒʐM���Ă��邩���m�F����p�̉��Z�q
	 * TRUE��\���Ƃ��ڑ��BFALES�̏ꍇ�ؒf�B
	 * 
	 * \param _arg_bt_slave
	 * \param _arg_flag_comp : ��r
	 * 
	 * \return bool 
	 * _arg_bt_slave��_arg_flag_comp���������Ƃ�true
	 */
	friend bool operator == (C_BT_304_SLAVE &_arg_bt_slave, BOOL _arg_flag_comp);
	
	/**
	 * \brief 
	 * Master�ƒʐM���Ă��邩���m�F����p�̉��Z�q
	 * TRUE��\���Ƃ��ڑ��BFALES�̏ꍇ�ؒf�B
	 * 
	 * \param _arg_bt_slave
	 * \param _arg_flag_comp : ��r
	 * 
	 * \return bool 
	 * _arg_bt_slave��_arg_flag_comp���������Ȃ��Ƃ�true
	 */
	friend bool operator != (C_BT_304_SLAVE &_arg_bt_slave, BOOL _arg_flag_comp);
};

#include "H28_C_BT_304_SLAVE.cpp"
