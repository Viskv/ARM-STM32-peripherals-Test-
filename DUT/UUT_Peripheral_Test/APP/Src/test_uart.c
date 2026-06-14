/*
 * @file test_uart.c
 * @brif UART loopback test
 *
 * 			UART8TX -> UART7RX than UART7TX -> Uart8RX
 *
 * 			test sequnce:
 * 			* init UART8 RX DMA
 * 			* init UART7 RX DMA
 * 			* Start UART8 TX DMA
 * 			* wait to end of transfer with hUart7RXSem
 * 			* Start UART7 TX DMA
 * 			* wait to end of transfer with hUart8RXSem
 * 			* Data Validetion with CRC32 or memcpy
 *
 * 			in case of fail mid run: (abort())
 * 			* Stop DMA
 * 			* flash the FreeRtos osSemaphore
 */

#include "test_uart.h"
#include "app_config.h"
#include "app_callbackes.h"
#include "tools.h" // for CRC32

#include <stdio.h>
#include <string.h>

static uint8_t uart8_RX_buff[MAX_BIT_PATTERN_LEN];
static uint8_t uart7_RX_buff[MAX_BIT_PATTERN_LEN];

// ----------- Utility tools -----------

static void abort()
{
	HAL_UART_DMAStop(UART8_H);
	HAL_UART_DMAStop(UART7_H);
	osSemaphoreAcquire(hUart8RXSem, 0U);
	osSemaphoreAcquire(hUart7RXSem, 0U);
}

static int buffer_compare(const uint8_t *sent, const uint8_t *received, uint8_t len)
{
    if (len > UART_CRC_THRESHOLD) {
        return (Tools_CRC32(sent, len) == Tools_CRC32(received, len)) ? 1 : 0;
    }
    return (memcmp(sent, received, len) == 0) ? 1 : 0;
}

// ----------- Test iteration -----------

static uint8_t run_iteration(const uint8_t *pattern,uint8_t len)
{
	// init recivers
	printf("[UART][INFO]- DMA INIT\r\n");
	if (HAL_UART_Receive_DMA(UART8_H, uart8_RX_buff, len) != HAL_OK)
	{
		printf("[UART][FAIL] UART8 DMA Reciver INIT Fail\r\n");
		abort();
		return TEST_FAIL;
	}

	if (HAL_UART_Receive_DMA(UART7_H, uart7_RX_buff, len) != HAL_OK)
	{
		printf("[UART][FAIL] UART7 DMA Reciver INIT Fail\r\n");
		abort();
		return TEST_FAIL;
	}

	// Trasmit UART8 -> UART7
	printf("[UART][INFO]- Send UART8 -> UART7\r\n");
	if(HAL_UART_Transmit_DMA(UART8_H, (uint8_t *)pattern, len) != HAL_OK)
	{
		printf("[UART][FAIL] UART8 -> UART7 Transmit Fail\r\n");
		abort();
		return TEST_FAIL;
	}

	// Wait for UART7 to recive data
	printf("[UART][INFO]- Wait for UART7 to recive data\r\n");
	if(osSemaphoreAcquire(hUart7RXSem, TEST_TIMEOUT_MS) != osOK)
	{
		printf("[UART][FAIL] UART7 RX Timeout\r\n");
		abort();
		return TEST_FAIL;
	}

	// Send back from UART7 -> UART8
	printf("[UART][INFO]- Send UART7 -> UART8\r\n");
	if(HAL_UART_Transmit_DMA(UART7_H, uart7_RX_buff, len) != HAL_OK)
	{
		printf("[UART][FAIL] UART7 -> UART8 Fail\r\n");
		abort();
		return TEST_FAIL;
	}

	// Wait for UART8 to recive data
	printf("[UART][INFO]- Wait for UART8 to recive data\r\n");
	if(osSemaphoreAcquire(hUart8RXSem, TEST_TIMEOUT_MS) != osOK)
	{
		printf("[UART][FAIL] UART8 RX Timeout\r\n");
		abort();
		return TEST_FAIL;
	}

	// Data Validetion
	printf("[UART][INFO]- Start Data Validetion\r\n");
	if(!buffer_compare(pattern, uart8_RX_buff, len))
	{
		printf("[UART][FAIL] Data Validation Fail\r\n");
		return TEST_FAIL;
	}

	return TEST_PASS;
}

// ----------- Main Test  -----------

uint8_t Test_UART(const TestCommand_t *cmd)
{
	// check Command Validity for test start
	if(cmd->pattern_len == 0U || cmd->iterations == 0U)
	{
		printf("[UART][FAIL] Pattern len is zero or number of iterations is zero\r\n");
		return TEST_FAIL;
	}

	// Start Iterations tests
	printf("[UART][INFO]---------Start UART Loopback Test---------\r\n");
	for(uint8_t i = 0U; i < cmd->iterations; i++)
	{
		printf("[UART][INFO]-----\r\n");
		printf("[UART][INFO]- Iteration %u#/%u#",i+1U,cmd->iterations);

		uint8_t res = run_iteration(cmd->pattern, cmd->pattern_len);
		if(res != TEST_PASS)
		{
			printf("[UART][FAIL] Test Fail on Iteration %u#\r\n",i+1U);
			return TEST_FAIL;
		}
	}
	printf("[UART][PASS] All %u# iterations have Passed\r\n", cmd->iterations);
	printf("[UART][INFO]**************************************************\r\n");
	return TEST_PASS;
}
