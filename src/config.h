#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "compiler.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "at90can_drv.h"

// General
#define FOSC			16000			// 16 MHz External cristal
#define F_CPU			16000000UL		// Need for AVR GCC

// CAN
#define CAN_BAUDRATE   	100

#endif  // _CONFIG_H_


