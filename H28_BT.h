
#pragma once

//#define USED_H28_AM_164 1

#include <H28_AVR/H28_AVR.h>
#include <string.h>

#define BT_DATA_NUM 4
#define BT_ADDR_BYTE 12

/**
 * \brief 
 * _arg_str_f��_arg_str���r���� 
 * _arg_str��_arg_str_f�̂��ׂĂ��܂܂�Ă���΂���ł���
 *
 * \param _arg_str_f : ��r���镶����
 * \param _arg_str : ��r����镶����
 * 
 * \return usint
 *		0 -> ��v
 *		1 -> �s��v
 */
inline usint 
strcmp
(
	const char *_arg_str_f, 
	char *_arg_str
)
{
	for (usint i = 0; _arg_str_f[i] != '\0'; i++)
	{
		if (_arg_str_f[i] != _arg_str[i])
		{
			return 1;
		}
	}
	
	return 0;
}

#include "H28_C_BT.cpp"
#include "H28_C_BT_304.cpp"
#include "H28_C_BT_304_MASTER.cpp"
#include "H28_C_BT_304_SLAVE.cpp"