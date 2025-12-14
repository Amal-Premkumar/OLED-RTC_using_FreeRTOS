/*
 * system_coreclock.c
 *
 *  Created on: Dec 14, 2025
 *      Author: Amal
 */

#include <stdint.h>


uint32_t SystemCoreClock = 16000000U;

void SystemCoreClockUpdate(void)
{
    SystemCoreClock = 16000000U;
}
