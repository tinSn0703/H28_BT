
/******************************************************************************
 H28 10 / 23 (Sun)
・改造案
	・C_BT_304とその派生先を削除。全て関数に変える。
	・__CTS__などを#ifndefを用いることで、プロジェクトによって調整可能にする。
	・C_BTの内部の大幅な見直し。
		・__RTS__のTimerでのチェックを行わないメンバの追加。-> めんどいからやっぱりいい。
		・受信フラグのTimerでのチェックを行わないメンバの追加。-> 同上
		・割り込み処理に対する工夫。
		・どうせ、戻り値使ってないし、戻り値でエラー吐かせる。
		・メンバ変数の__CTS__などを削除し、外部も#defineに置き換える。
	・C_BTの専用の関数の作成。ROBOBAとBlueMasterをこれで使い分ける。
	・ピンを#defineにすることで、C_BT外部でも分割した関数でも同じように用いることができるようにする。
	
 ・改造点
	・C_BTのCTS用などのメンバ変数を削除。外部の#defineを用いる構成に。
	・__CTS__など、回路によって変更する部分を#ifndefを用いてプロジェクトごとに変更可能に。
	・__XXX_ADDR__系を追加して、変更可能箇所を減らし、負担とミスの軽減。
	・__XXX__系でシフトしていたが、使い勝手が悪いのでシフトした__XXX_BIT__系を用意。
	・__XXX_READ__系はBOOL型に変換。
	・新たに__XXX_SET__系を追加。DDRへの入力ミスを無くす。
	・C_BT::Out()とC_BT::In()にBOOL型の戻り値を追加。これで、エラーのチェックが可能。
	
 H28 10 / 24 (Mon)
	色々やってみたけど、原因不明のバグが発生しました。なんか、コントローラからのデータのデータを受け取
　れませんでした。確認してみるとUART自体が機能してなさそう。TIMERが効かない。単体では普通に動いた。でも、
　BTのと合わせると機能しなくなる。ちょっと意味が分からない。fuck
　　C_BTが関係あるかは不明。C_BT用関数自体は問題なかった(消したけど)。このファイル群に置くプログラムは、
　BTとの通信用のものだけにしておきます。あまりよくばらないほうがいいのかねえ。fuck	
	
******************************************************************************/

#pragma once

#include <H28_AVR/H28_AVR.h>
#include <string.h>

/************************************************************************/

#define __BT_DATA_NUM__		4
#define __BT_ADDR_BYTE__	12

#define BT_DATA_NUM		4
#define BT_ADDR_BYTE	12

#ifndef __CTS_ADDR__
#	define __CTS_ADDR__ EI_PORTD
#endif
#ifndef __RTS_ADDR__
#	define __RTS_ADDR__ EI_PORTD
#endif
#ifndef __RSE_ADDR__
#	define __RSE_ADDR__ EI_PORTD
#endif

#ifndef __CTS__
#	define __CTS__ EI_IO4
#endif
#ifndef __RTS__
#	define __RTS__ EI_IO5
#endif
#ifndef __RSE__
#	define __RSE__ EI_IO6
#endif

#define __CTS_BIT__ (1 << __CTS__)
#define __RTS_BIT__ (1 << __RTS__)
#define __RSE_BIT__ (1 << __RSE__)

#define __CTS_PIN__ _SFR_IO8(__CTS_ADDR__ + 0)
#define __RTS_PIN__ _SFR_IO8(__RTS_ADDR__ + 0)
#define __RSE_PIN__ _SFR_IO8(__RSE_ADDR__ + 0)

#define __CTS_DDR__ _SFR_IO8(__CTS_ADDR__ + 1)
#define __RTS_DDR__ _SFR_IO8(__RTS_ADDR__ + 1)
#define __RSE_DDR__ _SFR_IO8(__RSE_ADDR__ + 1)

#define __CTS_PORT__ _SFR_IO8(__CTS_ADDR__ + 2)
#define __RTS_PORT__ _SFR_IO8(__RTS_ADDR__ + 2)
#define __RSE_PORT__ _SFR_IO8(__RSE_ADDR__ + 2)

#define __CTS_SET__ (__CTS_DDR__ |=  __CTS_BIT__)
#define __RTS_SET__ (__RTS_DDR__ &= ~__RTS_BIT__)
#define __RSE_SET__ (__RSE_DDR__ |=  __RSE_BIT__)

#define __CTS_READ__ ((BOOL)((__CTS_PIN__ >> __CTS__) & 1))
#define __RTS_READ__ ((BOOL)((__RTS_PIN__ >> __RTS__) & 1))
#define __RSE_READ__ ((BOOL)((__RSE_PIN__ >> __RSE__) & 1))

