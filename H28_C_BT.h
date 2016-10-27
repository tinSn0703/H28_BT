
#pragma once

#include "H28_BT.h"

/**
 * Bluetooth�Ƒ���M���s�����߂̃N���X
 */
class C_BT : protected C_UART_base
{
private:
	
	BOOL _mem_bt_flag :1;
	
//	C_TIMER_inside _mem_uart_timer;
	
public:
	
	/**
	 * \brief 
	 * ��̃R���X�g���N�^�B�����^�C�}�̐ݒ�݂̂��s���B(10ms)
	 */
	C_BT();
	
	/**
	 * \brief 
	 * �R���X�g���N�^�B�g�p����UART�̐ݒ���s��
	 * 115.2 [kbps]
	 * �{������
	 * �p���e�B�֎~
	 * 
	 * \param _arg_uart_addr	: Bluetooth�Ɛڑ�����UART�̃��W�X�^
	 */
	C_BT (E_UART_ADDR _arg_uart_addr);
	
	/**
	 * \brief CTS��HIGH�ɂ��āABluetooth����̑��M���֎~����B
	 */
	void Rce_off()	{	__CTS_HIGH__;	}
	
	/**
	 * \brief CTS��LOW�ɂ��āABluetooth����̑��M��������B
	 */
	void Rce_on()	{	__CTS_LOW___;	}
	
	/**
	 * \brief ��M�������荞�݂̐ݒ���s��
	 * 
	 * \param _arg_nf_isr : ONOFF
	 */
	void Set_isr(BOOL _arg_nf_isr);
	
	/**
	 * \brief ��M�������荞�݂�ON�ɂ���
	 */
	void Set_isr_on();
	
	/**
	 * \brief ��M�������荞�݂�OFF�ɂ���
	 */
	void Set_isr_off();
	
	void Out(const char _arg_out_data[]);
	
	void In(char _arg_re_in_data[]);
	
	/**
	 * \brief 
	 * Bluetooth�փf�[�^�𑗐M����
	 * ���M�Ɉڂ�Ȃ������ꍇ�̓t���O��FALSE�ɂ��ă^�C���A�E�g����B
	 * 
	 * \param _arg_out_data : ���M����f�[�^�B
	 */
//	BOOL Out_check(const char _arg_out_data[]);
	
	/**
	 * \brief 
	 * Bluetooth����̃f�[�^���󂯎��
	 * ��M�Ɉڂ�Ȃ�������A�^�C���A�E�g����B
	 * 
	 * \param _arg_re_in_data : �����Ɏ�M�f�[�^���i�[�����
	 */
//	BOOL In_check(char _arg_in_data[]);
	
	/**
	 * \brief 
	 * _arg_bt_str_comp�ƈ�v����܂Ŏ�M��������
	 * �����Ƃ��Ȃ��Ɩ������[�v�Ɋׂ�̂Œ���
	 * 
	 * \param _arg_str_comp : ��r���镶����
	 */
	void In_comp(const char _arg_str_comp[]);
	
	/**
	 * \brief 
	 * _arg_bt_str_comp�ƈ�v����܂Ŏ�M��������
	 * ��v�����f�[�^��Ԃ�
	 * 
	 * \param _arg_re_in_data : �����Ɏ�M�f�[�^���i�[�����
	 * \param _arg_str_comp : ��r���镶����
	 */
	void In_comp(char _arg_re_in_data[], const char _arg_str_comp[]);
	
	/**
	 * \brief Bluetooth���ċN������
	 */
	void Reset();
	
	/**
	 * \brief 
	 * Bluetooth�Ƀf�[�^�𑗐M���鉉�Z�q
	 * C_BT::Out()�Ɠ����@�\
	 * 
	 * \param _arg_bt
	 * \param _arg_out_data : ���M����f�[�^
	 */
	friend void operator << (C_BT &_arg_bt, const char _arg_out_data[]);
	
	/**
	 * \brief
	 * Bluetooth����̃f�[�^����M���鉉�Z�q�B
	 * �E�ӂ��萔�̏ꍇ�A�������M����܂Ŏ�M��������
	 * C_BT::In()��C_BT::In_comp()�Ɠ����@�\
	 * 
	 * \param _arg_bt
	 * \param _arg_re_in_data[] : �f�[�^���i�[�����ꏊ
	 */
	friend void operator >> (C_BT &_arg_bt, char _arg_re_in_data[]);
	
	/**
	 * \brief
	 * Bluetooth����̃f�[�^����M���鉉�Z�q�B
	 * �E�ӂ��萔�̏ꍇ�A�������M����܂Ŏ�M��������
	 * C_BT::In()��C_BT::In_comp()�Ɠ����@�\
	 * 
	 * \param _arg_bt
	 * \param _arg_str_comp : ��r���镶����
	 */
	friend void operator >= (C_BT &_arg_bt, const char _arg_str_comp[]);
	
	/**
	 * \brief 
	 * Bluetooth�Ɛڑ����Ă���̂��m�F�p�̉��Z�q�B
	 * TRUE��\���ꍇ�ڑ��AFALES��\���ꍇ�ؒf
	 * 
	 * \param _arg_bt
	 * \param _arg_flag_comp : ��r
	 * 
	 * \return bool _arg_bt��_arg_bt_flag_comp���������Ƃ�true
	 */
	friend bool operator == (C_BT &_arg_bt, BOOL _arg_flag_comp);
	
	/**
	 * \brief 
	 * Bluetooth�Ɛڑ����Ă���̂��m�F�p�̉��Z�q�B
	 * TRUE��\���ꍇ�ڑ��AFALES��\���ꍇ�ؒf
	 * 
	 * \param _arg_bt
	 * \param _arg_flag_comp : ��r
	 * 
	 * \return bool _arg_bt��_arg_flag_comp���������Ȃ��Ƃ�true
	 */
	friend bool operator != (C_BT &_arg_bt, BOOL _arg_flag_comp);
};

#include "H28_C_BT.cpp"
