
#ifndef _H28_BT_H_
#define _H28_BT_H_ 1

#include <H28_AVR/H28_AVR.h>
#include <string.h>

#define BT_DATA_NUM 4
#define BT_ADDR_BYTE 12

inline usint strcmp(const char *_arg_str_f, char *_arg_str)
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

#endif