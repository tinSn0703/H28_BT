
#ifndef _H28_C_ROBOBA_MASTER_CPP_
#define _H28_C_ROBOBA_MASTER_CPP_ 1

#include "H28_C_ROBOBA.cpp"

class C_ROBOBA_MASTER : public C_ROBOBA
{
	public:
	C_ROBOBA_MASTER()	{}
	C_ROBOBA_MASTER(E_UART_ADDR ,E_IO_PORT_ADDR, E_IO_NUM, E_IO_PORT_ADDR, E_IO_NUM );
	
	void Connect(const char []);
	
	void Connect();
	
	void Out(const char[]);
	
//	void Re_Connect();
};

C_ROBOBA_MASTER::C_ROBOBA_MASTER(E_UART_ADDR _arg_uart_addr,E_IO_PORT_ADDR _arg_rts_addr, E_IO_NUM _arg_rts_bit, E_IO_PORT_ADDR _arg_cts_addr, E_IO_NUM _arg_cts_bit)
: C_ROBOBA(_arg_uart_addr,_arg_rts_addr,_arg_rts_bit,_arg_cts_addr,_arg_cts_bit)
{}

void C_ROBOBA_MASTER::Connect()
{
	char CONMASTER[]  = "AT+CONMASTER=1,000190123456\r\n";
	char CONNECTED[]  = "\r\n+CONNECTED=000190123456\r\n";
	
	for (usint i = 0; i < BT_ADDR_BYTE; i++)
	{
		CONMASTER [15 + i] = _mem_str_addr[i];
		CONNECTED [13 + i] = _mem_str_addr[i];
	}
	
	_mem_bt << CONMASTER;
	
	_mem_bt == "\r\nACK\r\n";
	
	_mem_bt == CONNECTED;
	
	_mem_bt == "\r\nOK\r\n";
}

inline void C_ROBOBA_MASTER::Connect(const char _arg_str_addr[])
{
	Set_bt_addr(_arg_str_addr);
	
	Connect();
}

void C_ROBOBA_MASTER::Out(const char _arg_out_data[])
{
	char _out_data[20] = {};
	
	for (usint i = 0; i < BT_DATA_NUM; i++)
	{
		if (((_arg_out_data[i]) & 0xf0) < 0xa0)
		{
			_out_data[i * 2]  = 0x30;
			
			_out_data[i * 2] += ((_arg_out_data[i] & 0xf0) >> 4);
		}
		else
		{
			_out_data[i * 2]  = 0x40;
			
			_out_data[i * 2] += ((_arg_out_data[i] & 0xf0) >> 4);
			
			_out_data[i * 2] -= 9;
		}
		
		if (((_arg_out_data[i]) & 0x0f) < 0x0a)
		{
			_out_data[(i * 2) + 1]  = 0x30;
			
			_out_data[(i * 2) + 1] += (_arg_out_data[i] & 0x0f);
		}
		else
		{
			_out_data[(i * 2) + 1]  = 0x40;
			
			_out_data[(i * 2) + 1] += (_arg_out_data[i] & 0x0f);
			
			_out_data[(i * 2) + 1] -= 9;
		}
	}
	
	_out_data[BT_DATA_NUM * 2] = '\r';
	_out_data[BT_DATA_NUM * 2 + 1] = '\n';
	_out_data[BT_DATA_NUM * 2 + 2] = '\0';
	
	_mem_bt << _out_data;
}

#endif