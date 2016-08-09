
#pragma once

#include "H28_C_BT_304_SLAVE.h"

C_BT_304_SLAVE::
C_BT_304_SLAVE
(
	E_UART_ADDR		_arg_uart_addr,
	E_IO_PORT_ADDR	_arg_io_addr_rts,
	E_IO_NUM		_arg_bit_rts,
	E_IO_PORT_ADDR	_arg_io_addr_cts,
	E_IO_NUM		_arg_bit_cts,
	E_IO_PORT_ADDR	_arg_io_addr_rse,
	E_IO_NUM		_arg_bit_rse
)
 : C_BT_304
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
	_mem_bt_slave_flag_count = 0;
	_mem_bt_slave_falg = FALSE;
}

void 
C_BT_304_SLAVE::
Connect()
{
	char CONNECTING[] = "\r\n+CONNECTING=123456789abc\r\n";
	char CONNECTED[]  = "\r\n+CONNECTED=123456789abc\r\n";
	
	for (usint i = 0; i < BT_ADDR_BYTE; i++)
	{
		CONNECTING[14 + i] = _mem_bt_addr[i];
		CONNECTED [13 + i] = _mem_bt_addr[i];
	}
	
	go_re_connect:
	
	_mem_bt << "AT+CONSLAVE=1,0\r\n";
	
	_mem_bt >> "\r\nACK\r\n";
	
	char _in_data[40] = {};
	
	do
	{
		_mem_bt >> _in_data;
		
		if (strcmp("\r\nTIMEOUT\r\n",_in_data) == 0)	goto go_re_connect;
		
	} while (strcmp(CONNECTING,_in_data) != 0);
	
	_mem_bt >> "\r\n+ACCEPT?\r\n";
	
	_mem_bt << "AT+RESCON=1\r\n";
	
	_mem_bt.In_comp(CONNECTED);
	
	_mem_bt >> "\r\nOK\r\n";
}

inline void 
C_BT_304_SLAVE::
Connect (const char _arg_bt_addr[BT_ADDR_BYTE])
{
	Set_bt_addr(_arg_bt_addr);
	
	Connect();
}

void 
C_BT_304_SLAVE::
Re_Connect()
{
	_mem_bt.Reset();
	
	_mem_bt >> "\r\nOK\r\n";
	
	Connect();
}

void 
C_BT_304_SLAVE::
In (char _arg_re_data_in[])
{
	char _in_data[30] = {};

	_mem_bt_slave_falg = FALSE;

	_mem_bt >> _in_data;
	
	if (_mem_bt == TRUE)
	{
		_mem_bt_slave_flag_count = 0;
	}
	else
	{
		_mem_bt_slave_flag_count ++;
	}
	
	if  (	
			(strcmp("\r\n+LINK_LOST=000",_in_data) == 0)	|| 
			(strcmp("\r\n+DISCONNECTED=000",_in_data) == 0)	|| 
			(_mem_bt_slave_flag_count > 200)
		)
	{		
		_mem_bt_slave_flag_count = 0;
		
		return (void)0;
	}
	
	_mem_bt_slave_falg = TRUE;
	
	for (usint i = 0; i < BT_DATA_NUM; i++)
	{
		if (_in_data[i * 2 + 0] <= 0x39)
		{
			_arg_re_data_in[i] = ((_in_data[i * 2] & 0x0f) << 4);
		}
		else
		{
			_arg_re_data_in[i] = (((_in_data[i * 2] & 0x0f) + 9) << 4);
		}
		
		if (_in_data[i * 2 + 1] <= 0x39)
		{
			_arg_re_data_in[i] |= (_in_data[i * 2 + 1] & 0x0f);
		}
		else
		{
			_arg_re_data_in[i] |= ((_in_data[i * 2 + 1] & 0x0f) + 9);
		}
	}
}

void 
operator >>
(
	C_BT_304_SLAVE &_arg_bt_slave,
	char _arg_re_data_in[]
)
{
	_arg_bt_slave.In(_arg_re_data_in);
}

bool 
operator ==
(
	C_BT_304_SLAVE &_arg_bt_slave,
	BOOL _arg_flag_comp
)
{
	if	(
			(_arg_bt_slave._mem_bt == _arg_flag_comp)			 || 
			(_arg_bt_slave._mem_bt_slave_falg == _arg_flag_comp)
		)
	{
		return true;
	}
	
	return false;
}

bool
operator !=
(
	C_BT_304_SLAVE &_arg_bt_slave,
	BOOL _arg_flag_comp
)
{
	if	(
			(_arg_bt_slave._mem_bt != _arg_flag_comp)			 ||
			(_arg_bt_slave._mem_bt_slave_falg != _arg_flag_comp)
		)
	{
		return true;
	}
	
	return false;
}