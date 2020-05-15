/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#include "TOOL_crc.h"


unsigned short crc_update (unsigned short crc, unsigned char data)
{
	data ^= (crc & 0xff);
	data ^= data << 4;

	return ((((unsigned short )data << 8) | ((crc>>8)&0xff)) ^ (unsigned char )(data >> 4)
			^ ((unsigned short )data << 3));
}

unsigned short crc16(const void* data, unsigned short cnt)
{
	unsigned short crc=0xff;
	unsigned char * ptr=(unsigned char *) data;
	int i;

	for (i=0;i<cnt;i++)
	{
		crc=crc_update(crc,*ptr);
		ptr++;
	}
	return crc;
}

unsigned short updateCrc16(unsigned short crc, void * data, unsigned short cnt)
{
	int i=0;
	unsigned short crcNew=crc;
	unsigned char * chrData=(unsigned char *)data;

	for (i=0;i<cnt;i++)
		crcNew=crc_update(crcNew,chrData[i]);

	return crcNew;
}

unsigned char crc8(const void *vptr, int len)
{
	const unsigned char *data = vptr;
	unsigned crc = 0xff;
	int i, j;
	for (j = len; j; j--, data++)
	{
		crc ^= (*data << 8);
		for(i = 8; i; i--)
		{
			if (crc & 0x8000)
				crc ^= (0x1070 << 3);
			crc <<= 1;
		}
	}
	return (unsigned char)(crc >> 8);
}


//software version of crc32 for  pc programs

unsigned int crc32_init()
{
	return 0xFFFFFFFF;
}
 //feed with 4byte data!
unsigned int crc32_update (unsigned int Data, unsigned int Crc32)
{
	int i;
	Crc32 = Crc32 ^ Data;
    for(i=0; i<32; i++)
    {
    	if (Crc32 & 0x80000000)
	      Crc32 = (Crc32 << 1) ^ 0x04C11DB7; // Polynomial used in STM32
	    else
	      Crc32 = (Crc32 << 1);
    }
	return Crc32;
}
