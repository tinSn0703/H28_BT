
#pragma once

#include "H28_C_BT_304.h"

/**
 * BluetoothをSlaveで使うクラス。
 */
class C_BT_304_SLAVE : public C_BT_304
{
protected:
	
	T_NUM _mem_bt_slave_flag_count;
	BOOL _mem_bt_slave_falg :1;
	
public:
	
	/**
	 * \brief 
	 * 空のコンストラクタ。何もしない
	 */
	C_BT_304_SLAVE()	{}
		
	/**
	 * \brief コンストラクタ。BTの使用に必要な設定を行う
	 * 
	 * \param _arg_uart_addr	: Bluetoothと接続するUARTのレジスタ
	 * \param _arg_io_addr_rts  : RTSピンのレジスタ
	 * \param _arg_bit_rts		: RTSピンのビット
	 * \param _arg_io_addr_cts  : CTSピンのレジスタ
	 * \param _arg_bit_cts		: CTSピンのビット
	 * \param _arg_io_addr_rse	: RESETピンのレジスタ
	 * \param _arg_bit_rse		: RESETピンのビット
	 */
	C_BT_304_SLAVE(E_UART_ADDR _arg_uart_addr, E_IO_PORT_ADDR _arg_io_addr_rts, E_IO_NUM _arg_bit_rts, E_IO_PORT_ADDR _arg_io_addr_cts, E_IO_NUM _arg_bit_cts, E_IO_PORT_ADDR	_arg_io_addr_rse, E_IO_NUM _arg_bit_rse);
	
	/**
	 * \brief Masterと接続する。アドレスはあらかじめ入力されたものを使う
	 */
	void Connect();
	
	/**
	 * \brief Masterと接続する。アドレスを入力するタイプ。
	 * 
	 * \param _arg_bt_addr : アドレス。チェック系はないので
	 */
	void Connect(const char _arg_bt_addr[]);
	
	/**
	 * \brief 再接続。
	 */
	void Re_Connect();
	
	/**
	 * \brief 
	 * Masterからのデータを変換してから受け取る。
	 * 具体的には文字列に変換されたデータを数値に戻す。
	 * 
	 * \param _arg_re_data_in 
	 *		受信したデータの格納される場所。
	 *		要BT_DATA_NUM以上の要素数
	 */
	void In(char _arg_re_data_In[]);
	
	/**
	 * \brief 
	 * Masterからのデータを受け取る演算子
	 * 機能的にはC_BT_304_SLAVE::In()と同じ
	 * 
	 * \param _arg_bt_slave : みたまま
	 * \param _arg_re_data_in : データが格納される変数
	 */
	friend void operator >> (C_BT_304_SLAVE &_arg_bt_slave, char _arg_re_data_in[]);
	
	/**
	 * \brief 
	 * Masterと通信しているかを確認する用の演算子
	 * TRUEを表すとき接続。FALESの場合切断。
	 * 
	 * \param _arg_bt_slave
	 * \param _arg_flag_comp : 比較
	 * 
	 * \return bool 
	 * _arg_bt_slaveと_arg_flag_compが等しいときtrue
	 */
	friend bool operator == (C_BT_304_SLAVE &_arg_bt_slave, BOOL _arg_flag_comp);
	
	/**
	 * \brief 
	 * Masterと通信しているかを確認する用の演算子
	 * TRUEを表すとき接続。FALESの場合切断。
	 * 
	 * \param _arg_bt_slave
	 * \param _arg_flag_comp : 比較
	 * 
	 * \return bool 
	 * _arg_bt_slaveと_arg_flag_compが等しくないときtrue
	 */
	friend bool operator != (C_BT_304_SLAVE &_arg_bt_slave, BOOL _arg_flag_comp);
};

#include "H28_C_BT_304_SLAVE.cpp"
