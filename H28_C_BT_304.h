
#pragma once

#include "H28_C_BT.h"

/**
 * Bluetoothを扱うクラス
 */
class C_BT_304
{
protected:
	
	char _mem_bt_addr[BT_ADDR_BYTE];
	
	C_BT _mem_bt;
	
public:
	
	/**
	 * \brief 
	 * 空のコンストラクタ。何もしない
	 */
	C_BT_304()	{}
		
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
	C_BT_304(E_UART_ADDR _arg_uart_addr, E_IO_PORT_ADDR _arg_io_addr_rts, E_IO_NUM _arg_bit_rts, E_IO_PORT_ADDR _arg_io_addr_cts, E_IO_NUM _arg_bit_cts, E_IO_PORT_ADDR	_arg_io_addr_rse, E_IO_NUM _arg_bit_rse);
	
	/**
	 * \brief 通信する相手のBluetoothのアドレスの設定
	 * 
	 * \param _arg_bt_addr : ここにアドレスを入力する
	 */
	void Set_bt_addr(const char _arg_bt_addr[]);
	
	/**
	 * \brief 
	 * 接続されているBluetoothのアドレスを取得する
	 * リンクが確立した後は使えません
	 * 
	 * \param _arg_re_bt_addr : 取得したアドレスがここに格納される
	 */
	void Get_bt_addr(char _arg_re_bt_addr[]);
		
	/**
	 * \brief 通信する相手のアドレスを出力する
	 * 
	 * \param _arg_re_bt_addr : 通信する相手のアドレスがここに格納される
	 */
	void Ret_bt_addr(char _arg_re_bt_addr[]);
};

#include "H28_C_BT_304.cpp"