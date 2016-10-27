
#pragma once

inline BOOL
F_Str_comp
(
	const char	_arg_str_base[],
	char		_arg_str_comp[]
)
{
	for (usint i = 0; _arg_str_base[i] != '\0'; i++)	{	if (_arg_str_base[i] != _arg_str_comp[i])	return FALSE;	}
	
	return TRUE;
}

inline char *
F_Str_copy
(
	char		_arg_str_base[],
	const char	_arg_str_copy[]
)
{
	for (uint i = 0; (_arg_str_base[i] == '\0') || (_arg_str_copy[i] == '\0'); i++)	{	_arg_str_base[i] = _arg_str_copy[i];	}
	
	return _arg_str_base;
}

inline void 
F_Str_add
(
	char		_arg_str_base[], 
	const char	_arg_str_add[]
)
{
	usint i = 0;
	usint j = 0;
	
	while(_arg_str_base[i] != '\0')
	{
		i++;
	}
	
	while(_arg_str_add[j] != '\0')
	{
		_arg_str_base[i] = _arg_str_add[j];
		
		i++;	j++;
	}
	
	_arg_str_base[i] = '\0';
}

inline void 
F_BT_Out
(
	E_UART_ADDR _arg_uart_addr_t,
	const char _arg_data_out[]
)
{
	for (usint i = 0; _arg_data_out[i] != '\0'; i++)
	{
		while(__RTS_READ__);
		
		while ( ! (__UCSRA_F(_arg_uart_addr_t) & (1 << UDRE)));
		
		__UDR_F(_arg_uart_addr_t) = _arg_data_out[i];
	}
}

inline void 
F_BT_In
(
	E_UART_ADDR _arg_uart_addr_r,
	char		_arg_re_data_in[]
)
{	
	usint i = 0;
	
	while (1)
	{
		__CTS_LOW___;
		
		while ( ! (__UCSRA_F(_arg_uart_addr_r) & (1 << RXC)));
		
		 char _temp = __UDR_F(_arg_uart_addr_r);
		
		__CTS_HIGH__;
		
		if ((__UCSRA_F(_arg_uart_addr_r) & ((1 << FE) | (1 << DOR) | (1 << UPE))) == 0x00)
		{
			_arg_re_data_in[i] = _temp;
			
			if ((_arg_re_data_in[i] == '\n') && (i > 1))
			{
				_arg_re_data_in[i + 1] = '\0';
				
				break;
			}
			
			i++;
		}
	}
}

inline void 
F_BT_In_comp
(
	E_UART_ADDR _arg_uart_addr_r,
	char		_arg_re_data_in[],
	const char	_arg_data_comp[]
)
{
	do
	{
		F_BT_In(_arg_uart_addr_r, _arg_re_data_in);
		
	} while (F_Str_comp(_arg_data_comp, _arg_re_data_in) == FALSE);
}

inline void 
F_BT_In_comp
(
	E_UART_ADDR _arg_uart_addr_r,
	const char	_arg_data_comp[]
)
{
	char _data_in[40] = {};
	
	do
	{
		F_BT_In(_arg_uart_addr_r, _data_in);
		
	} while (F_Str_comp(_arg_data_comp, _data_in) == FALSE);
}

inline void 
F_BT_Cut ()
{
	__CTS_LOW___;
	
	__RSE_LOW___;
	_delay_ms(15);

	__RSE_HIGH__;
	_delay_ms(15);
}