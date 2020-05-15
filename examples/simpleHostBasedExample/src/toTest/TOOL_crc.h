/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: AGPL-3.0-or-later
 * @author: Sebastian Balz
 */

#ifndef TOOL_CRC_H_
#define TOOL_CRC_H_


extern unsigned short crc_update (unsigned short crc, unsigned char data);
extern unsigned short crc16(const void* data, unsigned short cnt);
extern unsigned short updateCrc16(unsigned short crc, void * data, unsigned short cnt);

extern unsigned char crc8(const void *vptr, int len);
extern unsigned short crc8_update(unsigned char crc, unsigned char data);

extern unsigned int crc32_init(void);
extern unsigned int crc32_update (unsigned int Data, unsigned int Crc32);


#endif /* TOOL_CRC_H_ */
