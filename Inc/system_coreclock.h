/*
 * system_coreclock.h
 *
 *  Created on: Dec 14, 2025
 *      Author: Amal
 */

#ifndef SYSTEM_CORECLOCK_H_
#define SYSTEM_CORECLOCK_H_


#include <stdint.h>

extern uint32_t SystemCoreClock;

void SystemCoreClockUpdate(void);
#endif /* SYSTEM_CORECLOCK_H_ */
