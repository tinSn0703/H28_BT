

#pragma once

#include "H28_BT.h"

/**
 * Bluetoothと送受信を行うためのクラス
 */
class C_BT : protected C_UART_base
{
private:

	E_IO_PORT_ADDR _mem_bt_port_cts :8;
	E_IO_PORT_ADDR _mem_bt_port_rts :8;
	E_IO_PORT_ADDR _mem_bt_port_rse :8;
	
	E_IO_NUM _mem_bt_bit_cts :3;
	E_IO_NUM _mem_bt_bit_rts :3;
	E_IO_NUM _mem_bt_bit_rse :3;
	
	BOOL _mem_bt_flag :1;
	
	C_TIMER_inside _mem_uart_timer;
	
protected:

	E_IO_PORT_ADDR Ret_port_cts()	{	return _mem_bt_port_cts;	}
	E_IO_PORT_ADDR Ret_port_rts()	{	return _mem_bt_port_rts;	}
	E_IO_PORT_ADDR Ret_port_rse()	{	return _mem_bt_port_rse;	}
	
	E_IO_NUM Ret_bit_cts()	{	return _mem_bt_bit_cts;	}
	E_IO_NUM Ret_bit_rts()	{	return _mem_bt_bit_rts;	}
	E_IO_NUM Ret_bit_rse()	{	return _mem_bt_bit_rse;	}
	
#	define PIN_RTS  _SFR_IO8(Ret_port_rts() + 0)
#	define PIN_CTS  _SFR_IO8(Ret_port_cts() + 0)
#	define PIN_RSE  _SFR_IO8(Ret_port_rse() + 0)
#	define DDR_RTS	_SFR_IO8(Ret_port_rts() + 1)
#	define DDR_CTS  _SFR_IO8(Ret_port_cts() + 1)
#	define DDR_RSE  _SFR_IO8(Ret_port_rse() + 1)
#	define PORT_RTS _SFR_IO8(Ret_port_rts() + 2)
#	define PORT_CTS _SFR_IO8(Ret_port_cts() + 2)
#	define PORT_RSE _SFR_IO8(Ret_port_rse() + 2)
	
#	define RTS_CHECK (CHECK_TURN_BIT_TF(PIN_RTS,Ret_bit_rts()))
	
#	define CTS_HIGH	(PORT_CTS |=  (1 << Ret_bit_cts()))
#	define CTS_LOW	(PORT_CTS &= ~(1 << Ret_bit_cts()))
	
#	define RSE_HIGH	(PORT_RSE |=  (1 << Ret_bit_rse()))
#	define RSE_LOW	(PORT_RSE &= ~(1 << Ret_bit_rse()))
	
public:

	C_BT()	{}
	C_BT(E_UART_ADDR ,E_IO_PORT_ADDR ,E_IO_NUM ,E_IO_PORT_ADDR ,E_IO_NUM ,E_IO_PORT_ADDR ,E_IO_NUM );
	
	/**
	 * \brief CTSをHIGHにして、Bluetoothからの送信を禁止する。
	 */
	void Rce_off()	{	CTS_HIGH;	}
	
	/**
	 * \brief CTSをLOWにして、Bluetoothからの送信を許可する。
	 */
	void Rce_on()	{	CTS_LOW;	}
	
	void Out(const char[]);
	void In(char []);
	void In_comp(const char []);
	void In_comp(char [], const char []);
	
	void Reset();
	
	friend void operator << (C_BT &, const char []);
	friend void operator >> (C_BT &, char []);
	friend void operator >> (C_BT &, const char []);
	
	friend bool operator == (C_BT &, BOOL );
	friend bool operator != (C_BT &, BOOL );
};

/**
 * \brief コンストラクタ
 * 
 * \param _arg_bt_uart_addr : Bluetoothと接続するUARTのレジスタ
 * \param _arg_bt_addr_rts  : RTSピンのレジスタ
 * \param _arg_bt_bit_rts	  : RTSピンのビット
 * \param _arg_bt_addr_cts  : CTSピンのレジスタ
 * \param _arg_bt_bit_cts   : CTSピンのビット
 * \param _arg_bt_addr_rse  : RESETピンのレジスタ
 * \param _arg_bt_bit_rse   : RESETピンのビット
 */
