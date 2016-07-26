
#pragma once

#include "H28_C_BT.cpp"

class C_BT_304
{
	protected:
	char _mem_bt_addr[BT_ADDR_BYTE];
	
	public:
	C_BT _mem_bt;
	
	C_BT_304()	{}
	C_BT_304(E_UART_ADDR ,E_IO_PORT_ADDR, E_IO_NUM, E_IO_PORT_ADDR, E_IO_NUM , E_IO_PORT_ADDR ,E_IO_NUM);
	
	void Set_bt_addr(const char []);
	void Get_bt_addr(char []);
};

/************************************************************************/

C_BT_304::
C_BT_304
(
	E_UART_ADDR		_arg_bt_uart_addr,
	E_IO_PORT_ADDR	_arg_bt_addr_rts, 
	E_IO_NUM		_arg_bt_bit_rts, 
	E_IO_PORT_ADDR	_arg_bt_addr_cts,
	E_IO_NUM		_arg_bt_bit_cts,
	E_IO_PORT_ADDR	_arg_bt_addr_rse,
	E_IO_NUM		_arg_bt_bit_rse
)
/*
見ての通りコンストラクタ。
継承の都合上、引数を持たない奴もあるから気を付けてね

	_arg_bt_uart_addr	: Bluetoothと接続するUARTのレジスタ
	_arg_bt_addr_rts	: RTSピンのレジスタ
	_arg_bt_bit_rts		: RTSピンのビット番号
	_arg_bt_addr_cts	: CTSピンのレジスタ
	_arg_bt_bit_cts		: CTSピンのビット番号
	_arg_bt_addr_rse	: RESETピンのレジスタ
	_arg_bt_bit_rse		: RESETピンのビット番号
*/
 : _mem_bt
(
	_arg_bt_uart_addr,
	_arg_bt_addr_rts,
	_arg_bt_bit_rts,
	_arg_bt_addr_cts,
	_arg_bt_bit_cts,
	_arg_bt_addr_rse,
	_arg_bt_bit_rse
)
{
	_mem_bt >> "\r\nOK\r\n";
}

/************************************************************************/

inline void
C_BT_304::
Set_bt_addr(const char _arg_bt_addr[])
/*
アドレスの設定

	_arg_bt_addr : アドレスを入力してね
*/
{
	for (usint i = 0; i < BT_ADDR_BYTE; i++)
	{
		_mem_bt_addr[i] = _arg_bt_addr[i];
	}
}

/************************************************************************/

inline void 
C_BT_304::
Get_bt_addr(char _arg_re_bt_addr[])
/*
Bluetoothのアドレスを取得する。
コネクトした後は使えないのであしからず

	_arg_re_bt_addr : ここにアドレスが入ります
*/
{
	_mem_bt << "AT+GETADDR\r\n";
	
	char _in_data[27] = {};
	
	_mem_bt.In_comp(_in_data,"\r\nBDADDR=");
	
	_mem_bt >> "\r\nOK\r\n";
	
	for (uint i = 0; i < BT_ADDR_BYTE; i++)
	{
		_arg_re_bt_addr[i] = _in_data[9 + i];
	}
	
	_arg_re_bt_addr[BT_ADDR_BYTE] = '\0';
}