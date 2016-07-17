
#ifndef _H28_C_ROBOBA_MASTER_CPP_
#define _H28_C_ROBOBA_MASTER_CPP_ 1

#include "H28_C_BT_304.cpp"

class C_BT_304_MASTER : public C_BT_304
{
	public:
	C_BT_304_MASTER()	{}
	C_BT_304_MASTER(E_UART_ADDR ,E_IO_PORT_ADDR, E_IO_NUM, E_IO_PORT_ADDR, E_IO_NUM ,E_IO_PORT_ADDR ,E_IO_NUM);
	
	void Connect(const char []);
	
	void Connect();
	
	void Out(const char[]);
	
	void Re_Connect();

	E_LOGIC Flag();

	friend void operator<< (C_BT_304_MASTER &, const char []);
	
	friend bool operator== (C_BT_304_MASTER &,E_LOGIC );
	friend bool operator!= (C_BT_304_MASTER &,E_LOGIC );
};

C_BT_304_MASTER::C_BT_304_MASTER
(
	E_UART_ADDR _arg_bt_uart_addr,
	E_IO_PORT_ADDR _arg_bt_rts_addr, 
	E_IO_NUM _arg_bt_rts_bit, 
	E_IO_PORT_ADDR _arg_bt_cts_addr, 
	E_IO_NUM _arg_bt_cts_bit,
	E_IO_PORT_ADDR _arg_bt_rse_addr,
	E_IO_NUM _arg_bt_rse_bit
)
: C_BT_304
(
	_arg_bt_uart_addr,
	_arg_bt_rts_addr,
	_arg_bt_rts_bit,
	_arg_bt_cts_addr,
	_arg_bt_cts_bit,
	_arg_bt_rse_addr,
	_arg_bt_rse_bit
)
{}

void C_BT_304_MASTER::Connect()
{
	char CONMASTER[]  = "AT+CONMASTER=1,000190123456\r\n";
	char CONNECTED[]  = "\r\n+CONNECTED=000190123456\r\n";
	
	for (usint i = 0; i < BT_ADDR_BYTE; i++)
	{
		CONMASTER [15 + i] = _mem_str_addr[i];
		CONNECTED [13 + i] = _mem_str_addr[i];
	}
	
	re_connect:
	
	_mem_bt << CONMASTER;
	
	_mem_bt >> "\r\nACK\r\n";
	
	char _in_data[40] = {};
		
	do
	{
		_mem_bt >> _in_data;
		
		if (strcmp("\r\nTIMEOUT\r\n",_in_data) == 0)	goto re_connect;
		
	} while (strcmp(CONNECTED,_in_data) != 0);
	
	_mem_bt >> "\r\nOK\r\n";
}

inline void C_BT_304_MASTER::Connect(const char _arg_bt_addr[])
{
	Set_bt_addr(_arg_bt_addr);
	
	Connect();
}

void C_BT_304_MASTER::Re_Connect()
{
	_mem_bt >> "\r\n+LINK_LOST=";
		
	_mem_bt << "AT+RESET\r\n";
	
	_mem_bt >> "\r\nACK\r\n";
	
	_mem_bt >> "\r\nOK\r\n";
	
	Connect();
}

void C_BT_304_MASTER::Out(const char _arg_bt_out_data[])
{
	char _out_data[20] = {};
	
	for (usint i = 0; i < BT_DATA_NUM; i++)
	{
		if (((_arg_bt_out_data[i]) & 0xf0) < 0xa0)
		{
			_out_data[i * 2 + 0]  = 0x30;
			
			_out_data[i * 2 + 0] += ((_arg_bt_out_data[i] & 0xf0) >> 4);
		}
		else
		{
			_out_data[i * 2 + 0]  = 0x40;
			
			_out_data[i * 2 + 0] += ((_arg_bt_out_data[i] & 0xf0) >> 4);
			
			_out_data[i * 2 + 0] -= 9;
		}
		
		if (((_arg_bt_out_data[i]) & 0x0f) < 0x0a)
		{
			_out_data[i * 2 + 1]  = 0x30;
			
			_out_data[i * 2 + 1] += (_arg_bt_out_data[i] & 0x0f);
		}
		else
		{
			_out_data[i * 2 + 1]  = 0x40;
			
			_out_data[i * 2 + 1] += (_arg_bt_out_data[i] & 0x0f);
			
			_out_data[i * 2 + 1] -= 9;
		}
	}
	
	_out_data[BT_DATA_NUM * 2 + 0] = '\r';
	_out_data[BT_DATA_NUM * 2 + 1] = '\n';
	_out_data[BT_DATA_NUM * 2 + 2] = '\0';
	
	_mem_bt << _out_data;
}

void operator<< (C_BT_304_MASTER &_arg_bt_master, const char _arg_bt_out_data[BT_DATA_NUM])
{
	_arg_bt_master.Out(_arg_bt_out_data);
}

bool operator== (C_BT_304_MASTER &_arg_bt_master, E_LOGIC _arg_bt_flag_rse)
/*
通信が動作しているかどうかの確認
	TRUE  -> 動作
	FALES -> 切断
*/
{
	if (_arg_bt_master._mem_bt == _arg_bt_flag_rse)	return true;
	
	return false;
}

bool operator!= (C_BT_304_MASTER &_arg_bt_master, E_LOGIC _arg_bt_flag_rse)
/*
通信が動作しているかどうかの確認
	TRUE  -> 動作
	FALES -> 切断
*/
{
	if (_arg_bt_master._mem_bt != _arg_bt_flag_rse)	return true;
	
	return false;
}
#endif