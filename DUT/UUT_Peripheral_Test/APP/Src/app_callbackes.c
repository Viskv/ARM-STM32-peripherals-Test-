/*
 * @file	app_callbackes.c
 * @brif	Overrides for HAL weak DMA/IT completion callbacks
 */


#include "app_callbackes.h"
#include "app_config.h"

osSemaphoreId_t hUart8RXSem    = NULL;
osSemaphoreId_t hUart7RXSem    = NULL;
osSemaphoreId_t hSpiMasterSem  = NULL;
osSemaphoreId_t hSpiSlaveSem   = NULL;
osSemaphoreId_t hI2CSlaveRXSem = NULL;
osSemaphoreId_t hI2CSlaveTXSem = NULL;

// ---------------------------------------------------------------------

void App_Callbacks_Init(void)
{
	hUart8RXSem    = osSemaphoreNew(1U, 0U, NULL);
	hUart7RXSem    = osSemaphoreNew(1U, 0U, NULL);
	hSpiMasterSem  = osSemaphoreNew(1U, 0U, NULL);
	hSpiSlaveSem   = osSemaphoreNew(1U, 0U, NULL);
	hI2CSlaveRXSem = osSemaphoreNew(1U, 0U, NULL);
	hI2CSlaveTXSem = osSemaphoreNew(1U, 0U, NULL);
}

// ---------------------------------------------------------------------

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == UART8) {
        osSemaphoreRelease(hUart8RXSem);
    } else if (huart->Instance == UART7) {
        osSemaphoreRelease(hUart7RXSem);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    HAL_UART_DMAStop(huart);
}

// ---------------------------------------------------------------------

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1) {
        osSemaphoreRelease(hSpiMasterSem);
    } else if (hspi->Instance == SPI3) {
        osSemaphoreRelease(hSpiSlaveSem);
    }
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    (void)hspi;
}

// ---------------------------------------------------------------------

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C2) {
        osSemaphoreRelease(hI2CSlaveRXSem);
    }
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C2) {
        osSemaphoreRelease(hI2CSlaveTXSem);
    }
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    (void)hi2c;
}
