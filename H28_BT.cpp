
#pragma once

inline int
strcmp
(
	const char *_arg_str_f,
	char *_arg_str
)
{
	for (usint i = 0; _arg_str_f[i] != '\0'; i++)
	{
		if (_arg_str_f[i] != _arg_str[i])
		{
			return 1;
		}
	}
	
	return 0;
}

inline char *
F_str_copy
(
	char _arg_str_base[],
	const char _arg_str[]
)
{
	for (uint i = 0; (_arg_str_base[i] == '\0') || (_arg_str[i] == '\0'); i++)
	{
		_arg_str_base[i] = _arg_str[i];
	}
	
	return _arg_str_base;
}

void 
F_Set_bt (E_UART_ADDR _arg_uart_addr)
{
	DDRD	|= (__CTS__ | __RSE__);
	PORTD	|= __RTS__;
	
	F_uart_set_detail(_arg_uart_addr, 10, EU_NON);
	
	F_In_comp_bt(_arg_uart_addr, "\r\nOK\r\n");
}

void 
F_Out_bt 
(
	E_UART_ADDR _arg_uart_addr_t, 
	const char _arg_data_out[]
)
{
	for (usint i = 0; _arg_data_out[i] != '\0'; i++)
	{
		while (__RTS_READ__);
		
		F_uart_out_8(_arg_uart_addr_t, _arg_data_out[i]);
	}
}

char *
F_In_bt 
(
	E_UART_ADDR _arg_uart_addr_r, 
	char _arg_data_in[]
)
{
	usint i = 0;
	
	while (1)
	{
		__CTS_LOW___;
		
		T_DATA_8 _temp_data_in = 0;
		
		E_UART_FLAG _flag = F_uart_in_8(_arg_uart_addr_r, &_temp_data_in);
		
		__CTS_HIGH__;
		
		if (_flag != EU_ERROR)
		{
			_arg_data_in[i] = _temp_data_in;
			
			if ((_arg_data_in[i] == '\n') && (1 < i))	break;
			
			i++;
		}
	}
	
	_arg_data_in[i + 1] = '\0';
	
	return _arg_data_in;
}

char *
F_In_bt 
(
	E_UART_ADDR _arg_uart_addr_r, 
	char _arg_data_in[], 
	const char _arg_data_comp[]
)
{
	do 
	{
		F_In_bt(_arg_uart_addr_r, _arg_data_in);
		
	} while (strcmp(_arg_data_comp, _arg_data_in));
	
	return _arg_data_in;
}

void 
F_In_comp_bt
(
	E_UART_ADDR _arg_uart_addr_r,
	const char _arg_data_comp[]
)
{
	char _data_in[40] = {};
	
	do
	{
		F_In_bt(_arg_uart_addr_r, _data_in);
		
	} while (strcmp(_arg_data_comp, _data_in));
}

#if defined(_AKILCD_H_)

char * 
F_Set_bt_addr
(
	E_BT_NUM _arg_num_bt,
	char _arg_bt_addr[],
	char _arg_char_num
)
{
	switch (_arg_num_bt)
	{
		case EB_BT0:	F_str_copy(_arg_bt_addr ,"000190EAD4B5"); _arg_char_num = '1';	break;
		case EB_BT1:	F_str_copy(_arg_bt_addr ,"000190EAD491"); _arg_char_num = '2';	break;
		case EB_BT2:	F_str_copy(_arg_bt_addr ,"000190E53F09"); _arg_char_num = '3';	break;
		case EB_BT3:	F_str_copy(_arg_bt_addr ,"000190EAD491"); _arg_char_num = '4';	break;
		case EB_BT4:	F_str_copy(_arg_bt_addr ,"000190EAD49A"); _arg_char_num = '5';	break;
		case EB_BT5:	F_str_copy(_arg_bt_addr ,"000190E53A7B"); _arg_char_num = '6';	break;
		case EB_BT6:	F_str_copy(_arg_bt_addr ,"000190EB1273"); _arg_char_num = '7';	break;
		case EB_BT7:	F_str_copy(_arg_bt_addr ,"000190EB1416"); _arg_char_num = '8';	break;
		case EB_BT8:	F_str_copy(_arg_bt_addr ,"000190EB105D"); _arg_char_num = '9';	break;
		case EB_BT9:	F_str_copy(_arg_bt_addr ,"000190EB1057"); _arg_char_num = 'A';	break;
		case EB_BTA:	F_str_copy(_arg_bt_addr ,"000190EB105A"); _arg_char_num = 'B';	break;
		case EB_BTB:	F_str_copy(_arg_bt_addr ,"000190EB1052"); _arg_char_num = 'C';	break;
		case EB_BTC:	F_str_copy(_arg_bt_addr ,"000190EB1050"); _arg_char_num = 'D';	break;
		case EB_BTD:	F_str_copy(_arg_bt_addr ,"000190EB105C"); _arg_char_num = 'E';	break;
		case EB_BTE:	F_str_copy(_arg_bt_addr ,"000190EB104F"); _arg_char_num = 'F';	break;
		case EB_BTF:	F_str_copy(_arg_bt_addr ,"000190EB104E"); _arg_char_num = '0';	break;
		default:																		break;
	}
	
	return _arg_bt_addr;
}

