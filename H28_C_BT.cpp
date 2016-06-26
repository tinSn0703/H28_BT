
#ifndef _H28_C_BT_CPP_
#define _H28_C_BT_CPP_ 1

#include <string.h>
#include <H28_AVR/H28_AVR.h>

#define LED4_ON  PORTC |=  (1 << 7)
#define LED4_OFF PORTC &= ~(1 << 7)

inline usint strcmp(const char *_arg_str_f, char *_arg_str)
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

inline usint strcmp(const char *_arg_str_0, const char *_arg_str_1, usint _arg_num)
{
	for (usint i = 0; i < _arg_num; i++)
	{
		if (_arg_str_0[i] == '\0')
		{
			break;
		}
		else if (_arg_str_0[i] != _arg_str_1[i])
		{
			return 1;
		}
	}
	
	return 0;
}

class C_BT : protected C_UART_base
{
	private:
	E_IO_PORT_ADDR _mem_bt_port_cts :8;
	E_IO_PORT_ADDR _mem_bt_port_rts :8;
	
	E_IO_NUM _mem_bt_bit_cts :8;
	E_IO_NUM _mem_bt_bit_rts :8;
	
	protected:
	E_IO_PORT_ADDR Ret_port_cts()	{	return _mem_bt_port_cts;	}
	E_IO_PORT_ADDR Ret_port_rts()	{	return _mem_bt_port_rts;	}
	
	E_IO_NUM Ret_bit_cts()	{	return _mem_bt_bit_cts;	}
	E_IO_NUM Ret_bit_rts()	{	return _mem_bt_bit_rts;	}
	
	#define PIN_RTS  _SFR_IO8(Ret_port_rts() + 0)
	#define DDR_CTS  _SFR_IO8(Ret_port_cts() + 1)
	#define DDR_RTS	 _SFR_IO8(Ret_port_rts() + 1)
	#define PORT_CTS _SFR_IO8(Ret_port_cts() + 2)
	
	#define RTS_CHECK (PIN_RTS & (1 << Ret_bit_rts()))
	
	#define CTS_HIGH	(PORT_CTS |=  (1 << Ret_bit_cts()))
	#define CTS_LOW		(PORT_CTS &= ~(1 << Ret_bit_cts()))
	
	void Out(const char[]);
	void In(char []);
	
	void In_comp(const char []);
	
	public:
	C_BT()	{}
	C_BT(E_UART_ADDR ,E_IO_PORT_ADDR ,E_IO_NUM ,E_IO_PORT_ADDR ,E_IO_NUM );
	
	void Rce_off()	{	CTS_HIGH;	}
	void Rce_on()	{	CTS_LOW;	}
		
	E_LOGIC Ret_rse_flag()	{	return CHECK_BIT_TF(UCSRA,RXC);	}
		
	friend void operator<< (C_BT &, const char []);
	friend void operator>> (C_BT &, char []);
	friend void operator>> (C_BT &, const char []);
	
	friend void operator== (C_BT &, const char []);
};

inline void C_BT::Out(const char _arg_bt_out_data[])
{
	usint i = 0;
	
	while (_arg_bt_out_data[i] != '\0')
	{
		while (RTS_CHECK != 0);
		
		while (!(UCSRA & (1<<UDRE)));
		UDR = _arg_bt_out_data[i];
		
		i++;
	}
}

inline void C_BT::In(char _arg_bt_in_data[])
{
	usint i = 0;
	
	while (1)
	{
		CTS_LOW;
		
		while (!(UCSRA & (1<<RXC)));
		_arg_bt_in_data[i] = UDR;
		
		CTS_HIGH;
		
		if ((_arg_bt_in_data[i] == '\n') && (i > 1))	break;
		
		i++;
	}
	
	_arg_bt_in_data[i + 1] = '\0';
	
	#ifdef _BT_TX_
	Lcd_put_str(0x40,_arg_bt_in_data);
	#endif
}

void C_BT::In_comp(const char _arg_str_comp[])
{
	char _in_data[40] = {};
	
	do
	{
		In(_in_data);
	}
	while (strcmp(_arg_str_comp,_in_data) != 0);
}

inline C_BT::C_BT(E_UART_ADDR _arg_bt_uart_addr, E_IO_PORT_ADDR _arg_bt_port_rts, E_IO_NUM _arg_bt_bit_rts, E_IO_PORT_ADDR _arg_bt_port_cts, E_IO_NUM _arg_bt_bit_cts)
{
	C_UART_base::Set_uart_base_addr(_arg_bt_uart_addr);
	
	_mem_bt_port_cts = _arg_bt_port_cts;
	_mem_bt_port_rts = _arg_bt_port_rts;
	
	_mem_bt_bit_cts = _arg_bt_bit_cts;
	_mem_bt_bit_rts = _arg_bt_bit_rts;
	
	//115.2kbps double-on parity-off
	//rec,tra-mode 8bit interrupt-off
	UBRRH = 0x00;
	UBRRL = 0x0a;
	UCSRA = (1<<U2X);
	UCSRB = ((1<<RXEN) | (1<<TXEN));
	UCSRC = ((1<<UCSZ0) | (1<<UCSZ1));

	DDR_CTS  |=  (1 << _arg_bt_bit_cts);
	DDR_RTS  &= ~(1 << _arg_bt_bit_rts);
	
	PORT_CTS |= (1 << _arg_bt_bit_cts);
}

void operator<< (C_BT &_arg_bt, const char _arg_out_data[])
{
	_arg_bt.Out(_arg_out_data);
}

void operator>> (C_BT &_arg_bt, char _arg_in_data[])
{
	_arg_bt.In(_arg_in_data);
}

void operator>> (C_BT &_arg_bt, const char _arg_str_comp[])
{
	_arg_bt.In_comp(_arg_str_comp);
}

void operator== (C_BT &_arg_bt, const char _arg_str_comp[])
{
	_arg_bt.In_comp(_arg_str_comp);
}

#include "H28_C_ROBOBA.cpp"

#endif