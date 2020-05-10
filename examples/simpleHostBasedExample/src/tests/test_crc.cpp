/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */

#include "toTest/TOOL_crc.h"
#include "eUnit.h"

uint8_t buff1[] = {0,0,0,0,0,0};
uint8_t buff2[] = {1,2,3,4,5,6,7};
uint8_t buff3[] = {255,255,255};
uint8_t buff4[5000];

unsigned int buff5[] = {0xffffffff, 0xff000000,0x11000000,0x00ff0000,0x00110000,0x0000ff00,0x00001100,0x000000ff,0x000000ff,0x8f000000,0x11000000,0x008f0000,0x80110000,0x8000ff00,0x80001100,0x800000ff,0x800000ff 	,0xAAAAAAAA,0xDDDDDD,0x11111111,0x555555,0xABABABAB};


TEST(CRC,shallNotFail)
{
	ASSERT_EQ(1,1);
}

//unsigned short crc_update (unsigned short crc, unsigned char data)
TEST(CRC,updateCRC_0_Data0)
{

	short isCrc = 0;
	ASSERT_EQ(crc_update(isCrc,0),0);
	ASSERT_EQ(crc_update(isCrc,0),0);

}
TEST(CRC,updateCRC_1_Data0)
{
	short isCrc = 1;
	ASSERT_EQ(crc_update(isCrc,0),0x1189);
}

TEST(CRC,updateCRC_Neg1_Data0)
{
	short isCrc = -1;
	isCrc = crc_update(isCrc,0);
	ASSERT_EQ(isCrc,0xf87);
	ASSERT_EQ(crc_update(isCrc,0),0xf0b8);

}

TEST(CRC,updateCRC_1_Data1)
{
	short isCrc = 1;
	isCrc = crc_update(isCrc,1);
	ASSERT_EQ(isCrc,0);
	ASSERT_EQ(crc_update(isCrc,1),4489);

}

TEST(CRC,updateCRC_0xff_Data1)
{
	short isCrc = 0xff;
	isCrc = crc_update(isCrc,0);
	ASSERT_EQ(isCrc,0xf78);
	ASSERT_EQ(crc_update(isCrc,0),0xFFC0);

}

// crc16(const void* data, unsigned short cnt)
TEST(CRC,crc16_Data_Zeros)
{
	short isCrc = crc16(buff1,sizeof(buff1));
	ASSERT_EQ(isCrc,-726);
}
TEST(CRC,crc16_Data_1To7)
{
	short isCrc = crc16(buff2,sizeof(buff2));
	ASSERT_EQ(isCrc,28628);
}

TEST(CRC,crc16_Data_Neg1)
{
	short isCrc = crc16(buff3,sizeof(buff3));
	ASSERT_EQ(isCrc,-3912);
}

// crc16(const void* data, unsigned short cnt)
TEST(CRC,crc16_Data_Zeros_Size0)
{
	short isCrc = crc16(buff1,0);
	ASSERT_EQ(isCrc,255);
}
TEST(CRC,crc16_Data_1To7_Size0)
{
	short isCrc = crc16(buff2,0);
	ASSERT_EQ(isCrc,255);
}

TEST(CRC,crc16_Data_Neg1_Size0)
{
	short isCrc = crc16(buff3,0);
	ASSERT_EQ(isCrc,255);
}


TEST(CRC,crc16_Data_X_Size0)
{
	short isCrc = crc16(buff4,0);
	ASSERT_EQ(isCrc,255);
}

TEST(CRC,crc16_Size5000_Data_Zeros)
{
	for(int i = 0; i<sizeof(buff4);i++)
	{
		buff4[i] = 0;
	}
	short isCrc = crc16(buff4,sizeof(buff4));
	ASSERT_EQ(isCrc,18812);
}

TEST(CRC,crc16_Size5000_Data_Neg1)
{
	for(int i = 0; i<sizeof(buff4);i++)
		{
		buff4[i] = -1;
	}
	short isCrc = crc16(buff4,sizeof(buff4));
	ASSERT_EQ(isCrc,-30793);
}
TEST(CRC,crc16_Size5000_Data_1)
{
	for(int i = 0; i<sizeof(buff4);i++)
	{
		buff4[i] = 1;
	}
	short isCrc = crc16(buff4,sizeof(buff4));
	ASSERT_EQ(isCrc,-4735);
}

TEST(CRC,crc16_Size5000_Data_Index)
{
	for(int i = 0; i<sizeof(buff4);i++)
	{
		buff4[i] = i;
	}
	short isCrc = crc16(buff4,sizeof(buff4));
	ASSERT_EQ(isCrc,-13543);
}

TEST(CRC,crc16_Size5000_Data_ReverseIndex)
{
	for(int i = 0; i<sizeof(buff4);i++)
	{
		buff4[i] = sizeof(buff4)-i;
	}
	short isCrc = crc16(buff4,sizeof(buff4));
	ASSERT_EQ(isCrc,23094);
}


// updateCrc16(unsigned short crc, void * data, unsigned short cnt)


