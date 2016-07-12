
#ifndef _H28_C_ROBOBA_SLAVE_CPP_
#define _H28_C_ROBOBA_SLAVE_CPP_ 1

#include "H28_C_ROBOBA.cpp"

class C_ROBOBA_SLAVE : public C_ROBOBA
{
	protected:
	E_LOGIC _mem_bt_falg :1;
	
	public:
	C_ROBOBA_SLAVE()	{}
	C_ROBOBA_SLAVE(E_UART_ADDR ,E_IO_PORT_ADDR, E_IO_NUM, E_IO_PORT_ADDR, E_IO_NUM );
	
	void Connect(const char []);
	
	void Connect();
	
	E_LOGIC In(char []);
	
	E_LOGIC Ret_flag()	{	return _mem_bt_falg;	}
	
	void Re_Connect();
	
	friend E_LOGIC operator>>(C_ROBOBA_SLAVE &,char []);
	
	friend bool operator==(C_ROBOBA_SLAVE &,E_LOGIC );
	friend bool operator!=(C_ROBOBA_SLAVE &,E_LOGIC );
};

C_ROBOBA_SLAVE::C_ROBOBA_SLAVE(E_UART_ADDR _arg_bt_uart_addr,E_IO_PORT_ADDR _arg_bt_rts_addr, E_IO_NUM _arg_bt_rts_bit, E_IO_PORT_ADDR _arg_bt_cts_addr, E_IO_NUM _arg_bt_cts_bit)
: C_ROBOBA(_arg_bt_uart_addr,_arg_bt_rts_addr,_arg_bt_rts_bit,_arg_bt_cts_addr,_arg_bt_cts_bit)
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
	
	re_connect:
	
	_mem_bt << "AT+CONSLAVE=1,0\r\n";
	
	_mem_bt >> "\r\nACK\r\n";
	
	char _in_data[40] = {};
	
	do
	{
		_mem_bt >> _in_data;
		
		if (strcmp("\r\nTIMEOUT\r\n",_in_data) == 0)	goto re_connect;
		
	} while (strcmp(CONNECTING,_in_data) != 0);
	
	_mem_bt >> "\r\n+ACCEPT?\r\n";
	
	_mem_bt << "AT+RESCON=1\r\n";
	
	_mem_bt.In_comp(CONNECTED);
	
	_mem_bt >> "\r\nOK\r\n";
}

inline void C_ROBOBA_SLAVE::Connect(const char _arg_bt_addr[])
{
	Set_bt_addr(_arg_bt_addr);
	
	Connect();
}

void C_ROBOBA_SLAVE::Re_Connect()
{
	_mem_bt << "AT+RESET\r\n";
	
	_mem_bt >> "\r\nACK\r\n";
	
	_mem_bt >> "\r\nOK\r\n";
	
	Connect();
}

E_LOGIC C_ROBOBA_SLAVE::In(char _arg_bt_in_data[])
{
	char _in_data[25] = {};
	
	_mem_bt >> _in_data;

	if ((strcmp("\r\n+DISCONNECTED=",_in_data) | strcmp("\r\n+LINK_LOST=",_in_data)) == 0)
	{
		_mem_bt_falg = FALES;
		
		return FALES;
	}
	
	for (usint i = 0; i < BT_DATA_NUM; i++)
	{
		_arg_bt_in_data[i] = 0x00;
		
		if (_in_data[i * 2] <= 0x39)
		{
			_arg_bt_in_data[i] |= ((_in_data[i * 2] & 0x0f) << 4);
		}
		else
		{
			_arg_bt_in_data[i] |= (((_in_data[i * 2] & 0x0f) + 9) << 4);
		}
		
		if (_in_data[i * 2 + 1] <= 0x39)
		{
			_arg_bt_in_data[i] |= (_in_data[i * 2 + 1] & 0x0f);
		}
		else
		{
			_arg_bt_in_data[i] |= ((_in_data[i * 2 + 1] & 0x0f) + 9);
		}
	}
	
	_mem_bt_falg = TRUE;
	
	return TRUE;
}

E_LOGIC operator>>(C_ROBOBA_SLAVE &_arg_bt_slave,char _arg_bt_in_data[])
{
	return _arg_bt_slave.In(_arg_bt_in_data);
}

bool operator==(C_ROBOBA_SLAVE &_arg_bt_slave,E_LOGIC _arg_bt_com_flag)
{
	if (_arg_bt_slave._mem_bt_falg == _arg_bt_com_flag)	return true;
	
	return false;
}

bool operator!=(C_ROBOBA_SLAVE &_arg_bt_slave,E_LOGIC _arg_bt_com_flag)
{
	if (_arg_bt_slave._mem_bt_falg != _arg_bt_com_flag)	return true;
	
	return false;
}
#endif