C_BT::
C_BT
(	
	E_UART_ADDR		_arg_bt_uart_addr, 
	E_IO_PORT_ADDR	_arg_bt_addr_rts,
	E_IO_NUM		_arg_bt_bit_rts, 
	E_IO_PORT_ADDR	_arg_bt_addr_cts, 
	E_IO_NUM		_arg_bt_bit_cts,
	E_IO_PORT_ADDR	_arg_bt_addr_rse,
	E_IO_NUM		_arg_bt_bit_rse
)
: _mem_uart_timer(100) //10ms
{	
	C_UART_base::_mem_uart_base_addr = _arg_bt_uart_addr;
	
	_mem_bt_port_cts = _arg_bt_addr_cts;
	_mem_bt_port_rts = _arg_bt_addr_rts;
	_mem_bt_port_rse = _arg_bt_addr_rse;
	
	_mem_bt_bit_cts = _arg_bt_bit_cts;
	_mem_bt_bit_rts = _arg_bt_bit_rts;
	_mem_bt_bit_rse = _arg_bt_bit_rse;
	
	//115.2kbps double-on parity-off
	//rec,tra-mode 8bit interrupt-off
	UBRRH = 0x00;
	UBRRL = 0x0a;
	UCSRA = (1<<U2X);
	UCSRB = ((1<<RXEN) | (1<<TXEN));
	UCSRC = ((1<<UCSZ0) | (1<<UCSZ1));

	DDR_CTS |=  (1 << _arg_bt_bit_cts);
	DDR_RSE |=  (1 << _mem_bt_bit_rse);
	DDR_RTS &= ~(1 << _arg_bt_bit_rts);
	
	PORT_CTS |= (1 << _arg_bt_bit_cts);
	PORT_RSE |= (1 << _mem_bt_bit_rse);
	
	_mem_bt_flag = FALES;
}

/**
 * \brief 
 * Bluetoothへデータを送信する
 * 送信に移れなかった場合はフラグをFALESにしてタイムアウトする。
 * 
 * \param _arg_bt_out_data : 送信するデータ。
 */
inline void
C_BT::
Out (const char _arg_bt_out_data[])
{	
	for (usint i = 0; _arg_bt_out_data[i] != '\0'; i++)
	{
		_mem_uart_timer.Start();
		
		while (1)
		{
			if ((_mem_uart_timer.Ret_flag() & RTS_CHECK) == TRUE) //通信可能
			{
				_mem_uart_timer.End();
				
				_mem_bt_flag = TRUE;
				
				goto Go_succe;
			}
			
			if (_mem_uart_timer.Check())	//カウント完了(タイムアウト)
			{				
				_mem_bt_flag = FALES;
				
				return (void)0;
			}
		}
		
		Go_succe:
		
		while (!(UCSRA & (1<<UDRE)));
		
		UDR = _arg_bt_out_data[i];
	}
}

/**
 * \brief 
 * Bluetoothからのデータを受け取る
 * 受信に移れなかったら、タイムアウトする。
 * 
 * \param _arg_re_bt_in_data : ここに受信データが格納される
 */
inline void
C_BT::
In (char _arg_re_bt_in_data[])
{	
	usint i = 0;
	
	while (1)
	{
		CTS_LOW;
		
		_mem_uart_timer.Start();
		
		while (1)
		{
			if ((_mem_uart_timer.Ret_flag() & CHECK_BIT_TF(UCSRA,RXC)) == TRUE)	//通信可能
			{
				_mem_uart_timer.End();
				
				_mem_bt_flag = TRUE;
				
				goto GO_succe;
			}
			
			if (_mem_uart_timer.Check())	//カウント完了(タイムアウト)
			{				
				CTS_HIGH;
				
				_mem_bt_flag = FALES;
				
				return (void)0;
			}
		}
		
		GO_succe:
		
		_arg_re_bt_in_data[i] = UDR;
		
		CTS_HIGH;
		
		if ((_arg_re_bt_in_data[i] == '\n') && (i > 1))
		{
			_arg_re_bt_in_data[i + 1] = '\0';
			
			break;
		}
		
		i++;
	}
}

