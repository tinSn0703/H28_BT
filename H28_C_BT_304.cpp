
#ifndef _H28_C_ROBOBA_CPP_
#define _H28_C_ROBOBA_CPP_ 1

#include "H28_C_BT.cpp"

class C_BT_304
{
	protected:
	char _mem_str_addr[BT_ADDR_BYTE];
	
	public:
	C_BT _mem_bt;
	
	C_BT_304()	{}
	C_BT_304(E_UART_ADDR ,E_IO_PORT_ADDR, E_IO_NUM, E_IO_PORT_ADDR, E_IO_NUM , E_IO_PORT_ADDR ,E_IO_NUM);
	
	void Set_bt_addr(const char []);
	void Get_bt_addr(char []);
};

C_BT_304::C_BT_304
(
	E_UART_ADDR _arg_uart_addr,
	E_IO_PORT_ADDR _arg_rts_addr, 
	E_IO_NUM _arg_rts_bit, 
	E_IO_PORT_ADDR _arg_cts_addr,
	E_IO_NUM _arg_cts_bit,
	E_IO_PORT_ADDR _arg_bt_rse_addr,
	E_IO_NUM _arg_bt_rse_bit
)
: _mem_bt
(
	_arg_uart_addr,
	_arg_rts_addr,
	_arg_rts_bit,
	_arg_cts_addr,
	_arg_cts_bit,
	_arg_bt_rse_addr,
	_arg_bt_rse_bit
)
{
	_mem_bt >> "\r\nOK\r\n";
}

inline void C_BT_304::Set_bt_addr(const char _arg_str_addr[])
{
	for (usint i = 0; i < BT_ADDR_BYTE; i++)
	{
		_mem_str_addr[i] = _arg_str_addr[i];
	}
}

inline void C_BT_304::Get_bt_addr(char _arg_str_addr[])
{
	_mem_bt << "AT+GETADDR\r\n";
	
	char _in_data[27] = {};
	
	do
	{
		_mem_bt >> _in_data;
		
	} while (strcmp("\r\nBDADDR=",_in_data));
	
	_mem_bt >> "\r\nOK\r\n";
	
	for (uint i = 0; i < BT_ADDR_BYTE; i++)
	{
		_arg_str_addr[i] = _in_data[9 + i];
	}
	
	_arg_str_addr[BT_ADDR_BYTE] = '\0';
}

#endif