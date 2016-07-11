
#ifndef _H28_C_ROBOBA_SLAVE_CPP_
#define _H28_C_ROBOBA_SLAVE_CPP_ 1

#include "H28_C_ROBOBA.cpp"

class C_ROBOBA_SLAVE : public C_ROBOBA
{
	public:
	C_ROBOBA_SLAVE()	{}
	C_ROBOBA_SLAVE(E_UART_ADDR ,E_IO_PORT_ADDR, E_IO_NUM, E_IO_PORT_ADDR, E_IO_NUM );
	
	void Connect(const char []);
	
	void Connect();
	
	E_LOGIC In(char []);
	
	//void Re_Connect();
	
	friend E_LOGIC operator>>(C_ROBOBA_SLAVE &,char []);
};

C_ROBOBA_SLAVE::C_ROBOBA_SLAVE(E_UART_ADDR _arg_uart_addr,E_IO_PORT_ADDR _arg_rts_addr, E_IO_NUM _arg_rts_bit, E_IO_PORT_ADDR _arg_cts_addr, E_IO_NUM _arg_cts_bit)
: C_ROBOBA(_arg_uart_addr,_arg_rts_addr,_arg_rts_bit,_arg_cts_addr,_arg_cts_bit)
{}

void C_ROBOBA_SLAVE::Connect()
{
	char CONNECTING[] = "\r\n+CONNECTING=123456789abc\r\n";
	char CONNECTED[]  = "\r\n+CONNECTED=123456789abc\r\n";
	
	for (usint i = 0; i < BT_ADDR_BYTE; i++)
	{
		CONNECTING[14 + i] = _mem_str_addr[i];
		CONNECTED [13 + i] = _mem_str_addr[i];
	}
	
	_mem_bt << "AT+CONSLAVE=1,0\r\n";
	
	_mem_bt == "\r\nACK\r\n";
	
	_mem_bt == CONNECTING;
	
	_mem_bt == "\r\n+ACCEPT?\r\n";
	
	
	_mem_bt << "AT+RESCON=1\r\n";
	
	_mem_bt == CONNECTED;
	
	_mem_bt == "\r\nOK\r\n";
}

inline void C_ROBOBA_SLAVE::Connect(const char _arg_str_addr[])
{
	Set_bt_addr(_arg_str_addr);
	
	Connect();
}

E_LOGIC C_ROBOBA_SLAVE::In(char _arg_in_data[])
{
	char _in_data[25] = {};
	
	_mem_bt >> _in_data;

	if ((strcmp("\r\n+DISCONNECTED=",_in_data) | strcmp("\r\n+LINK_LOST=",_in_data)) == 0)
	{
		return FALES;
	}
	
	for (usint i = 0; i < BT_DATA_NUM; i++)
	{
		_arg_in_data[i] = 0x00;
		
		if (_in_data[i * 2] <= 0x39)
		{
			_arg_in_data[i] |= ((_in_data[i * 2] & 0x0f) << 4);
		}
		else
		{
			_arg_in_data[i] |= (((_in_data[i * 2] & 0x0f) + 9) << 4);
		}
		
		if (_in_data[i * 2 + 1] <= 0x39)
		{
			_arg_in_data[i] |= (_in_data[i * 2 + 1] & 0x0f);
		}
		else
		{
			_arg_in_data[i] |= ((_in_data[i * 2 + 1] & 0x0f) + 9);
		}
	}
	
	return TRUE;
}

E_LOGIC operator>>(C_ROBOBA_SLAVE &_arg_bt_slave,char _arg_in_data[])
{
	return _arg_bt_slave.In(_arg_in_data);
}

#endif