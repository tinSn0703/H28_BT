
#pragma once

#include "H28_C_BT_304.cpp"

/**
 * BluetoothをSlaveで使うクラス。
 */
class C_BT_304_SLAVE : public C_BT_304
{
protected:
	
	T_NUM _mem_bt_slave_flag_count;
	BOOL _mem_bt_slave_falg :1;
	
public:
	
	C_BT_304_SLAVE()	{}
	C_BT_304_SLAVE(E_UART_ADDR ,E_IO_PORT_ADDR, E_IO_NUM, E_IO_PORT_ADDR, E_IO_NUM ,E_IO_PORT_ADDR, E_IO_NUM);
	
	void Connect();
	
	void Connect(const char []);
	
	void Re_Connect();
	
	void In(char []);
	
	friend void operator >> (C_BT_304_SLAVE &,char []);
	
	friend bool operator == (C_BT_304_SLAVE &,BOOL );
	friend bool operator != (C_BT_304_SLAVE &,BOOL );
};

/**
 * \brief コンストラクタ
 * 
 * \param _arg_bt_slave_uart_addr : Bluetoothと接続するUARTのレジスタ
 * \param _arg_bt_slave_addr_rts  : RTSピンのレジスタ
 * \param _arg_bt_slave_bit_rts	  : RTSピンのビット
 * \param _arg_bt_slave_addr_cts  : CTSピンのレジスタ
 * \param _arg_bt_slave_bit_cts   : CTSピンのビット
 * \param _arg_bt_slave_addr_rse  : RESETピンのレジスタ
 * \param _arg_bt_slave_bit_rse   : RESETピンのビット
 */
C_BT_304_SLAVE::
C_BT_304_SLAVE
(
	E_UART_ADDR		_arg_bt_slave_uart_addr,
	E_IO_PORT_ADDR	_arg_bt_slave_addr_rts,
	E_IO_NUM		_arg_bt_slave_bit_rts,
	E_IO_PORT_ADDR	_arg_bt_slave_addr_cts,
	E_IO_NUM		_arg_bt_slave_bit_cts,
	E_IO_PORT_ADDR	_arg_bt_slave_addr_rse,
	E_IO_NUM		_arg_bt_slave_bit_rse
)
 : C_BT_304
(
	_arg_bt_slave_uart_addr,
	_arg_bt_slave_addr_rts,
	_arg_bt_slave_bit_rts,
	_arg_bt_slave_addr_cts,
	_arg_bt_slave_bit_cts,
	_arg_bt_slave_addr_rse,
	_arg_bt_slave_bit_rse
)
{
	_mem_bt_slave_flag_count = 0;
	_mem_bt_slave_falg = FALES;
}

/**
 * \brief Masterと接続する。アドレスはあらかじめ入力されたものを使う
 */
void 
C_BT_304_SLAVE::
Connect()
{
	char CONNECTING[] = "\r\n+CONNECTING=123456789abc\r\n";
	char CONNECTED[]  = "\r\n+CONNECTED=123456789abc\r\n";
	
	for (usint i = 0; i < BT_ADDR_BYTE; i++)
	{
		CONNECTING[14 + i] = _mem_bt_addr[i];
		CONNECTED [13 + i] = _mem_bt_addr[i];
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

/**
 * \brief Masterと接続する。アドレスを入力するタイプ。
 * 
 * \param _arg_bt_slave_addr : アドレス。チェック系はないので
 */
inline void 
C_BT_304_SLAVE::
Connect (const char _arg_bt_slave_addr[BT_ADDR_BYTE])
{
	Set_bt_addr(_arg_bt_slave_addr);
	
	Connect();
}

/**
 * \brief 再接続。
 */
void 
C_BT_304_SLAVE::
Re_Connect()
{
	_mem_bt.Reset();
	
	_mem_bt >> "\r\nOK\r\n";
	
	Connect();
}

/**
 * \brief 
 * Masterからのデータを変換してから受け取る。
 * 具体的には文字列に変換されたデータを数値に戻す。
 * 
 * \param _arg_re_bt_slave_in_data 
 *		受信したデータの格納される場所。
 *		要BT_DATA_NUM以上の要素数
 */
void 
C_BT_304_SLAVE::
In (char _arg_re_bt_slave_in_data[])
{
	char _in_data[30] = {};

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
			(_mem_bt_slave_flag_count > 200)
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
			_arg_re_bt_slave_in_data[i] = ((_in_data[i * 2] & 0x0f) << 4);
		}
		else
		{
			_arg_re_bt_slave_in_data[i] = (((_in_data[i * 2] & 0x0f) + 9) << 4);
		}
		
		if (_in_data[i * 2 + 1] <= 0x39)
		{
			_arg_re_bt_slave_in_data[i] |= (_in_data[i * 2 + 1] & 0x0f);
		}
		else
		{
			_arg_re_bt_slave_in_data[i] |= ((_in_data[i * 2 + 1] & 0x0f) + 9);
		}
	}
}

/**
 * \brief 
 * Masterからのデータを受け取る演算子
 * 機能的にはC_BT_304_SLAVE::In()と同じ
 * 
 * \param _arg_bt_slave : みたまま
 * \param _arg_re_bt_slave_in_data : データが格納される変数
 */
void 
operator >>
(
	C_BT_304_SLAVE &_arg_bt_slave,
	char _arg_re_bt_slave_in_data[]
)
{
	_arg_bt_slave.In(_arg_re_bt_slave_in_data);
}

/**
 * \brief 
 * Masterと通信しているかを確認する用の演算子
 * TRUEを表すとき接続。FALESの場合切断。
 * 
 * \param _arg_bt_slave
 * \param _arg_bt_slave_flag_comp : 比較
 * 
 * \return bool 
 * _arg_bt_slaveと_arg_bt_slave_flag_compが等しいときtrue
 */
bool 
operator ==
(
	C_BT_304_SLAVE &_arg_bt_slave,
	BOOL _arg_bt_slave_flag_comp
)
{
	if (_arg_bt_slave._mem_bt == _arg_bt_slave_flag_comp)				return true;
	
	if (_arg_bt_slave._mem_bt_slave_falg == _arg_bt_slave_flag_comp)	return true;
	
	return false;
}

/**
 * \brief 
 * Masterと通信しているかを確認する用の演算子
 * TRUEを表すとき接続。FALESの場合切断。
 * 
 * \param _arg_bt_slave
 * \param _arg_bt_slave_flag_comp : 比較
 * 
 * \return bool 
 * _arg_bt_slaveと_arg_bt_slave_flag_compが等しくないときtrue
 */
bool
operator !=
(
	C_BT_304_SLAVE &_arg_bt_slave,
	BOOL _arg_bt_slave_flag_comp
)
{
	if (_arg_bt_slave._mem_bt != _arg_bt_slave_flag_comp)				return true;
	
	if (_arg_bt_slave._mem_bt_slave_falg != _arg_bt_slave_flag_comp)	return true;
	
	return false;
}