#define __CTS_HIGH__ (__CTS_PORT__ |=  __CTS_BIT__)
#define __RSE_HIGH__ (__RSE_PORT__ |=  __RSE_BIT__)

#define __CTS_LOW___ (__CTS_PORT__ &= ~__CTS_BIT__)
#define __RSE_LOW___ (__RSE_PORT__ &= ~__RSE_BIT__)

enum E_BT_NUM
{
	EB_BT0 = 0x00,
	EB_BT1 = 0x01,
	EB_BT2 = 0x02,
	EB_BT3 = 0x03,
	EB_BT4 = 0x04,
	EB_BT5 = 0x05,
	EB_BT6 = 0x06,
	EB_BT7 = 0x07,
	EB_BT8 = 0x08,
	EB_BT9 = 0x09,
	EB_BTA = 0x0a,
	EB_BTB = 0x0b,
	EB_BTC = 0x0c,
	EB_BTD = 0x0d,
	EB_BTE = 0x0e,
	EB_BTF = 0x0f,
};

/************************************************************************/

/**
 * \brief 
 * _arg_str_fと_arg_strを比較する 
 * _arg_strに_arg_str_fのすべてが含まれていれば一致となる
 *
 * \param _arg_str_base : 比較する文字列
 * \param _arg_str_comp : 比較される文字列
 * 
 * \return BOOL : 比較の結果
 *	不一致	-> FALSE
 *	一致	-> TRUE
 */
BOOL F_Str_comp (const char _arg_str_base[], char _arg_str_comp[]);

/**
 * \brief 
 *	_arg_str_baseを_arg_strに書き換えます。
 *	どちらかが、最後になるまで書き換えます。それ以降はそのまま。
 *
 * \param _arg_str_base	: 書き換えられる文字列
 * \param _arg_str_copy	: 書き換える文字列
 * 
 * \return char * _arg_str_baseがreturnされます
 */
char * F_Str_copy (char _arg_str_base[], const char _arg_str_copy[]);

/**
 * \brief 
 *	_arg_str_baseの後ろに_arg_str_addを追加します
 * 
 * \param _arg_str_base : 追加される文字列
 * \param _arg_str_add  : 追加する文字列
 */
void F_Str_add(char _arg_str_base[], const char _arg_str_add);

/**
 * \brief 
 *	Bluetoothへ_arg_data_outを送信する。
 *	送信完了するまで続きます。プログラムが停止したら疑ってみてね。
 *
 * \param _arg_uart_addr_t	: 送信に用いるUART
 * \param _arg_data_out		: 送信するdata
 */
void F_BT_Out (E_UART_ADDR _arg_uart_addr_t, const char _arg_data_out[]);

/**
 * \brief 
 *	Bluetoothから受信する。
 *	受信したdataは_arg_re_data_inに格納される。
 *	受信完了するまで続きます。プログラムが停止したら疑ってみてね。 
 *
 * \param _arg_uart_addr_r	: 受信に用いるUART
 * \param _arg_re_data_in	: 受信したdataを格納する所
 */
void F_BT_In (E_UART_ADDR _arg_uart_addr_r, char _arg_re_data_in[]);

/**
 * \brief 
 *	受信したデータが_arg_data_compに一致するまで受信し続ける。
 *	仕様上、合わなかったら無限ループ状態になるので気を付けてね。 
 *
 * \param _arg_uart_addr_r	: 用いるUART
 * \param _arg_data_comp	: 比較対象
 */
void F_BT_In_comp (E_UART_ADDR _arg_uart_addr_r, const char _arg_data_comp[]);

/**
 * \brief 
 *	受信したデータが_arg_data_compに一致するまで受信し続ける。
 *	受信したデータは_arg_re_data_inに格納される。
 *	仕様上、合わなかったら無限ループ状態になるので気を付けてね。 
 *
 * \param _arg_uart_addr_r	: 用いるUART
 * \param _arg_re_data_in	: 受信したデータを格納する場所
 * \param _arg_data_comp	: 比較対象
 */
void F_BT_In_comp (E_UART_ADDR _arg_uart_addr_r, char _arg_re_data_in[], const char _arg_data_comp[]);

/**
 * \brief 
 *	Bluetoothをコマンドモードにする。
 */
void F_BT_Cut();

/************************************************************************/

#include "H28_C_BT.cpp"

#include "H28_BT.cpp"


//#include "H28_C_BT_304.cpp"
//#include "H28_C_BT_304_MASTER.cpp"
//#include "H28_C_BT_304_SLAVE.cpp"