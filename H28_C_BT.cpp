

#ifndef _H28_C_BT_CPP_
#define _H28_C_BT_CPP_ 1

#include "H28_BT.h"

class C_BT : protected C_UART_base , public C_TIMER_inside
/*
Bluetoothと送受信を行うためのクラス
*/
{
private:

	E_IO_PORT_ADDR _mem_bt_port_cts :8;
	E_IO_PORT_ADDR _mem_bt_port_rts :8;
	E_IO_PORT_ADDR _mem_bt_port_rse :8;
	
	E_IO_NUM _mem_bt_bit_cts :3;
	E_IO_NUM _mem_bt_bit_rts :3;
	E_IO_NUM _mem_bt_bit_rse :3;
	
	E_LOGIC _mem_bt_flag :1;
	
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
	
	void Rce_off()	{	CTS_HIGH;	}
	/*
	CTSをHIGHにして、Bluetoothからの送信を禁止する。
	*/
	
	void Rce_on()	{	CTS_LOW;	}
	/*
	CTSをLOWにして、Bluetoothからの送信を許可する。
	*/
	
	void Out(const char[]);
	void In(char []);
	void In_comp(const char []);
	void In_comp(char [], const char []);
	
	void Reset();
	
	friend void operator << (C_BT &, const char []);
	friend void operator >> (C_BT &, char []);
	friend void operator >> (C_BT &, const char []);
	
	friend bool operator == (C_BT &, E_LOGIC );
	friend bool operator != (C_BT &, E_LOGIC );
};

/************************************************************************/

inline 
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
/*
見ての通りコンストラクタ。
継承の都合上、引数を持たない奴もあるから気を付けてね

	_arg_bt_uart_addr	: Bluetoothと接続するUARTのレジスタ
	_arg_bt_addr_rts	: RTSピンのレジスタ
	_arg_bt_bit_rts		: RTSピンのビット番号
	_arg_bt_addr_cts	: CTSピンのレジスタ
	_arg_bt_bit_cts		: CTSピンのビット番号
	_arg_bt_addr_rse	: RESETピンのレジスタ
	_arg_bt_bit_rse		: RESETピンのビット番号
	
*/
{
	C_TIMER_inside::Set(100);
	
	C_UART_base::Set_uart_base_addr(_arg_bt_uart_addr);
	
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

/************************************************************************/

inline void
C_BT::
Out
(
	const char _arg_bt_out_data[]
)
/*
Bluetoothへデータを送信する。
送信に移れなかったら、タイムアウトする。

	_arg_bt_out_data : 送信するデータ
*/
{	
	for (usint i = 0; _arg_bt_out_data[i] != '\0'; i++)
	{
		C_TIMER_inside::Start();
		
		while (1)
		{
			if ((C_TIMER_inside::Ret_flag() & RTS_CHECK) == TRUE)
			{
				C_TIMER_inside::End();
				
				_mem_bt_flag = TRUE;
				
				PORTB &= ~(1 << PB4);
				
				goto Go_succe;
			}
			
			if (C_TIMER_inside::Check())	//カウント完了(タイムアウト)
			{				
				_mem_bt_flag = FALES;
				
				PORTB |= (1 << PB4);
				
				return (void)0;
			}
		}
		
		Go_succe:
		
		while (!(UCSRA & (1<<UDRE)));
		
		UDR = _arg_bt_out_data[i];
	}
}

/************************************************************************/

inline void
C_BT::
In
(
	char _arg_re_bt_in_data[]
)
/*
Bluetoothからのデータを受信する
受信に移れなかったら、タイムアウトする。

	_arg_re_bt_in_data : ここに受信データが格納される。
*/
{	
	usint i = 0;
	
	while (1)
	{
		CTS_LOW;
		
		C_TIMER_inside::Start();
		
		while (1)
		{
			if ((C_TIMER_inside::Ret_flag() & CHECK_BIT_TF(UCSRA,RXC)) == TRUE)	//通信可能
			{
				C_TIMER_inside::End();
				
				_mem_bt_flag = TRUE;
				
				goto GO_succe;
			}
			
			if (C_TIMER_inside::Check())	//カウント完了(タイムアウト)
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

/************************************************************************/

void 
C_BT::
In_comp
(
	const char _arg_bt_str_comp[]
)
/*
_arg_bt_str_compと一致するまで受信し続ける。

	_arg_bt_str_comp : 比較する文字列。
*/
{
	char _in_data[40] = {};
	
	do
	{
		In(_in_data);
	}
	while (strcmp(_arg_bt_str_comp,_in_data) != 0);
}

/************************************************************************/

void
C_BT::
In_comp
(
	char _arg_re_bt_in_data[],
	const char _arg_bt_str_comp[]
)
/*
_arg_bt_str_compと一致するまで受信し続け、一致したデータを返す。

	_arg_re_bt_in_data : ここにデータが格納される
	_arg_bt_str_comp : 比較する文字列
*/
{
	do
	{
		In(_arg_re_bt_in_data);
	}
	while (strcmp(_arg_bt_str_comp,_arg_re_bt_in_data) != 0);
}

/************************************************************************/

void 
C_BT::
Reset
()
/*
Bluetoothを再起動する
*/
{	
	RSE_LOW;
	_delay_ms(15);

	RSE_HIGH;
	_delay_ms(15);
}

/************************************************************************/

void 
operator << 
(
	C_BT &_arg_bt, 
	const char _arg_bt_out_data[]
)
/*
C_BT::Out()の演算子ver
*/
{
	_arg_bt.Out(_arg_bt_out_data);
}

/************************************************************************/

void 
operator >> 
(
	C_BT &_arg_bt, 
	char _arg_re_bt_in_data[]
)
/*
C_BT::In()の演算子ver
*/
{
	_arg_bt.In(_arg_re_bt_in_data);
}

/************************************************************************/

void 
operator >> 
(
	C_BT &_arg_bt, 
	const char _arg_bt_str_comp[]
)
/*
C_BT::In_comp()の演算子ver
*/
{
	_arg_bt.In_comp(_arg_bt_str_comp);
}

/************************************************************************/

bool 
operator == 
(
	C_BT &_arg_bt, 
	E_LOGIC _arg_bt_flag_comp
)
/*
if文などで使うための演算子
Bluetoothと接続しているかどうかの確認用

	TRUE  -> 生存
	FALES -> 死亡
*/
{
	if (_arg_bt._mem_bt_flag == _arg_bt_flag_comp)	return true;
	
	return false;
}

/************************************************************************/

bool 
operator != 
(
	C_BT &_arg_bt, 
	E_LOGIC _arg_bt_flag_comp
)
/*
if文などで使うための演算子
Bluetoothと接続しているかどうかの確認用

	TRUE  -> 生存
	FALES -> 死亡
*/
{
	if (_arg_bt._mem_bt_flag != _arg_bt_flag_comp)	return true;
	
	return false;
}
#endif