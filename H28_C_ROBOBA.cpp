
#ifndef _H28_C_ROBOBA_CPP_
#define _H28_C_ROBOBA_CPP_ 1

class C_ROBOBA
{
	private:
	E_UART_MODE _mem_bt_mode :1;
	E_LOGIC _mem_nf_link :1;
	char _mem_str_addr[12];
	C_BT _mem_bt;
	
	public:
	C_ROBOBA()	{}
	C_ROBOBA(E_UART_MODE ,E_UART_ADDR ,E_IO_PORT_ADDR, E_IO_NUM, E_IO_PORT_ADDR, E_IO_NUM );
	
	E_LOGIC Ret_link()	{	return _mem_nf_link;	}
	
	void SET_ADDR(const char []);
	void GET_ADDR(char []);
	
	void CONNECT_MASTER(const char []);
	void CONNECT_SLAVE (const char []);
	
	void CONNECT_MASTER();
	void CONNECT_SLAVE ();
	
	//	void RE_CONNECT_MASTER();
	//	void RE_CONNECT_SLAVE ();
	
	friend void operator<< (C_ROBOBA &,const char []);
	
	friend void operator>> (C_ROBOBA &,char[]);
	friend void operator>> (C_ROBOBA &,const char[]);
};

C_ROBOBA::C_ROBOBA(E_UART_MODE _arg_bt_mode,E_UART_ADDR _arg_uart_addr,E_IO_PORT_ADDR _arg_rts_addr, E_IO_NUM _arg_rts_bit, E_IO_PORT_ADDR _arg_cts_addr, E_IO_NUM _arg_cts_bit)
: _mem_bt(_arg_uart_addr,_arg_rts_addr,_arg_rts_bit,_arg_cts_addr,_arg_cts_bit)
{
	_mem_bt_mode = _arg_bt_mode;
	
	_mem_nf_link = FALES;
	
	_mem_bt >> "\r\nOK\r\n";
}

inline void C_ROBOBA::SET_ADDR(const char _arg_str_addr[])
{
	for (usint i = 0; i < 12; i++)
	{
		_mem_str_addr[i] = _arg_str_addr[i];
	}
}

void C_ROBOBA::GET_ADDR(char _arg_str_addr[])
{
	_mem_bt << "AT+GETADDR\r\n";
	
	char _in_data[27] = {};
	
	do
	{
		_mem_bt >> _in_data;
		
	} while (strcmp("\r\nBDADDR=",_in_data));
	
	_mem_bt >> "\r\nOK\r\n";
	
	for (uint i = 0; i < 12; i++)
	{
		_arg_str_addr[i] = _in_data[9 + i];
	}
	
	_arg_str_addr[12] = '\0';
}

void C_ROBOBA::CONNECT_MASTER()
{
	if (_mem_bt_mode != EU_TRA)	return (void)NULL;
	
	char CONMASTER[]  = "AT+CONMASTER=1,000190123456\r\n";
	char CONNECTED[]  = "\r\n+CONNECTED=000190123456\r\n";
	
	for (usint i = 0; i < 12; i++)
	{
		CONMASTER [15 + i] = _mem_str_addr[i];
		CONNECTED [13 + i] = _mem_str_addr[i];
	}
	
	_mem_bt << CONMASTER;
	
	_mem_bt >> "\r\nACK\r\n";
	
	_mem_bt == CONNECTED;
	
	_mem_bt >> "\r\nOK\r\n";
	
	_mem_nf_link = TRUE;
}

void C_ROBOBA::CONNECT_SLAVE()
{
	if (_mem_bt_mode != EU_REC)	return (void)NULL;
	
	char CONNECTING[] = "\r\n+CONNECTING=123456789abc\r\n";
	char CONNECTED[]  = "\r\n+CONNECTED=123456789abc\r\n";
	
	for (usint i = 0; i < 12; i++)
	{
		CONNECTING[14 + i] = _mem_str_addr[i];
		CONNECTED [13 + i] = _mem_str_addr[i];
	}
	
	_mem_bt << "AT+CONSLAVE=1,0\r\n";
	
	_mem_bt >> "\r\nACK\r\n";
	
	_mem_bt == CONNECTING;
	
	_mem_bt >> "\r\n+ACCEPT?\r\n";
	
	_mem_bt << "AT+RESCON=1\r\n";
	
	_mem_bt == CONNECTED;
	
	_mem_bt >> "\r\nOK\r\n";
	
	_mem_nf_link = TRUE;
}

inline void C_ROBOBA::CONNECT_MASTER(const char _arg_str_addr[])
{
	SET_ADDR(_arg_str_addr);
	
	CONNECT_MASTER();
}

inline void C_ROBOBA::CONNECT_SLAVE(const char _arg_str_addr[])
{
	SET_ADDR(_arg_str_addr);
	
	CONNECT_SLAVE();
}

void operator<< (C_ROBOBA &_arg_bt, const char _arg_out_data[])
{
	_arg_bt._mem_bt << _arg_out_data;
}

void operator>> (C_ROBOBA &_arg_bt, char _arg_in_data[])
{
	_arg_bt._mem_bt >> _arg_in_data;
}

void operator>> (C_ROBOBA &_arg_bt, const char _arg_in_data[])
{
	_arg_bt._mem_bt >> _arg_in_data;
}

#endif