#else

char * 
F_Set_bt_addr
(
	E_BT_NUM _arg_num_bt,	
	char _arg_bt_addr[]
)
{
	switch (_arg_num_bt)
	{
		case EB_BT0:	F_str_copy(_arg_bt_addr ,"000190EAD4B5");	break;
		case EB_BT1:	F_str_copy(_arg_bt_addr ,"000190EAD491");	break;
		case EB_BT2:	F_str_copy(_arg_bt_addr ,"000190E53F09");	break;
		case EB_BT3:	F_str_copy(_arg_bt_addr ,"000190EAD491");	break;
		case EB_BT4:	F_str_copy(_arg_bt_addr ,"000190EAD49A");	break;
		case EB_BT5:	F_str_copy(_arg_bt_addr ,"000190E53A7B");	break;
		case EB_BT6:	F_str_copy(_arg_bt_addr ,"000190EB1273");	break;
		case EB_BT7:	F_str_copy(_arg_bt_addr ,"000190EB1416");	break;
		case EB_BT8:	F_str_copy(_arg_bt_addr ,"000190EB105D");	break;
		case EB_BT9:	F_str_copy(_arg_bt_addr ,"000190EB1057");	break;
		case EB_BTA:	F_str_copy(_arg_bt_addr ,"000190EB105A");	break;
		case EB_BTB:	F_str_copy(_arg_bt_addr ,"000190EB1052");	break;
		case EB_BTC:	F_str_copy(_arg_bt_addr ,"000190EB1050");	break;
		case EB_BTD:	F_str_copy(_arg_bt_addr ,"000190EB105C");	break;
		case EB_BTE:	F_str_copy(_arg_bt_addr ,"000190EB104F");	break;
		case EB_BTF:	F_str_copy(_arg_bt_addr ,"000190EB104E");	break;
		default:													break;
	}
	
	return _arg_bt_addr;
}

#endif

