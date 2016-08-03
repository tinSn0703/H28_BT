
#pragma once

#include "H28_C_BT_304.h"

/**
 * BluetoothをMasterとして扱うクラス
 */
class C_BT_304_MASTER : public C_BT_304
{
public:

	/**
	 * \brief 
	 * 空のコンストラクタ。
	 */
	C_BT_304_MASTER()	{}
		
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
	C_BT_304_MASTER(E_UART_ADDR _arg_uart_addr,E_IO_PORT_ADDR _arg_io_addr_rts,E_IO_NUM _arg_bit_rts,E_IO_PORT_ADDR _arg_io_addr_cts,E_IO_NUM _arg_bit_cts,E_IO_PORT_ADDR _arg_io_addr_rse,E_IO_NUM _arg_bit_rse);
	
	/**
	 * \brief Masterと接続する。アドレスを入力するタイプ。
	 * 
	 * \param _arg_bt_addr : アドレス。チェック系はないので
	 */
	void Connect(const char _arg_bt_addr[]);
	
	/**
	 * \brief Masterと接続する。アドレスはあらかじめ入力されたものを使う
	 */
	void Connect();
	
	/**
	 * \brief 再接続。
	 */
	void Re_Connect();
	
	/**
	 * \brief 
	 * Slaveへ変換してから送る
	 * 具体的には数値を文字列に変換する
	 * 
	 * \param _arg_out_data : 送信するデータ
	 */
	void Out(const char _arg_out_data[]);
	
	/**
	 * \brief 
	 * Slaveへデータを送信する。
	 * 機能的にはC_BT_304_MASTER::Out()と同じ
	 * 
	 * \param _arg_bt_master
	 * \param _arg_out_data : 送信するデータ
	 */
	friend void operator << (C_BT_304_MASTER &_arg_bt_master, const char _arg_out_data[]);
	
	/**
	 * \brief 
	 * Slaveと通信しているかを確認する用の演算子
	 * TRUEを表すとき接続。FALESの場合切断。
	 * 
	 * \param _arg_bt_master
	 * \param _arg_flag_comp : 比較
	 * 
	 * \return bool 
	 * _arg_bt_masterと_arg_flag_compが等しいときtrue
	 */
	friend bool operator == (C_BT_304_MASTER &_arg_bt_master, BOOL _arg_flag_comp);
	
	/**
	 * \brief 
	 * Slaveと通信しているかを確認する用の演算子
	 * TRUEを表すとき接続。FALESの場合切断。
	 * 
	 * \param _arg_bt_master
	 * \param _argr_flag_comp : 比較
	 * 
	 * \return bool 
	 * _arg_bt_masterと_arg_flag_compが等しくないときtrue
	 */
	friend bool operator != (C_BT_304_MASTER &_arg_bt_master, BOOL _arg_flag_comp);
};

#include "H28_C_BT_304_MASTER.cpp"