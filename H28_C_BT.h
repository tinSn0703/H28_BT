
#pragma once

#include "H28_BT.h"

/**
 * Bluetooth�Ƒ���M���s�����߂̃N���X
 */
class C_BT : protected C_UART_base
{
private:
	
	E_IO_PORT_ADDR _mem_bt_io_addr_cts :8;
	E_IO_PORT_ADDR _mem_bt_io_addr_rts :8;
	E_IO_PORT_ADDR _mem_bt_io_addr_rse :8;
	
	E_IO_NUM _mem_bt_bit_cts :3;
	E_IO_NUM _mem_bt_bit_rts :3;
	E_IO_NUM _mem_bt_bit_rse :3;
	
	BOOL _mem_bt_flag :1;
	
	C_TIMER_inside _mem_uart_timer;
	
protected:
	
	E_IO_PORT_ADDR ADDR_CTS()	{	return _mem_bt_io_addr_cts;	}
	E_IO_PORT_ADDR ADDR_RTS()	{	return _mem_bt_io_addr_rts;	}
	E_IO_PORT_ADDR ADDR_RSE()	{	return _mem_bt_io_addr_rse;	}
	
	E_IO_NUM CTS()	{	return _mem_bt_bit_cts;	}
	E_IO_NUM RTS()	{	return _mem_bt_bit_rts;	}
	E_IO_NUM RSE()	{	return _mem_bt_bit_rse;	}
	
#	define __PIN_RTS__  _SFR_IO8(ADDR_RTS() + 0)
#	define __PIN_CTS__  _SFR_IO8(ADDR_CTS() + 0)
#	define __PIN_RSE__  _SFR_IO8(ADDR_RSE() + 0)
	
#	define __DDR_RTS__	_SFR_IO8(ADDR_RTS() + 1)
#	define __DDR_CTS__  _SFR_IO8(ADDR_CTS() + 1)
#	define __DDR_RSE__  _SFR_IO8(ADDR_RSE() + 1)
	
#	define __PORT_RTS__ _SFR_IO8(ADDR_RTS() + 2)
#	define __PORT_CTS__ _SFR_IO8(ADDR_CTS() + 2)
#	define __PORT_RSE__ _SFR_IO8(ADDR_RSE() + 2)
	
#	define __RTS_CHECK__ (CHECK_TURN_BIT_TF(__PIN_RTS__,RTS()))
	
#	define __CTS_HIGH__	(__PORT_CTS__ |=  (1 << CTS()))
#	define __CTS_LOW__	(__PORT_CTS__ &= ~(1 << CTS()))
	
#	define __RSE_HIGH__	(__PORT_RSE__ |=  (1 << RSE()))
#	define __RSE_LOW__	(__PORT_RSE__ &= ~(1 << RSE()))
	
public:
	
	/**
	 * \brief 
	 * ��̃R���X�g���N�^�B�����^�C�}�̐ݒ�݂̂��s���B(10ms)
	 */
	C_BT();
	
	/**
	 * \brief 
	 * �R���X�g���N�^�B�g�p����UART�ƃs���̐ݒ���s��
	 * 115.2 [kbps]
	 * �{������
	 * �p���e�B�֎~
	 * 
	 * \param _arg_uart_addr	: Bluetooth�Ɛڑ�����UART�̃��W�X�^
	 * \param _arg_io_addr_rts  : RTS�s���̃��W�X�^
	 * \param _arg_bit_rts		: RTS�s���̃r�b�g
	 * \param _arg_io_addr_cts  : CTS�s���̃��W�X�^
	 * \param _arg_bit_cts		: CTS�s���̃r�b�g
	 * \param _arg_io_addr_rse	: RESET�s���̃��W�X�^
	 * \param _arg_bit_rse		: RESET�s���̃r�b�g
	 */
	C_BT(E_UART_ADDR _arg_uart_addr,E_IO_PORT_ADDR _arg_io_addr_rts,E_IO_NUM _arg_bit_rts,E_IO_PORT_ADDR _arg_io_addr_cts,E_IO_NUM _arg_bit_cts,E_IO_PORT_ADDR _arg_io_addr_rse,E_IO_NUM _arg_bit_rse);
	
	/**
	 * \brief CTS��HIGH�ɂ��āABluetooth����̑��M���֎~����B
	 */
	void Rce_off()	{	__CTS_HIGH__;	}
	
	/**
	 * \brief CTS��LOW�ɂ��āABluetooth����̑��M��������B
	 */
	void Rce_on()	{	__CTS_LOW__;	}
	
	/**
	 * \brief 
	 * Bluetooth�փf�[�^�𑗐M����
	 * ���M�Ɉڂ�Ȃ������ꍇ�̓t���O��FALES�ɂ��ă^�C���A�E�g����B
	 * 
	 * \param _arg_out_data : ���M����f�[�^�B
	 */
	void Out(const char _arg_out_data[]);
	
	/**
	 * \brief 
	 * Bluetooth����̃f�[�^���󂯎��
	 * ��M�Ɉڂ�Ȃ�������A�^�C���A�E�g����B
	 * 
	 * \param _arg_re_in_data : �����Ɏ�M�f�[�^���i�[�����
	 */
	void In(char _arg_in_data[]);
	
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
	friend void operator >> (C_BT &_arg_bt, const char _arg_str_comp[]);
	
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