BOOL  
F_Connect_bt_slave 
(
	E_UART_ADDR _arg_uart_addr,
	E_BT_NUM _arg_num_bt
)
{
	char _bt_master_addr[__BT_ADDR_BYTE__] = {};
	
#if defined(_AKILCD_H_)
	
	Lcd_put_str(0x40, "Now setting     ");
	
	char _bt_master_num = 0;

	F_Set_bt_addr(_arg_num_bt, _bt_master_addr, _bt_master_num);
	
	char _state_ing[15] = "Connecting_0x ";	_state_ing[13] = _bt_master_num;
	char _state_ed [15] = "Connected_0x  ";	_state_ed [12] = _bt_master_num;
	
	Lcd_put_str(0x40,_state_ing);
	
#else
	
	F_Set_bt_addr(_arg_num_bt, _bt_master_addr);
	
#endif
	
	char CONNECTING[] = "\r\n+CONNECTING=123456789abc\r\n";
	char CONNECTED[]  = "\r\n+CONNECTED=123456789abc\r\n";
	
	for (usint i = 0; i < __BT_ADDR_BYTE__; i++)
	{
		CONNECTING[14 + i] = _bt_master_addr[i];
		CONNECTED [13 + i] = _bt_master_addr[i];
	}
	
	go_re_connect:
	
	F_Out_bt(_arg_uart_addr, "AT+CONSLAVE=1,0\r\n");
	
	F_In_comp_bt(_arg_uart_addr, "\r\nACK\r\n");
	
	char _data_in[40] = {};
	
	do
	{
		F_In_bt(_arg_uart_addr, _data_in);
		
		if (strcmp("\r\nTIMEOUT\r\n",_data_in) == 0)	goto go_re_connect;
		
	} while (strcmp(CONNECTING,_data_in) != 0);
	
	F_In_comp_bt(_arg_uart_addr, "\r\n+ACCEPT?\r\n");
	
	F_Out_bt(_arg_uart_addr, "AT+RESCON=1\r\n");
	
	F_In_comp_bt(_arg_uart_addr, CONNECTED);
	
	F_In_comp_bt(_arg_uart_addr, "\r\nOK\r\n");
	
#ifdef _AKILCD_H_
	Lcd_put_str(0x40,_state_ed);
#endif
	
	return TRUE;
}

BOOL  
F_Connect_bt_master 
(
	E_UART_ADDR _arg_uart_addr, 
	E_BT_NUM _arg_num_bt
)
{
	char _bt_slave_addr[__BT_ADDR_BYTE__] = {};
	
#if defined(_AKILCD_H_)
	
	char _bt_slave_num = 0;

	Lcd_put_str(0x40, "Now setting     ");

	F_Set_bt_addr(_arg_num_bt, _bt_slave_addr, _bt_slave_num);
	
	char _state_ing[15] = "Connecting_0x ";
	char _state_ed [15] = "Connected_0x  ";
	
	_state_ing[13] = _bt_slave_num;
	_state_ed [12] = _bt_slave_num;
	
	Lcd_put_str(0x40,_state_ing);
	
#else
	
	F_Set_bt_addr(_arg_num_bt, _bt_slave_addr);
	
#endif
	
	char CONMASTER[]  = "AT+CONMASTER=1,000190123456\r\n";
	char CONNECTED[]  = "\r\n+CONNECTED=000190123456\r\n";
	
	for (usint i = 0; i < __BT_ADDR_BYTE__; i++)
	{
		CONMASTER [15 + i] = _bt_slave_addr[i];
		CONNECTED [13 + i] = _bt_slave_addr[i];
	}
	
	re_connect:
	
	F_Out_bt(_arg_uart_addr, CONMASTER);
	
	F_In_comp_bt(_arg_uart_addr, "\r\nACK\r\n");
	
	char _in_data[40] = {};
	
	do
	{
		F_In_bt(_arg_uart_addr, _in_data);
		
		if (strcmp("\r\nTIMEOUT\r\n",_in_data) == 0)	goto re_connect;
		
	} while (strcmp(CONNECTED,_in_data) != 0);
	
	F_In_comp_bt(_arg_uart_addr, "\r\nOK\r\n");
	
#ifdef _AKILCD_H_
	Lcd_put_str(0x40,_state_ed);
#endif

	return TRUE;
}

void 
F_Re_Connect_bt_slave
(
	E_UART_ADDR _arg_uart_addr, 
	E_BT_NUM _arg_num_bt
)
{
	F_Cut_Connect_bt();
	
	F_In_comp_bt(_arg_uart_addr, "\r\nOK\r\n");
	
	F_Re_Connect_bt_slave(_arg_uart_addr, _arg_num_bt);
}

void
F_Re_Connect_bt_master
(
	E_UART_ADDR _arg_uart_addr, 
	E_BT_NUM _arg_num_bt
)
{
	F_Cut_Connect_bt();
	
	F_In_comp_bt(_arg_uart_addr, "\r\nOK\r\n");
	
	F_Re_Connect_bt_master(_arg_uart_addr, _arg_num_bt);
}

void 
F_Cut_Connect_bt ()
{
	__RSE_LOW___;
	_delay_ms(15);

	__RSE_HIGH__;
	_delay_ms(15);
}