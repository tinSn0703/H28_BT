

#pragma once

#include "H28_C_BT.h"

C_BT :: 
C_BT ()
//: _mem_uart_timer(100)
{}

C_BT::
C_BT (E_UART_ADDR _arg_uart_addr)
//: _mem_uart_timer(100) //10ms
{	
	C_UART_base::_mem_uart_base_addr = _arg_uart_addr;
	
	//115.2kbps double-on parity-off
	__UBRRH__ = 0x00;
	__UBRRL__ = 0x0a;
	__UCSRA__ = (1<<U2X);
	__UCSRB__ = 0x00;
	__UCSRC__ = ((1 << UCSZ0) | (1 << UCSZ1));

	__CTS_SET__;	__RSE_SET__;	__RTS_SET__;
	
	__CTS_HIGH__;	__RSE_HIGH__;
	
	_mem_bt_flag = FALSE;
}

inline void 
C_BT :: 
Set_isr(BOOL _arg_nf_isr)
{
	switch (_arg_nf_isr)
	{
		case TRUE:	__UCSRB__ |=  (1 << RXCIE);	break; //On
		case FALSE:	__UCSRB__ &= ~(1 << RXCIE);	break; //Off
	}
}

inline void 
C_BT :: 
Set_isr_on ()
{
	__UCSRB__ |=  (1 << RXCIE);
}


inline void
C_BT ::
Set_isr_off ()
{
	__UCSRB__ &= ~(1 << RXCIE);
}

inline void 
C_BT :: 
Out (const char _arg_out_data[])
{
	__UCSRB__ |= (1 << TXEN);
	
	for (usint i = 0; _arg_out_data[i] != '\0'; i++)
	{

#ifndef __DEBUG__

		while (__RTS_READ__);
		
#endif
		
		while ( ! (__UCSRA__ & (1<<UDRE)));
		
		__UDR__ = _arg_out_data[i];
	}
}

inline void 
C_BT ::
In (char _arg_re_in_data[])
{
	__UCSRB__ |= (1 << RXEN);
	
	usint i = 0;
	
	while (1)
	{
		__CTS_LOW___;
		
		while ( ! (__UCSRA__ & (1 << RXC)));
		
		if ((__UCSRA__ & ((1 << FE) | (1 << DOR) | (1 << UPE))) == 0x00)
		{
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
}

// inline BOOL
// C_BT::
// Out_check (const char _arg_out_data[])
// {	
// 	for (usint i = 0; _arg_out_data[i] != '\0'; i++) //out loop
// 	{
// 		_mem_uart_timer.Start();
// 		
// 		while (1) //check loop
// 		{
// 			if ((_mem_uart_timer.Ret_flag() & __RTS_READ__) == TRUE) //通信可能
// 			{
// 				_mem_uart_timer.End();
// 				
// 				_mem_bt_flag = TRUE;
// 				
// 				goto Go_succe; //check loop を終了させます
// 			}
// 			
// 			if (_mem_uart_timer.Check())	//カウント完了(タイムアウト)
// 			{				
// 				_mem_bt_flag = FALSE;
// 				
// 				goto Go_fail; //out loop を終了させます。
// 			}
// 		}
// 		
// 		Go_succe:
// 		
// 		while ( ! (__UCSRA__ & (1<<UDRE)));
// 		
// 		__UDR__ = _arg_out_data[i];
// 	}
// 	
// 	Go_fail:
// 	
// 	return _mem_bt_flag;
// 	
// }
// 
// inline BOOL
// C_BT::
// In_check (char _arg_re_in_data[])
// {	
// 	usint i = 0;
// 	
// 	while (1)
// 	{
// 		__CTS_LOW___;
// 		
// 		_mem_uart_timer.Start();
// 		
// 		while (1)
// 		{
// 			if ((_mem_uart_timer.Ret_flag() & CHECK_BIT_TF(__UCSRA__,RXC)) == TRUE)	//通信可能
// 			{
// 				_mem_uart_timer.End();
// 				
// 				_mem_bt_flag = TRUE;
// 				
// 				goto Go_succe;
// 			}
// 			
// 			if (_mem_uart_timer.Check())	//カウント完了(タイムアウト)
// 			{				
// 				__CTS_HIGH__;
// 				
// 				_mem_bt_flag = FALSE;
// 				
// 				goto Go_fail;
// 			}
// 		}
// 		
// 		Go_succe:
// 		
// 		if ((__UCSRA__ & ((1 << FE) | (1 << DOR) | (1 << UPE))) == 0x00)
// 		{
// 			_arg_re_in_data[i] = __UDR__;
// 			
// 			__CTS_HIGH__;
// 			
// 			if ((_arg_re_in_data[i] == '\n') && (i > 1))
// 			{
// 				_arg_re_in_data[i + 1] = '\0';
// 				
// 				break;
// 			}
// 			
// 			i++;
// 		}
// 	}
// 	
// 	Go_fail:
// 	
// 	return _mem_bt_flag;
// }

void 
C_BT::
In_comp (const char _arg_str_comp[])
{
#ifndef __DEBUG__
	
	char _in_data[40] = {};
	
	do
	{
		In(_in_data);
	}
	while (F_Str_comp(_arg_str_comp,_in_data) == FALSE);
	
#endif
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
	while (F_Str_comp(_arg_str_comp,_arg_re_in_data) == FALSE);
}

void 
C_BT::
Reset ()
{	
	__RSE_LOW___;
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
operator >= 
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