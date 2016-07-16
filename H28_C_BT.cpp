

#ifndef _H28_C_BT_CPP_
#define _H28_C_BT_CPP_ 1

#include "H28_BT.h"

class C_BT : protected C_UART_base , public C_TIMER_inside
{
	private:
	E_IO_PORT_ADDR _mem_bt_port_cts :8;
	E_IO_PORT_ADDR _mem_bt_port_rts :8;
	
	E_IO_NUM _mem_bt_bit_cts :8;
	E_IO_NUM _mem_bt_bit_rts :8;
	
	E_LOGIC _mem_bt_flag :1;
	
	protected:
	E_IO_PORT_ADDR Ret_port_cts()	{	return _mem_bt_port_cts;	}
	E_IO_PORT_ADDR Ret_port_rts()	{	return _mem_bt_port_rts;	}
	
	E_IO_NUM Ret_bit_cts()	{	return _mem_bt_bit_cts;	}
	E_IO_NUM Ret_bit_rts()	{	return _mem_bt_bit_rts;	}
	
	#define PIN_RTS  _SFR_IO8(Ret_port_rts() + 0)
	#define PIN_CTS  _SFR_IO8(Ret_port_cts() + 0)
	#define DDR_RTS	 _SFR_IO8(Ret_port_rts() + 1)
	#define DDR_CTS  _SFR_IO8(Ret_port_cts() + 1)
	#define PORT_RTS _SFR_IO8(Ret_port_rts() + 2)
	#define PORT_CTS _SFR_IO8(Ret_port_cts() + 2)
	
	#define RTS_CHECK (CHECK_TURN_BIT_TF(PIN_RTS,Ret_bit_rts()))
	
	#define CTS_HIGH	(PORT_CTS |=  (1 << Ret_bit_cts()))
	#define CTS_LOW		(PORT_CTS &= ~(1 << Ret_bit_cts()))
	
	E_UART_FLAG Check();
	E_UART_FLAG Check_in();
	E_UART_FLAG Check_out();
	
	public:
	C_BT()	{}
	C_BT(E_UART_ADDR ,E_IO_PORT_ADDR ,E_IO_NUM ,E_IO_PORT_ADDR ,E_IO_NUM );
	
	void Rce_off()	{	CTS_HIGH;	}
	void Rce_on()	{	CTS_LOW;	}
	
	void Out(const char[]);
	void In(char []);
	void In_comp(const char []);
	
	friend void operator<< (C_BT &, const char []);
	friend void operator>> (C_BT &, char []);
	friend void operator>> (C_BT &, const char []);
	
	friend bool operator== (C_BT &, E_LOGIC );
	friend bool operator!= (C_BT &, E_LOGIC );
};

//protected
E_UART_FLAG C_BT::Check()
{
	C_TIMER_inside::Start();
	
	while (1)
	{
		if ((CHECK_BIT_TF(UCSRA,RXC) | RTS_CHECK) == TRUE)
		{
			if (C_TIMER_inside::Ret_flag())
			{
				C_TIMER_inside::End();
				
				_mem_bt_flag = TRUE;
				
				return EU_SUCCE;
			}
		}
		
		if (C_TIMER_inside::Check() == TRUE)	//カウント完了(タイムアウト)
		{
			_mem_bt_flag = FALES;
			
			return EU_ERROR;
		}
	}
	
	return EU_NONE;
}

E_UART_FLAG C_BT::Check_in()
{
	C_TIMER_inside::Start();
	
	while (1)
	{
		if ((C_TIMER_inside::Ret_flag() & CHECK_BIT_TF(UCSRA,RXC)) == TRUE)	//受信完了
		{
			C_TIMER_inside::End();
			
			_mem_bt_flag = TRUE;
			
			return EU_SUCCE;
		}
		
		if (C_TIMER_inside::Check() == TRUE)	//カウント完了(タイムアウト)
		{
			_mem_bt_flag = FALES;
			
			return EU_ERROR;
		}
	}
	
	return EU_NONE;
}

E_UART_FLAG C_BT::Check_out()
{
	C_TIMER_inside::Start();
	
	while (1)
	{
		if ((C_TIMER_inside::Ret_flag() & RTS_CHECK) == TRUE)	//受信完了
		{
			C_TIMER_inside::End();
			
			_mem_bt_flag = TRUE;
			
			return EU_SUCCE;
		}
		
		if (C_TIMER_inside::Check() == TRUE)	//カウント完了(タイムアウト)
		{
			_mem_bt_flag = FALES;
			
			return EU_ERROR;
		}
	}
	
	return EU_NONE;
}

//public
inline C_BT::C_BT(E_UART_ADDR _arg_bt_uart_addr, E_IO_PORT_ADDR _arg_bt_port_rts, E_IO_NUM _arg_bt_bit_rts, E_IO_PORT_ADDR _arg_bt_port_cts, E_IO_NUM _arg_bt_bit_cts)
{
	C_TIMER_inside::Set(80);
	
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
	
	_mem_bt_flag = FALES;
}

inline void C_BT::Out(const char _arg_bt_out_data[])
{	
	for (usint i = 0; _arg_bt_out_data[i] != '\0'; i++)
	{
		E_UART_FLAG _flag = Check();
		
		if (_flag != EU_SUCCE)
		{
			break;
		}
		
		while (!(UCSRA & (1<<UDRE)));
		
		UDR = _arg_bt_out_data[i];
	}
}

inline void C_BT::In(char _arg_bt_in_data[])
{
	usint i = 0;
	
	while (1)
	{
		CTS_LOW;
		
		E_UART_FLAG _flag = Check();
		
		if (_flag != EU_SUCCE)
		{
			CTS_HIGH;
			break;
		}
		
		_arg_bt_in_data[i] = UDR;
		
		CTS_HIGH;
		
		if ((_arg_bt_in_data[i] == '\n') && (i > 1))
		{
			_arg_bt_in_data[i + 1] = '\0';
			break;
		}
		
		i++;
	}
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

bool operator== (C_BT &_arg_bt, E_LOGIC _arg_bt_flag)
{
	if (_arg_bt._mem_bt_flag == _arg_bt_flag)	return true;
	
	return false;
}

bool operator!= (C_BT &_arg_bt, E_LOGIC _arg_bt_flag)
{
	if (_arg_bt._mem_bt_flag != _arg_bt_flag)	return true;
	
	return false;
}
#endif