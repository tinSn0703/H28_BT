
#pragma once

//#define USED_H28_AM_164 1

#include <H28_AVR/H28_AVR.h>
#include <string.h>

#define __BT_DATA_NUM__		4
#define __BT_ADDR_BYTE__	12

#define BT_DATA_NUM		4
#define BT_ADDR_BYTE	12

#define __CTS__ (1 << PD4)
#define __RTS__ (1 << PD5)
#define __RSE__ (1 << PD6)

#define __CTS_PIN__ PIND
#define __RTS_PIN__ PIND
#define __RSE_PIN__ PIND

#define __CTS_PORT__ PORTD
#define __RTS_PORT__ PORTD
#define __RSE_PORT__ PORTD

#define __CTS_DDR__ DDRD
#define __RTS_DDR__ DDRD
#define __RSE_DDR__ DDRD

#define __CTS_READ__ (__CTS_PIN__ & __CTS__)
#define __RTS_READ__ (__RTS_PIN__ & __RTS__)
#define __RSE_READ__ (__RSE_PIN__ & __RSE__)

#define __RTS_READ_T__ ((__RTS_PIN__ & __RTS__) != 0x00)
#define __RTS_READ_F__ ((__RTS_PIN__ & __RTS__) == 0x00)

#define __CTS_HIGH__ (__CTS_PORT__ |=  __CTS__)
#define __CTS_LOW___ (__CTS_PORT__ &= ~__CTS__)

#define __RSE_HIGH__ (__RSE_PORT__ |=  __RSE__)
#define __RSE_LOW___ (__RSE_PORT__ &= ~__RSE__)

enum E_BT_NUM
{
	EB_BT0 = 0xfe,
	EB_BT1 = 0x01,
	EB_BT2 = 0xfd,
	EB_BT3 = 0x02,
	EB_BT4 = 0xfb,
	EB_BT5 = 0x04,
	EB_BT6 = 0xf7,
	EB_BT7 = 0x08,
	EB_BT8 = 0xef,
	EB_BT9 = 0x10,
	EB_BTA = 0xdf,
	EB_BTB = 0x20,
	EB_BTC = 0xbf,
	EB_BTD = 0x40,
	EB_BTE = 0x7f,
	EB_BTF = 0x80,
};

/**
 * \brief 
 * _arg_str_f‚Æ_arg_str‚ð”äŠr‚·‚é 
 * _arg_str‚É_arg_str_f‚Ì‚·‚×‚Ä‚ªŠÜ‚Ü‚ê‚Ä‚¢‚ê‚Î‚»‚ê‚Å‚¢‚¢
 *
 * \param _arg_str_f : ”äŠr‚·‚é•¶Žš—ñ
 * \param _arg_str : ”äŠr‚³‚ê‚é•¶Žš—ñ
 * 
 * \return usint
 *		0 -> ˆê’v
 *		1 -> •sˆê’v
 */
int strcmp (const char *_arg_str_f, char *_arg_str);

char * F_str_copy (char _arg_str_base[], const char _arg_str[]);

void F_Set_bt (E_UART_ADDR _arg_uart_addr);

void F_Out_bt (E_UART_ADDR _arg_uart_addr_t, const char _arg_data_out[]);

char * F_In_bt (E_UART_ADDR _arg_uart_addr_r, char _arg_data_in[]);

char * F_In_bt (E_UART_ADDR _arg_uart_addr_r, char _arg_data_in[], const char _arg_data_comp[]);

void F_In_comp_bt (E_UART_ADDR _arg_uart_addr_r, const char _arg_data_comp[]);

#if defined(_AKILCD_H_)

char * F_Set_bt_addr (E_BT_NUM _arg_num_bt, char _arg_bt_addr[], char _arg_char_num);

#else

char * F_Set_bt_addr (E_BT_NUM _arg_num_bt, char _arg_bt_addr[]);

#endif

BOOL F_Connect_bt_slave (E_UART_ADDR _arg_uart_addr, E_BT_NUM _arg_num_bt);

BOOL F_Connect_bt_master (E_UART_ADDR _arg_uart_addr, E_BT_NUM _arg_num_bt);

void F_Re_Connect_bt_slave (E_UART_ADDR _arg_uart_addr, E_BT_NUM _arg_num_bt);

void F_Re_Connect_bt_master (E_UART_ADDR _arg_uart_addr, E_BT_NUM _arg_num_bt);

void F_Cut_Connect_bt();

#include "H28_BT.cpp"

#include "H28_C_BT.cpp"
#include "H28_C_BT_304.cpp"
#include "H28_C_BT_304_MASTER.cpp"
#include "H28_C_BT_304_SLAVE.cpp"