TEST(CRC,updatecrc16_Size5000_Data_Neg1_step1)
{
	for(int i = 0; i<sizeof(buff4);i++)
	{
		buff4[i] = -1;
	}
	short isCrc = 0xff;
	for(int i = 0; i<sizeof(buff4);i++)
	{
		isCrc = updateCrc16(isCrc,buff4+i,1);
	}
	ASSERT_EQ(isCrc,-30793);
}
TEST(CRC,updatecrc16_Size5000_Data_Neg1_step5)
{
	for(int i = 0; i<sizeof(buff4);i++)
	{
		buff4[i] = -1;
	}
	short isCrc = 0xff;
	for(int i = 0; i<sizeof(buff4)/5;i++)
	{
		isCrc = updateCrc16(isCrc,buff4+(i*5),5);
	}
	ASSERT_EQ(isCrc,-30793);
}

TEST(CRC,updatecrc16_Size5000_Data_Index_step1)
{
	for(int i = 0; i<sizeof(buff4);i++)
	{
		buff4[i] = i;
	}
	short isCrc = 0xff;
	for(int i = 0; i<sizeof(buff4);i++)
	{
		isCrc = updateCrc16(isCrc,buff4+i,1);
	}
	ASSERT_EQ(isCrc,-13543);
}
TEST(CRC,updatecrc16_Size5000_Data_Index_step5)
{
	for(int i = 0; i<sizeof(buff4);i++)
	{
		buff4[i] = i;
	}
	short isCrc = 0xff;
	for(int i = 0; i<sizeof(buff4)/5;i++)
	{
		isCrc = updateCrc16(isCrc,buff4+(i*5),5);
	}
	ASSERT_EQ(isCrc,-13543);
}






TEST(CRC,crc8_Size5000_Data_Zeros)
{
	for(int i = 0; i<sizeof(buff4);i++)
	{
		buff4[i] = 0;
	}
	uint8_t isCrc = crc8(buff4,sizeof(buff4));
	ASSERT_EQ(isCrc,192);
}

TEST(CRC,crc8_Size5000_Data_Neg1)
{
	for(int i = 0; i<sizeof(buff4);i++)
	{
		buff4[i] = -1;
	}
	uint8_t isCrc = crc8(buff4,sizeof(buff4));
	ASSERT_EQ(isCrc,197);
}
TEST(CRC,crc8_Size5000_Data_1)
{
	for(int i = 0; i<sizeof(buff4);i++)
	{
		buff4[i] = 1;
	}
	uint8_t isCrc = crc8(buff4,sizeof(buff4));
	ASSERT_EQ(isCrc,65);
}

TEST(CRC,crc8_Size5000_Data_Index)
{
	for(int i = 0; i<sizeof(buff4);i++)
	{
		buff4[i] = i;
	}
	uint8_t isCrc = crc8(buff4,sizeof(buff4));
	ASSERT_EQ(isCrc,58);
}

TEST(CRC,crc8_Size5000_Data_ReverseIndex)
{
	for(int i = 0; i<sizeof(buff4);i++)
	{
		buff4[i] = sizeof(buff4)-i;
	}
	uint8_t isCrc = crc8(buff4,sizeof(buff4));
	ASSERT_EQ(isCrc,202);
}



TEST(CRC,CRC32)
{
	ASSERT_EQ(crc32_init(),0xffffffff);
}
TEST(CRC,CRC32_Size5000_Data_Zeros)
{
	uint32_t isCRC = crc32_init();
		for(int i = 0; i<1000;i++)
		{
			isCRC = crc32_update(0,isCRC);
		}
		ASSERT_EQ(isCRC,2184588963);
}

TEST(CRC,CRC32_Size5000_Data_Neg1)
{
	uint32_t isCRC = crc32_init();
		for(int i = 0; i<1000;i++)
		{
			isCRC = crc32_update(-1,isCRC);
		}
		ASSERT_EQ(isCRC,1124705228);
}
TEST(CRC,crc32_Size5000_Data_1)
{
	uint32_t isCRC = crc32_init();
		for(int i = 0; i<1000;i++)
		{
			isCRC = crc32_update(1,isCRC);
		}
		ASSERT_EQ(isCRC,3755708822);
}

TEST(CRC,CRC32_Size5000_Data_Index)
{
	uint32_t isCRC = crc32_init();
		for(int i = 0; i<1000;i++)
		{
			isCRC = crc32_update(i,isCRC);
		}
		ASSERT_EQ(isCRC,95420490);
}

// crc32_update (unsigned int Data, unsigned int Crc32)
TEST(CRC,CRC32_Size5000_Data_ReverseIndex)
{
	uint32_t isCRC = crc32_init();
	for(int i = 0; i<1000;i++)
	{
		isCRC = crc32_update(1000-i,isCRC);
	}
	ASSERT_EQ(isCRC,368465167);
}
// crc32_update (unsigned int Data, unsigned int Crc32)
TEST(CRC,CRC32_Buff5)
{
	uint32_t isCRC = crc32_init();
	for(int i = 0; i<sizeof(buff5)/sizeof(int);i++)
	{
		isCRC = crc32_update(buff5[i],isCRC);
	}
	ASSERT_EQ(isCRC,3177257216);
}

