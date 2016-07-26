
#pragma once

#include "H28_C_BT_304.cpp"

class C_BT_304_MASTER : public C_BT_304
{
	public:
	C_BT_304_MASTER()	{}
	C_BT_304_MASTER(E_UART_ADDR ,E_IO_PORT_ADDR, E_IO_NUM, E_IO_PORT_ADDR, E_IO_NUM ,E_IO_PORT_ADDR ,E_IO_NUM);
	
	void Connect(const char []);
	void Connect();
	void Re_Connect();
	
	void Out(const char[]);

	friend void operator << (C_BT_304_MASTER &, const char []);
	
	friend bool operator == (C_BT_304_MASTER &,BOOL );
	friend bool operator != (C_BT_304_MASTER &,BOOL );
};

/************************************************************************/

C_BT_304_MASTER::C_BT_304_MASTER
(
	E_UART_ADDR		_arg_bt_master_uart_addr,
	E_IO_PORT_ADDR	_arg_bt_master_addr_rts, 
	E_IO_NUM		_arg_bt_master_bit_rts, 
	E_IO_PORT_ADDR	_arg_bt_master_addr_cts, 
	E_IO_NUM		_arg_bt_master_bit_cts,
	E_IO_PORT_ADDR	_arg_bt_master_addr_rse,
	E_IO_NUM		_arg_bt_master_bit_rse
)
/*
見ての通りコンストラクタ。
継承の都合上、引数を持たない奴もあるから気を付けてね

	_arg_bt_master_uart_addr : Bluetoothと接続するUARTのレジスタ
	_arg_bt_master_addr_rts	 : RTSピンのレジスタ
	_arg_bt_master_bit_rts	 : RTSピンのビット番号
	_arg_bt_master_addr_cts	 : CTSピンのレジスタ
	_arg_bt_master_bit_cts	 : CTSピンのビット番号
	_arg_bt_master_addr_rse	 : RESETピンのレジスタ
	_arg_bt_master_bit_rse	 : RESETピンのビット番号
*/
 : C_BT_304
(
	_arg_bt_master_uart_addr,
	_arg_bt_master_addr_rts,
	_arg_bt_master_bit_rts,
	_arg_bt_master_addr_cts,
	_arg_bt_master_bit_cts,
	_arg_bt_master_addr_rse,
	_arg_bt_master_bit_rse
)
{}

/************************************************************************/

void
C_BT_304_MASTER::
Connect()
/*
Slaveと接続する。
あらかじめ入力されたアドレスをもとにしてるので入れといてね
*/
{
	char CONMASTER[]  = "AT+CONMASTER=1,000190123456\r\n";
	char CONNECTED[]  = "\r\n+CONNECTED=000190123456\r\n";
	
	for (usint i = 0; i < BT_ADDR_BYTE; i++)
	{
		CONMASTER [15 + i] = _mem_bt_addr[i];
		CONNECTED [13 + i] = _mem_bt_addr[i];
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

/************************************************************************/

inline void 
C_BT_304_MASTER::
Connect(const char _arg_bt_master_addr[BT_ADDR_BYTE])
/*
アドレスを入力するタイプ。
入力したアドレスは保持しておくので新たに入れなおす必要性はないよ
	
	_arg_bt_master_addr : アドレスを入力しといてください。それ以外は入れないでね。
*/
{
	Set_bt_addr(_arg_bt_master_addr);
	
	Connect();
}

/************************************************************************/

void 
C_BT_304_MASTER::
Re_Connect()
/*
再接続
*/
{	
	_mem_bt.Reset();
	
	_mem_bt >> "\r\nOK\r\n";
	
	Connect();
}

/************************************************************************/

void 
C_BT_304_MASTER::
Out(const char _arg_bt_master_out_data[BT_DATA_NUM])
/*
コントローラからのデータをSlaveに送信する。

	_arg_bt_master_out_data : 送信するデータ.要素数はBT_DATA_NUMで
*/
{
	char _out_data[20] = {};
	
	for (usint i = 0; i < BT_DATA_NUM; i++)
	{
		if (((_arg_bt_master_out_data[i]) & 0xf0) < 0xa0)
		{
			_out_data[i * 2 + 0]  = 0x30;
			
			_out_data[i * 2 + 0] += ((_arg_bt_master_out_data[i] & 0xf0) >> 4);
		}
		else
		{
			_out_data[i * 2 + 0]  = 0x40;
			
			_out_data[i * 2 + 0] += ((_arg_bt_master_out_data[i] & 0xf0) >> 4);
			
			_out_data[i * 2 + 0] -= 9;
		}
		
		if (((_arg_bt_master_out_data[i]) & 0x0f) < 0x0a)
		{
			_out_data[i * 2 + 1]  = 0x30;
			
			_out_data[i * 2 + 1] += (_arg_bt_master_out_data[i] & 0x0f);
		}
		else
		{
			_out_data[i * 2 + 1]  = 0x40;
			
			_out_data[i * 2 + 1] += (_arg_bt_master_out_data[i] & 0x0f);
			
			_out_data[i * 2 + 1] -= 9;
		}
	}
	
	_out_data[BT_DATA_NUM * 2 + 0] = '\r';
	_out_data[BT_DATA_NUM * 2 + 1] = '\n';
	_out_data[BT_DATA_NUM * 2 + 2] = '\0';
	
	_mem_bt << _out_data;
}

/************************************************************************/

void 
operator << 
(
	C_BT_304_MASTER &_arg_bt_master, 
	const char _arg_bt_master_out_data[]
)
/*
コントローラからのデータをSlaveに送信する演算子。
ぶっちゃけC_BT_304_MASTER::Out()とやってることは変わりません。

	_arg_bt_master_out_data : 送信するデータ.要素数はBT_DATA_NUM以上で
*/
{
	_arg_bt_master.Out(_arg_bt_master_out_data);
}

/************************************************************************/

bool 
operator == 
(
	C_BT_304_MASTER &_arg_bt_master, 
	BOOL _arg_bt_master_flag_comp
)
/*
if文などで使うための演算子
Slaveと接続しているかどうかの確認用

	TRUE  -> 生存
	FALES -> 死亡
*/
{
	if (_arg_bt_master._mem_bt == _arg_bt_master_flag_comp)	return true;
	
	return false;
}

/************************************************************************/

bool 
operator != 
(
	C_BT_304_MASTER &_arg_bt_master, 
	BOOL _arg_bt_master_flag_comp
)
/*
if文などで使うための演算子
Slaveと接続しているかどうかの確認用

	TRUE  -> 生存
	FALES -> 死亡
*/
{
	if (_arg_bt_master._mem_bt != _arg_bt_master_flag_comp)	return true;
	
	return false;
}