/**
 * \brief 
 * _arg_bt_str_compと一致するまで受信し続ける
 * ちゃんとしないと無限ループに陥るので注意
 * 
 * \param _arg_bt_str_comp : 比較する文字列
 */
void 
C_BT::
In_comp (const char _arg_bt_str_comp[])
{
	char _in_data[40] = {};
	
	do
	{
		In(_in_data);
	}
	while (strcmp(_arg_bt_str_comp,_in_data) != 0);
}

/**
 * \brief 
 * _arg_bt_str_compと一致するまで受信し続ける
 * 一致したデータを返す
 * 
 * \param _arg_re_bt_in_data : ここに受信データが格納される
 * \param _arg_bt_str_comp : 比較する文字列
 */
void
C_BT::
In_comp
(
	char _arg_re_bt_in_data[],
	const char _arg_bt_str_comp[]
)
{
	do
	{
		In(_arg_re_bt_in_data);
	}
	while (strcmp(_arg_bt_str_comp,_arg_re_bt_in_data) != 0);
}

/**
 * \brief Bluetoothを再起動する
 */
void 
C_BT::
Reset ()
{	
	RSE_LOW;
	_delay_ms(15);

	RSE_HIGH;
	_delay_ms(15);
}

/**
 * \brief 
 * Bluetoothにデータを送信する演算子
 * C_BT::Out()と同じ機能
 * 
 * \param _arg_bt
 * \param _arg_bt_out_data : 送信するデータ
 */
void 
operator << 
(
	C_BT &_arg_bt, 
	const char _arg_bt_out_data[]
)
{
	_arg_bt.Out(_arg_bt_out_data);
}

/**
 * \brief
 * Bluetoothからのデータを受信する演算子。
 * 右辺が定数の場合、それを受信するまで受信し続ける
 * C_BT::In()とC_BT::In_comp()と同じ機能
 * 
 * \param _arg_bt
 * \param _arg_re_bt_in_data[] : データが格納される場所
 */
void 
operator >> 
(
	C_BT &_arg_bt, 
	char _arg_re_bt_in_data[]
)
{
	_arg_bt.In(_arg_re_bt_in_data);
}

/**
 * \brief
 * Bluetoothからのデータを受信する演算子。
 * 右辺が定数の場合、それを受信するまで受信し続ける
 * C_BT::In()とC_BT::In_comp()と同じ機能
 * 
 * \param _arg_bt
 * \param _arg_bt_str_comp : 比較する文字列
 */
void 
operator >> 
(
	C_BT &_arg_bt, 
	const char _arg_bt_str_comp[]
)
{
	_arg_bt.In_comp(_arg_bt_str_comp);
}

/**
 * \brief 
 * Bluetoothと接続しているのか確認用の演算子。
 * TRUEを表す場合接続、FALESを表す場合切断
 * 
 * \param _arg_bt
 * \param _arg_bt_flag_comp : 比較
 * 
 * \return bool _arg_btと_arg_bt_flag_compが等しいときtrue
 */
bool 
operator == 
(
	C_BT &_arg_bt, 
	BOOL _arg_bt_flag_comp
)
{
	if (_arg_bt._mem_bt_flag == _arg_bt_flag_comp)	return true;
	
	return false;
}

/**
 * \brief 
 * Bluetoothと接続しているのか確認用の演算子。
 * TRUEを表す場合接続、FALESを表す場合切断
 * 
 * \param _arg_bt
 * \param _arg_bt_flag_comp : 比較
 * 
 * \return bool _arg_btと_arg_bt_flag_compが等しくないときtrue
 */
bool 
operator != 
(
	C_BT &_arg_bt, 
	BOOL _arg_bt_flag_comp
)
{
	if (_arg_bt._mem_bt_flag != _arg_bt_flag_comp)	return true;
	
	return false;
}