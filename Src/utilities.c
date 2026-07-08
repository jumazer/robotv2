/*
 * utilities.c
 *
 *  Created on: Jul 7, 2026
 *      Author: julius
 */

#include <string.h>

#define UPPER_A 65
#define LOWER_Z 90
#define LOWER_CASE_ASCII_DISTANCE 32

/*
 * Lowers all upper case characters
 */
void to_lower(char* str) {
	int len = strlen(str);
	for(int i = 0; i < len; i++) {
		char c = str[i];
		if(c >= UPPER_A && c <= LOWER_Z) {
			str[i] = c + LOWER_CASE_ASCII_DISTANCE;
		}
	}
}
