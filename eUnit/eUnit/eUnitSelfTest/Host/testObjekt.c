/**
 * Copyright (C) 2020 Intel Corporation
 * SPDX-License-Identifier: Apache-2.0
 * @author: Sebastian Balz
 */
#include <stdint.h>
uint8_t error_code = 0;

/**
 * access the Hardware.
 * or maybe not
 */
uint8_t funktionWithDep(){
	printOverGdb("waste some time ... \n");
	for(int x = 1; x<= 10;x++){
		printOverGdb(".");
		for(int i = 1; i<= 1000000;i++){
			if(i == -1)
				printOverGdb(".");
		}
	}
	printOverGdb(" ... this took a long time now :)\n");
}


/**
 *  evaluate the GPIO State
 */
int evaluateGPIO(){
	switch(funktionWithDep()){
		case 0: return 0;
		case 1 - 254 : return 1;
		case 255 : if (error_code)
						return -2 ;
					else
						return -1;
	}
}
