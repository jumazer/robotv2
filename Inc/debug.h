/*
 * debug.h
 *
 *  Created on: May 25, 2026
 *      Author: julius
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>

#ifdef DEBUG
  #define DBG_PRINTF(...) printf(__VA_ARGS__)
#else
  #define DBG_PRINTF(...) do { } while(0)
#endif

#endif /* DEBUG_H_ */
