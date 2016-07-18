
#ifndef _H28_C_ROBOBA_SLAVE_CPP_
#define _H28_C_ROBOBA_SLAVE_CPP_ 1

#include "H28_C_BT_304.cpp"

class C_BT_304_SLAVE : public C_BT_304
{
	protected:
	T_NUM _mem_bt_slave_flag_count;
	E_LOGIC _mem_bt_slave_falg :1;
	
	public:
	C_BT_304_SLAVE()	{}
	C_BT_304_SLAVE(E_UART_ADDR ,E_IO_PORT_ADDR, E_IO_NUM, E_IO_PORT_ADDR, E_IO_NUM ,E_IO_PORT_ADDR, E_IO_NUM);
	
	void Connect();
	
	void Connect(const char []);
	
	void Re_Connect();
	
	void In(char []);
	
	E_LOGIC Ret_flag();
	
	friend void operator>>(C_BT_304_SLAVE &,char []);
	
	friend bool operator==(C_BT_304_SLAVE &,E_LOGIC );
	friend bool operator!=(C_BT_304_SLAVE &,E_LOGIC );
};

C_BT_304_SLAVE::C_BT_304_SLAVE
(
	E_UART_ADDR		_arg_bt_slave_uart_addr,
	E_IO_PORT_ADDR	_arg_bt_slave_rts_addr,
	E_IO_NUM		_arg_bt_slave_rts_bit,
	E_IO_PORT_ADDR	_arg_bt_slave_cts_addr,
	E_IO_NUM		_arg_bt_slave_cts_bit,
	E_IO_PORT_ADDR	_arg_bt_slave_rse_addr,
	E_IO_NUM		_arg_bt_slave_rse_bit
)/*
見ての通りコンストラクタ。
継承の都合上、引数を持たない奴もあるから気を付けてね

	_arg_bt_slave_uart_addr	: SLAVEと接続するUARTのレジスタ
	_arg_bt_slave_rts_addr	: RTSピンのレジスタ
	_arg_bt_slave_rts_bit	: RTSピンのビット番号
	_arg_bt_slave_cts_addr	: CTSピンのレジスタ
	_arg_bt_slave_cts_bit	: CTSピンのビット番号
	_arg_bt_slave_rse_addr	: RESETピンのレジスタ
	_arg_bt_slave_rse_bit	: RESETピンのビット番号
	
*/: C_BT_304
(
	_arg_bt_slave_uart_addr,
	_arg_bt_slave_rts_addr,
	_arg_bt_slave_rts_bit,
	_arg_bt_slave_cts_addr,
	_arg_bt_slave_cts_bit,
	_arg_bt_slave_rse_addr,
	_arg_bt_slave_rse_bit
)
{
	_mem_bt_slave_flag_count = 0;
	_mem_bt_slave_falg = FALES;
}

E_LOGIC 
C_BT_304_SLAVE::
Ret_flag
()/*
接続しているかどうかのフラグ。

	TRUE  -> 生存
	FALES -> 死亡

*/{
	return _mem_bt_slave_falg;
}

void 
C_BT_304_SLAVE::
Connect
()/*
Masterと接続する。
あらかじめ入力されたアドレスをもとにしてるので入れといてね
*/{
	char CONNECTING[] = "\r\n+CONNECTING=123456789abc\r\n";
	char CONNECTED[]  = "\r\n+CONNECTED=123456789abc\r\n";
	
	for (usint i = 0; i < BT_ADDR_BYTE; i++)
	{
		CONNECTING[14 + i] = _mem_str_addr[i];
		CONNECTED [13 + i] = _mem_str_addr[i];
	}
	
	go_re_connect:
	
	_mem_bt << "AT+CONSLAVE=1,0\r\n";
	
	_mem_bt >> "\r\nACK\r\n";
	
	char _in_data[40] = {};
	
	do
	{
		_mem_bt >> _in_data;
		
		if (strcmp("\r\nTIMEOUT\r\n",_in_data) == 0)	goto go_re_connect;
		
	} while (strcmp(CONNECTING,_in_data) != 0);
	
	_mem_bt >> "\r\n+ACCEPT?\r\n";
	
	_mem_bt << "AT+RESCON=1\r\n";
	
	_mem_bt.In_comp(CONNECTED);
	
	_mem_bt >> "\r\nOK\r\n";
}

