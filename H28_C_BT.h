
#pragma once

#include "H28_BT.h"

/**
 * Bluetoothと送受信を行うためのクラス
 */
class C_BT : protected C_UART_base
{
private:
	
	BOOL _mem_bt_flag :1;
	
//	C_TIMER_inside _mem_uart_timer;
	
public:
	
	/**
	 * \brief 
	 * 空のコンストラクタ。内部タイマの設定のみを行う。(10ms)
	 */
	C_BT();
	
	/**
	 * \brief 
	 * コンストラクタ。使用するUARTの設定を行う
	 * 115.2 [kbps]
	 * 倍速許可
	 * パリティ禁止
	 * 
	 * \param _arg_uart_addr	: Bluetoothと接続するUARTのレジスタ
	 */
	C_BT (E_UART_ADDR _arg_uart_addr);
	
	/**
	 * \brief CTSをHIGHにして、Bluetoothからの送信を禁止する。
	 */
	void Rce_off()	{	__CTS_HIGH__;	}
	
	/**
	 * \brief CTSをLOWにして、Bluetoothからの送信を許可する。
	 */
	void Rce_on()	{	__CTS_LOW___;	}
	
	/**
	 * \brief 受信完了割り込みの設定を行う
	 * 
	 * \param _arg_nf_isr : ONOFF
	 */
	void Set_isr(BOOL _arg_nf_isr);
	
	/**
	 * \brief 受信完了割り込みをONにする
	 */
	void Set_isr_on();
	
	/**
	 * \brief 受信完了割り込みをOFFにする
	 */
	void Set_isr_off();
	
	void Out(const char _arg_out_data[]);
	
	void In(char _arg_re_in_data[]);
	
	/**
	 * \brief 
	 * Bluetoothへデータを送信する
	 * 送信に移れなかった場合はフラグをFALSEにしてタイムアウトする。
	 * 
	 * \param _arg_out_data : 送信するデータ。
	 */
//	BOOL Out_check(const char _arg_out_data[]);
	
	/**
	 * \brief 
	 * Bluetoothからのデータを受け取る
	 * 受信に移れなかったら、タイムアウトする。
	 * 
	 * \param _arg_re_in_data : ここに受信データが格納される
	 */
//	BOOL In_check(char _arg_in_data[]);
	
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
	friend void operator >= (C_BT &_arg_bt, const char _arg_str_comp[]);
	
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
