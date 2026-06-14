/*
 * @file test_i2c.c
 * @brif I2C loopbac test
 *
 * 			I2C1 master; I2C2 Slave (Interapt mode)
 *
 * 			part1 (Master -> Slave):
 * 			* Slave IT Recive
 * 			* Master Transmit
 * 			* Slave callback conformation end of transmit
 * 			* verfy Slave recive pattern
 *
 * 			part2 (Slave -> Master):
 * 			* Slave IT Transmit
 * 			* Master Recive
 * 			* callback end of transmit of Slave
 * 			* verfy Master Reciver againts the original data
 */


#include "test_i2c.h"
#include "app_config.h"
#include "app_callbackes.h"

#include <stdio.h>
#include <string.h>

static uint8_t slave_rx[MAX_BIT_PATTERN_LEN];  // I2C2 RX Data Holder
static uint8_t master_rx[MAX_BIT_PATTERN_LEN]; // I2C1 RX Data Holder

// ----------- Test iteration -----------
static uint8_t run_iteration(const uint8_t *pattern,uint8_t len)
{
	// ---------Part 1 (Master -> Slave)---------
	printf("- Start  part_1(Master -> Slave) transmition\r\n");
	if(HAL_I2C_Slave_Receive_IT(I2C_SLAVE, slave_rx, len) != HAL_OK)
	{
		printf("[I2C][FAIL] I2C Slave RX_IT start Fail\r\n");
		return TEST_FAIL;
	}

	if(HAL_I2C_Master_Transmit(I2C_MASTER, I2C_SLAVE_ADDR, (uint8_t*)pattern, len, TEST_TIMEOUT_MS) != HAL_OK)
	{
		printf("[I2C][FAIL] I2C Master TX Fail\r\n[FAIL] (HAL Error 0x%1X)\r\n",(uint8_t)HAL_I2C_GetError(I2C_MASTER));
		return TEST_FAIL;
	}

	if(osSemaphoreAcquire(hI2CSlaveRXSem, 100U) != osOK)
	{
		printf("[I2C][FAIL] I2C Slave osSemaphore Timeout Fail\r\n");
		return TEST_FAIL;
	}

	printf("- End part1(Master -> Slave) transmition\r\n");
	printf("- Part_1 transmition data validation\r\n");
	if(memcpy(slave_rx,pattern,len) != 0)
	{
		printf("[I2C][FAIL] I2C Slave RX Data Validetion Fail\r\n");
		return TEST_FAIL;
	}

	// ---------Part 2 (Slave -> Master)---------
	printf("- Start  part_2(Slave -> Master) transmition\r\n");
	if(HAL_I2C_Slave_Transmit_IT(I2C_SLAVE, slave_rx, len) != HAL_OK)
	{
		printf("[I2C][FAIL] I2C Slave TX IT Fail\r\n");
		return TEST_FAIL;
	}

	if(HAL_I2C_Master_Receive(I2C_MASTER, I2C_SLAVE_ADDR, master_rx, len, TEST_TIMEOUT_MS) != HAL_OK)
	{
		printf("[I2C][FAIL] I2C Master RX Fail\r\n[FAIL] (HAL Error 0x%1X)\r\n",(uint8_t)HAL_I2C_GetError(I2C_MASTER));
		return TEST_FAIL;
	}

	if(osSemaphoreAcquire(hI2CSlaveTXSem, 100U) != osOK)
	{
		printf("[I2C][FAIL] I2C Slave osSemaphore TX Timeout Fail\r\n");
		return TEST_FAIL;
	}
	printf("- End part2(Slave -> Master) transmition\r\n");
	printf("- Part_2 transmition data validation\r\n");
	if(memcpy(master_rx,pattern,len) != 0)
	{
		printf("[I2C][FAIL] I2C Maste RX Data Validetion Fail\r\n");
		return TEST_FAIL;
	}

	return TEST_PASS;
}

// ----------- Main Test  -----------

uint8_t Test_I2C(const TestCommand_t *cmd)
{
	// check Command Validity for test start
	if(cmd->pattern_len == 0U || cmd->iterations == 0U)
	{
		printf("[I2C][FAIL] Pattern len is zero or number of iterations is zero\r\n");
		return TEST_FAIL;
	}

	// Start Iterations tests
	printf("---------Start I2C Loopback Test---------\r\n");
	for(uint8_t i = 0U; i < cmd->iterations; i++)
	{
		uint8_t res = run_iteration(cmd->pattern, cmd->pattern_len);
		if(res != TEST_PASS)
		{
			printf("[I2C][FAIL] Test Fail on Iteration %u#\r\n",i+1U);
			return TEST_FAIL;
		}
		printf("-----\r\n");
	}
	printf("[I2C][PASS] All %u# iterations have Passed\r\n", cmd->iterations);
	printf("**************************************************\r\n");
	return TEST_PASS;
}