inline void 
C_BT_304_SLAVE::
Connect
(
	const char _arg_bt_slave_addr[BT_ADDR_BYTE]
)/*
アドレスを入力するタイプ。
入力したアドレスは保持しておくので新たに入れなおす必要性はないよ
	
	_arg_bt_slave_addr : アドレスを入力しといてください。それ以外は入れないでね。
	
*/{
	Set_bt_addr(_arg_bt_slave_addr);
	
	Connect();
}

void 
C_BT_304_SLAVE::
Re_Connect
()/*
再接続。
*/{
	_mem_bt.Reset();
	
	_mem_bt >> "\r\nOK\r\n";
	
	Connect();
}

void 
C_BT_304_SLAVE::
In
(
	char _arg_bt_slave_in_data[]
)/*
Masterからデータを受け取る。

	_arg_bt_slave_in_data : BT_DATA_NUM以上の要素数にしてね.

*/{
	char _in_data[30] = {}; //ここ30にしとかないとダメ 減らすな

	_mem_bt_slave_falg = FALES;

	_mem_bt >> _in_data;
	
	if (_mem_bt == TRUE)
	{
		_mem_bt_slave_flag_count = 0;
	}
	else
	{
		_mem_bt_slave_flag_count ++;
	}
	
	if  (	
			(strcmp("\r\n+LINK_LOST=000",_in_data) == 0)	|| 
			(strcmp("\r\n+DISCONNECTED=000",_in_data) == 0)	|| 
			(_mem_bt_slave_flag_count > 500)
		)
	{		
		_mem_bt_slave_flag_count = 0;
		
		return (void)0;
	}
	
	_mem_bt_slave_falg = TRUE;
	
	for (usint i = 0; i < BT_DATA_NUM; i++)
	{
		if (_in_data[i * 2 + 0] <= 0x39)
		{
			_arg_bt_slave_in_data[i] = ((_in_data[i * 2] & 0x0f) << 4);
		}
		else
		{
			_arg_bt_slave_in_data[i] = (((_in_data[i * 2] & 0x0f) + 9) << 4);
		}
		
		if (_in_data[i * 2 + 1] <= 0x39)
		{
			_arg_bt_slave_in_data[i] |= (_in_data[i * 2 + 1] & 0x0f);
		}
		else
		{
			_arg_bt_slave_in_data[i] |= ((_in_data[i * 2 + 1] & 0x0f) + 9);
		}
	}
}

void 
operator>>
(
	C_BT_304_SLAVE &_arg_bt_slave,
	char _arg_bt_slave_in_data[]
)/*
Masterからのデータを受け取る演算子。
機能的にはC_BT_304_SLVAE::In()まんま
	
	_arg_bt_slave_in_data : BT_DATA_NUM以上の要素数にしてね
	
*/{
	_arg_bt_slave.In(_arg_bt_slave_in_data);
}

bool 
operator==
(
	C_BT_304_SLAVE &_arg_bt_slave,
	E_LOGIC _arg_bt_slave_comp_flag
)/*
if文などで使うための演算子
Masterと接続しているかどうかの確認用

	TRUE  -> 生存
	FALES -> 死亡

*/{
	if (_arg_bt_slave._mem_bt == _arg_bt_slave_comp_flag)		return true;
	
	if (_arg_bt_slave._mem_bt_slave_falg == _arg_bt_slave_comp_flag)	return true;
	
	return false;
}

bool 
operator!=
(
	C_BT_304_SLAVE &_arg_bt_slave,
	E_LOGIC _arg_bt_slave_comp_flag
)/*
if文などで使うための演算子
Masterと接続しているかどうかの確認用

	TRUE  -> 生存
	FALES -> 死亡

*/{
	if (_arg_bt_slave._mem_bt != _arg_bt_slave_comp_flag)		return true;
	
	if (_arg_bt_slave._mem_bt_slave_falg != _arg_bt_slave_comp_flag)	return true;
	
	return false;
}
#endif