
#pragma once

#include "H28_C_BT_304.h"

C_BT_304::
C_BT_304
(
	E_UART_ADDR		_arg_uart_addr,
	E_IO_PORT_ADDR	_arg_io_addr_rts, 
	E_IO_NUM		_arg_bit_rts, 
	E_IO_PORT_ADDR	_arg_io_addr_cts,
	E_IO_NUM		_arg_bit_cts,
	E_IO_PORT_ADDR	_arg_io_addr_rse,
	E_IO_NUM		_arg_bit_rse
)
 : _mem_bt
(
	_arg_uart_addr,
	_arg_io_addr_rts,
	_arg_bit_rts,
	_arg_io_addr_cts,
	_arg_bit_cts,
	_arg_io_addr_rse,
	_arg_bit_rse
)
{
	_mem_bt >> "\r\nOK\r\n";
}

inline void
C_BT_304::
Set_bt_addr(const char _arg_bt_addr[])
{
	for (usint i = 0; i < BT_ADDR_BYTE; i++)
	{
		_mem_bt_addr[i] = _arg_bt_addr[i];
	}
}

inline void 
C_BT_304::
Get_bt_addr(char _arg_re_bt_addr[])
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

inline void 
C_BT_304 :: 
Ret_bt_addr (char _arg_re_bt_addr[])
{
	for (usint i = 0; i < BT_ADDR_BYTE; i++)
	{
		_arg_re_bt_addr[i] = _mem_bt_addr[i];
	}
}