
#pragma once

#include "H28_C_BT_304.cpp"

/**
 * BluetoothをMasterで扱うクラス
 */
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

/**
 * \brief コンストラクタ
 * 
 * \param _arg_bt_master_uart_addr : Bluetoothと接続するUARTのレジスタ
 * \param _arg_bt_master_addr_rts  : RTSピンのレジスタ
 * \param _arg_bt_master_bit_rts   : RTSピンのビット
 * \param _arg_bt_master_addr_cts  : CTSピンのレジスタ
 * \param _arg_bt_master_bit_cts   : CTSピンのビット
 * \param _arg_bt_master_addr_rse  : RESETピンのレジスタ
 * \param _arg_bt_master_bit_rse   : RESETピンのビット
 */
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

/**
 * \brief Masterと接続する。アドレスはあらかじめ入力されたものを使う
 */
void
C_BT_304_MASTER::
Connect()
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

/**
 * \brief Masterと接続する。アドレスを入力するタイプ。
 * 
 * \param _arg_bt_slave_addr : アドレス。チェック系はないので
 */
inline void 
C_BT_304_MASTER::
Connect(const char _arg_bt_master_addr[BT_ADDR_BYTE])
{
	Set_bt_addr(_arg_bt_master_addr);
	
	Connect();
}

/**
 * \brief 再接続。
 */
void 
C_BT_304_MASTER::
Re_Connect()
{	
	_mem_bt.Reset();
	
	_mem_bt >> "\r\nOK\r\n";
	
	Connect();
}

/**
 * \brief 
 * Slaveへ変換してから送る
 * 具体的には数値を文字列に変換する
 * 
 * \param _arg_bt_master_out_data : 送信するデータ
 */
void 
C_BT_304_MASTER::
Out(const char _arg_bt_master_out_data[BT_DATA_NUM])
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

/**
 * \brief 
 * Slaveへデータを送信する。
 * 機能的にはC_BT_304_MASTER::Out()と同じ
 * 
 * \param _arg_bt_master
 * \param _arg_bt_master_out_data : 送信するデータ
 */
void 
operator << 
(
	C_BT_304_MASTER &_arg_bt_master, 
	const char _arg_bt_master_out_data[]
)
{
	_arg_bt_master.Out(_arg_bt_master_out_data);
}

/**
 * \brief 
 * Slaveと通信しているかを確認する用の演算子
 * TRUEを表すとき接続。FALESの場合切断。
 * 
 * \param _arg_bt_master
 * \param _arg_bt_master_flag_comp : 比較
 * 
 * \return bool 
 * _arg_bt_masterと_arg_bt_master_flag_compが等しいときtrue
 */
bool 
operator == 
(
	C_BT_304_MASTER &_arg_bt_master, 
	BOOL _arg_bt_master_flag_comp
)
{
	if (_arg_bt_master._mem_bt == _arg_bt_master_flag_comp)	return true;
	
	return false;
}

/**
 * \brief 
 * Slaveと通信しているかを確認する用の演算子
 * TRUEを表すとき接続。FALESの場合切断。
 * 
 * \param _arg_bt_master
 * \param _arg_bt_master_flag_comp : 比較
 * 
 * \return bool 
 * _arg_bt_masterと_arg_bt_master_flag_compが等しくないときtrue
 */
bool 
operator != 
(
	C_BT_304_MASTER &_arg_bt_master, 
	BOOL _arg_bt_master_flag_comp
)
{
	if (_arg_bt_master._mem_bt != _arg_bt_master_flag_comp)	return true;
	
	return false;
}