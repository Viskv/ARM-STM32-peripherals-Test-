/*
 * @file app_callbackes.h
 * @brif
 */

#ifndef APP_CALLBACKES_H_
#define APP_CALLBACKES_H_

#include "cmsis_os.h"

// UART DMA Semaphore handlers
extern osSemaphoreId_t hUart8RXSem;
extern osSemaphoreId_t hUart7RXSem;

// SPI DMA Semaphore handlers
extern osSemaphoreId_t hSpiMasterSem;
extern osSemaphoreId_t hSpiSlaveSem;

// I2C IT Semaphore handlers
extern osSemaphoreId_t hI2CSlaveRXSem;
extern osSemaphoreId_t hI2CSlaveTXSem;


void App_Callbacks_Init(void); // Creat the Semaphore handllers


#endif /* APP_CALLBACKES_H_ */
