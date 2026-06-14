/*
 * @file test_spi.c
 * @brif SPI full-duplex loopback test
 *
 * 			Both SPI use DMA, the patter go from Master to Slave -> from Slave to Master
 *
 * 			sequnce:
 * 			* INIT Slave TX buffer to transmit to Master
 * 			* init Slave DMA to TransmitReceive data
 * 			* set CS to zero (select Slave)
 * 			* start TransmitReceive
 * 			* aquire end of DMA from Master and Slave
 * 			* Set CS to One (deselect Slave)
 * 			* transmited data validetion
 * 			in case of fail mid run: (abort())
 * 			* set CS to one
 * 			* Stop DMA
 * 			* flash the FreeRtos osSemaphore
 *
 */

#include "test_spi.h"
#include "app_config.h"
#include "app_callbackes.h"

#include <stdio.h>
#include <string.h>

static uint8_t slave_rx[MAX_BIT_PATTERN_LEN];  // SPI Slave RX Data Holder
static uint8_t slave_tx[MAX_BIT_PATTERN_LEN];  // SPI Slave TX Data Holder
static uint8_t master_rx[MAX_BIT_PATTERN_LEN]; // SPI Master RX Data Holder

// ----------- Utility tools -----------

static void abort()
{
	SPI_CS_ONE();
	HAL_SPI_DMAStop(SPI_MASTER);
	HAL_SPI_DMAStop(SPI_SLAVE);
	osSemaphoreAcquire(hSpiMasterSem, 0U);
	osSemaphoreAcquire(hSpiSlaveSem, 0U);
}

// ----------- Test iteration -----------

static uint8_t run_iteration(const uint8_t *pattern,uint8_t len)
{
	printf("- Slave DMA init\r\n");
	memcpy(slave_tx, pattern, len);

	if(HAL_SPI_TransmitReceive_DMA(SPI_SLAVE, slave_tx, slave_rx, len) != HAL_OK)
	{
		printf("[SPI][FAIL] Fail to init Slave DMA\r\n");
		return TEST_FAIL;
	}

	osDelayUntil(1U); // Make sure that master will not start befor Slave DMA is ready

	printf("- CS = 0 (Slave Selected)\r\n");
	SPI_CS_ZERO(); // Select slave

	printf("- Start TransmitRecive\r\n");
	if(HAL_SPI_TransmitReceive_DMA(SPI_MASTER, pattern, master_rx, len) != HAL_OK)
		{
			printf("[SPI][FAIL] fail in Maste TransmitReceive_DMA\r\n");
			abort();
			return TEST_FAIL;
		}

	osStatus_t masterState = osSemaphoreAcquire(hSpiMasterSem, TEST_TIMEOUT_MS);
	osStatus_t slaveState  = osSemaphoreAcquire(hSpiSlaveSem, TEST_TIMEOUT_MS);
	SPI_CS_ZERO(); // deselect Slave endTest

	printf("- Check if Test finished\r\n");
	if(masterState != osOK || slaveState != osOK)
	{
		printf("[SPI][FAIL] Test TimeOut (MasterState = %d | SlaveState = %d)\r\n",(int)masterState,(int)slaveState);
		abort();
		return TEST_FAIL;
	}

	// Data Validetion
	printf("- Check Data Validetion\r\n");
	if(memcpy(slave_rx,pattern,len) != 0)
	{
		printf("[SPI][FAIL] Slave RX Data Validetion Fail\r\n");
		return TEST_FAIL;
	}

	if(memcpy(master_rx,pattern,len) != 0)
	{
		printf("[SPI][FAIL] Master RX Data Validetion Fail\r\n");
		return TEST_FAIL;
	}

	return TEST_PASS;
}

// ----------- Main Test  -----------

uint8_t Test_SPI(const TestCommand_t *cmd)
{
	// check Command Validity for test start
	if(cmd->pattern_len == 0U || cmd->iterations == 0U)
	{
		printf("[SPI][FAIL] Pattern len is zero or number of iterations is zero\r\n");
		return TEST_FAIL;
	}

	// Start Iterations tests
	printf("---------Start SPI Loopback Test---------\r\n");
	for(uint8_t i = 0U; i < cmd->iterations; i++)
	{
		printf("-----\r\n");
		printf("- Iteration %u#/%u#",i+1U,cmd->iterations);

		uint8_t res = run_iteration(cmd->pattern, cmd->pattern_len);
		if(res != TEST_PASS)
		{
			printf("[SPI][FAIL] Test Fail on Iteration %u#\r\n",i+1U);
			return TEST_FAIL;
		}
	}
	printf("[SPI][PASS] All %u# iterations have Passed\r\n", cmd->iterations);
	printf("**************************************************\r\n");
	return TEST_PASS;
}

