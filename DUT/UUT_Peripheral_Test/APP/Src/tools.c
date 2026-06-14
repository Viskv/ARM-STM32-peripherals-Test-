/*
 * @ tools.c
 * @brif printf/scanf redairection to UART Debug (USART3) software CRC-32
 */

#include "tools.h"
#include "app_config.h"

static osMutexId_t print_mutex = NULL; // FreeRTOS flag for using printf only one function at a time

void Tools_Init()
{
	print_mutex = osMutexNew(NULL);
}

//CRC32 -
uint32_t Tools_CRC32(const uint8_t *data, size_t len)
{
    uint32_t crc = 0xFFFFFFFFU;

    for (size_t i = 0U; i < len; i++)
    {
        crc ^= (uint32_t)data[i];
        for (int b = 0; b < 8; b++)
        {
            /* Branchless: apply polynomial only when LSB is 1 */
        	//Magic for LSB machine
            crc = (crc >> 1U) ^ (0xEDB88320U & (uint32_t)(-(int32_t)(crc & 1U)));
        }
    }
    return ~crc;
}

// printf
int __io_putchar(int ch)
{
    if (print_mutex != NULL)
    {
        osMutexAcquire(print_mutex, osWaitForever);
    }
    HAL_UART_Transmit(UART_DEBUG, (uint8_t *)&ch, 1U, 0xFFFFU);
    if (print_mutex != NULL)
    {
        osMutexRelease(print_mutex);
    }
    return ch;
}

// printf
int _write(int file, char *ptr, int len)
{
    if (print_mutex != NULL)
    {
        osMutexAcquire(print_mutex, osWaitForever);
    }
    HAL_UART_Transmit(UART_DEBUG, (uint8_t *)ptr, (uint16_t)len, 0xFFFFU);
    if (print_mutex != NULL)
    {
        osMutexRelease(print_mutex);
    }
    return len;
}

// scanf
int _read(int file, char *ptr, int len)
{
	int ch=0;
	HAL_UART_Receive(UART_DEBUG, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
	HAL_UART_Transmit(UART_DEBUG, (uint8_t *)&ch, 1,HAL_MAX_DELAY);
	if(ch==13)
	{
		ch=10;
		HAL_UART_Transmit(UART_DEBUG, (uint8_t *)&ch, 1,HAL_MAX_DELAY);
	}
	else if(ch==8)
	{
		ch=0x30;
		HAL_UART_Transmit(UART_DEBUG, (uint8_t *)&ch, 1,HAL_MAX_DELAY);
	}

	*ptr=ch;

	return 1;
}
