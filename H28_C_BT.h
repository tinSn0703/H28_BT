
#pragma once

#include "H28_BT.h"

/**
 * Bluetoothと送受信を行うためのクラス
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
	 * 空のコンストラクタ。内部タイマの設定のみを行う。(10ms)
	 */
	C_BT();
	
	/**
	 * \brief 
	 * コンストラクタ。使用するUARTとピンの設定を行う
	 * 115.2 [kbps]
	 * 倍速許可
	 * パリティ禁止
	 * 
	 * \param _arg_uart_addr	: Bluetoothと接続するUARTのレジスタ
	 * \param _arg_io_addr_rts  : RTSピンのレジスタ
	 * \param _arg_bit_rts		: RTSピンのビット
	 * \param _arg_io_addr_cts  : CTSピンのレジスタ
	 * \param _arg_bit_cts		: CTSピンのビット
	 * \param _arg_io_addr_rse	: RESETピンのレジスタ
	 * \param _arg_bit_rse		: RESETピンのビット
	 */
	C_BT(E_UART_ADDR _arg_uart_addr,E_IO_PORT_ADDR _arg_io_addr_rts,E_IO_NUM _arg_bit_rts,E_IO_PORT_ADDR _arg_io_addr_cts,E_IO_NUM _arg_bit_cts,E_IO_PORT_ADDR _arg_io_addr_rse,E_IO_NUM _arg_bit_rse);
	
	/**
	 * \brief CTSをHIGHにして、Bluetoothからの送信を禁止する。
	 */
	void Rce_off()	{	__CTS_HIGH__;	}
	
	/**
	 * \brief CTSをLOWにして、Bluetoothからの送信を許可する。
	 */
	void Rce_on()	{	__CTS_LOW__;	}
	
	/**
	 * \brief 
	 * Bluetoothへデータを送信する
	 * 送信に移れなかった場合はフラグをFALESにしてタイムアウトする。
	 * 
	 * \param _arg_out_data : 送信するデータ。
	 */
	void Out(const char _arg_out_data[]);
	
	/**
	 * \brief 
	 * Bluetoothからのデータを受け取る
	 * 受信に移れなかったら、タイムアウトする。
	 * 
	 * \param _arg_re_in_data : ここに受信データが格納される
	 */
	void In(char _arg_in_data[]);
	
	/**
	 * \brief 
	 * _arg_bt_str_compと一致するまで受信し続ける
	 * ちゃんとしないと無限ループに陥るので注意
	 * 
	 * \param _arg_str_comp : 比較する文字列
	 */
	void In_comp(const char _arg_str_comp[]);
	
	/**
	 * \brief 
	 * _arg_bt_str_compと一致するまで受信し続ける
	 * 一致したデータを返す
	 * 
	 * \param _arg_re_in_data : ここに受信データが格納される
	 * \param _arg_str_comp : 比較する文字列
	 */
	void In_comp(char _arg_re_in_data[], const char _arg_str_comp[]);
	
	/**
	 * \brief Bluetoothを再起動する
	 */
	void Reset();
	
	/**
	 * \brief 
	 * Bluetoothにデータを送信する演算子
	 * C_BT::Out()と同じ機能
	 * 
	 * \param _arg_bt
	 * \param _arg_out_data : 送信するデータ
	 */
	friend void operator << (C_BT &_arg_bt, const char _arg_out_data[]);
	
	/**
	 * \brief
	 * Bluetoothからのデータを受信する演算子。
	 * 右辺が定数の場合、それを受信するまで受信し続ける
	 * C_BT::In()とC_BT::In_comp()と同じ機能
	 * 
	 * \param _arg_bt
	 * \param _arg_re_in_data[] : データが格納される場所
	 */
	friend void operator >> (C_BT &_arg_bt, char _arg_re_in_data[]);
	
	/**
	 * \brief
	 * Bluetoothからのデータを受信する演算子。
	 * 右辺が定数の場合、それを受信するまで受信し続ける
	 * C_BT::In()とC_BT::In_comp()と同じ機能
	 * 
	 * \param _arg_bt
	 * \param _arg_str_comp : 比較する文字列
	 */
	friend void operator >> (C_BT &_arg_bt, const char _arg_str_comp[]);
	
	/**
	 * \brief 
	 * Bluetoothと接続しているのか確認用の演算子。
	 * TRUEを表す場合接続、FALESを表す場合切断
	 * 
	 * \param _arg_bt
	 * \param _arg_flag_comp : 比較
	 * 
	 * \return bool _arg_btと_arg_bt_flag_compが等しいときtrue
	 */
	friend bool operator == (C_BT &_arg_bt, BOOL _arg_flag_comp);
	
	/**
	 * \brief 
	 * Bluetoothと接続しているのか確認用の演算子。
	 * TRUEを表す場合接続、FALESを表す場合切断
	 * 
	 * \param _arg_bt
	 * \param _arg_flag_comp : 比較
	 * 
	 * \return bool _arg_btと_arg_flag_compが等しくないときtrue
	 */
	friend bool operator != (C_BT &_arg_bt, BOOL _arg_flag_comp);
};

#include "H28_C_BT.cpp"
