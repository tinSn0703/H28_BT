

#pragma once

#include "H28_C_BT.h"

C_BT :: 
C_BT ()
: _mem_uart_timer(100)
{}

C_BT::
C_BT
(	
	E_UART_ADDR		_arg_uart_addr, 
	E_IO_PORT_ADDR	_arg_io_addr_rts,
	E_IO_NUM		_arg_bit_rts, 
	E_IO_PORT_ADDR	_arg_io_addr_cts, 
	E_IO_NUM		_arg_bit_cts,
	E_IO_PORT_ADDR	_arg_io_addr_rse,
	E_IO_NUM		_arg_bit_rse
)
: _mem_uart_timer(100) //10ms
{	
	C_UART_base::_mem_uart_base_addr = _arg_uart_addr;
	
	_mem_bt_io_addr_cts = _arg_io_addr_cts;
	_mem_bt_io_addr_rts = _arg_io_addr_rts;
	_mem_bt_io_addr_rse = _arg_io_addr_rse;
	
	_mem_bt_bit_cts = _arg_bit_cts;
	_mem_bt_bit_rts = _arg_bit_rts;
	_mem_bt_bit_rse = _arg_bit_rse;
	
	//115.2kbps double-on parity-off
	__UBRRH__ = 0x00;
	__UBRRL__ = 0x0a;
	__UCSRA__ = (1<<U2X);
	__UCSRB__ = ((1<<RXEN) | (1<<TXEN));
	__UCSRC__ = ((1<<UCSZ0) | (1<<UCSZ1));

	__DDR_CTS__ |=  (1 << _arg_bit_cts);
	__DDR_RSE__ |=  (1 << _mem_bt_bit_rse);
	__DDR_RTS__ &= ~(1 << _arg_bit_rts);
	
	__PORT_CTS__ |= (1 << _arg_bit_cts);
	__PORT_RSE__ |= (1 << _mem_bt_bit_rse);
	
	_mem_bt_flag = FALSE;
}

inline void
C_BT::
Out (const char _arg_out_data[])
{	
	for (usint i = 0; _arg_out_data[i] != '\0'; i++)
	{
		_mem_uart_timer.Start();
		
		while (1)
		{
			if ((_mem_uart_timer.Ret_flag() & __RTS_CHECK__) == TRUE) //通信可能
			{
				_mem_uart_timer.End();
				
				_mem_bt_flag = TRUE;
				
				goto Go_succe;
			}
			
			if (_mem_uart_timer.Check())	//カウント完了(タイムアウト)
			{				
				_mem_bt_flag = FALSE;
				
				return (void)0;
			}
		}
		
		Go_succe:
		
		while (!(__UCSRA__ & (1<<UDRE)));
		
		__UDR__ = _arg_out_data[i];
	}
}

inline void
C_BT::
In (char _arg_re_in_data[])
{	
	usint i = 0;
	
	while (1)
	{
		__CTS_LOW__;
		
		_mem_uart_timer.Start();
		
		while (1)
		{
			if ((_mem_uart_timer.Ret_flag() & CHECK_BIT_TF(__UCSRA__,RXC)) == TRUE)	//通信可能
			{
				_mem_uart_timer.End();
				
				_mem_bt_flag = TRUE;
				
				goto GO_succe;
			}
			
			if (_mem_uart_timer.Check())	//カウント完了(タイムアウト)
			{				
				__CTS_HIGH__;
				
				_mem_bt_flag = FALSE;
				
				return (void)0;
			}
		}
		
		GO_succe:
		
		_arg_re_in_data[i] = __UDR__;
		
		__CTS_HIGH__;
		
		if ((_arg_re_in_data[i] == '\n') && (i > 1))
		{
			_arg_re_in_data[i + 1] = '\0';
			
			break;
		}
		
		i++;
	}
}

void 
C_BT::
In_comp (const char _arg_str_comp[])
{
	char _in_data[40] = {};
	
	do
	{
		In(_in_data);
	}
	while (strcmp(_arg_str_comp,_in_data) != 0);
}

void
C_BT::
In_comp
(
	char _arg_re_in_data[],
	const char _arg_str_comp[]
)
{
	do
	{
		In(_arg_re_in_data);
	}
	while (strcmp(_arg_str_comp,_arg_re_in_data) != 0);
}

void 
C_BT::
Reset ()
{	
	__RSE_LOW__;
	_delay_ms(15);

	__RSE_HIGH__;
	_delay_ms(15);
}

void 
operator << 
(
	C_BT &_arg_bt, 
	const char _arg_out_data[]
)
{
	_arg_bt.Out(_arg_out_data);
}

void 
operator >> 
(
	C_BT &_arg_bt, 
	char _arg_re_in_data[]
)
{
	_arg_bt.In(_arg_re_in_data);
}

void 
operator >> 
(
	C_BT &_arg_bt, 
	const char _arg_str_comp[]
)
{
	_arg_bt.In_comp(_arg_str_comp);
}

bool 
operator == 
(
	C_BT &_arg_bt, 
	BOOL _arg_flag_comp
)
{
	if (_arg_bt._mem_bt_flag == _arg_flag_comp)	return true;
	
	return false;
}

bool 
operator != 
(
	C_BT &_arg_bt, 
	BOOL _arg_flag_comp
)
{
	if (_arg_bt._mem_bt_flag != _arg_flag_comp)	return true;
	
	return false;
}