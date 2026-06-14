/*
 * @file	test_timer.c
 * @brif	TIM2 accuracy test
 *
 * 			TIM2 Configured to 1MHz Rate
 * 			test sequnce:
 * 			* reset counter
 * 			* start TIM2
 * 			* use osDelay for TIMER_WINDOW_MS = 100[msec]
 * 			* read TIM2 counter
 * 			* stop TIM2
 * 			* Compare Expected counter VS mesured counter
 * 			  (Expected count = TIMER_WINDOW_MS × 1000 = 100 000.
 *             Tolerance = ±TIMER_TOLERANCE_PERCENT % = ±2 000 count)
 */

#include "test_timer.h"
#include "app_config.h"
#include <stdio.h>

const static uint32_t timer_expected_res = TIMER_WINDOW_MS * 1000UL;
const static uint32_t timer_tolorence = (timer_expected_res * TIMER_TOLERANCE_PERCENT)/100UL;
// ----------- Test iteration -----------

static uint8_t run_iteration()
{
	uint32_t diff = 0xFFFFFFFFUL;
	__HAL_TIM_SET_PRESCALER(TIMER_HANDLE, TIMER_PRESCALER_VAL);
	__HAL_TIM_SET_COUNTER(TIMER_HANDLE, 0U);

	HAL_TIM_Base_Start(TIMER_HANDLE);

	osDelay(TIMER_WINDOW_MS);

	uint32_t ret = __HAL_TIM_GET_COUNTER(TIMER_HANDLE);

	HAL_TIM_Base_Stop(TIMER_HANDLE);

	if (ret > timer_expected_res) {diff = ret - timer_expected_res;}
	else {diff = timer_expected_res - ret;}

	if(diff > timer_tolorence)
	{
		printf("[TIMER][FAIL] Counter= %lu ; Expected= %lu | diff = %lu ; tolorance= %lu \r\n",ret,timer_expected_res,diff,timer_tolorence);
		return TEST_FAIL;
	}
	else {printf("[TIMER][PASS] Counter= %lu ; Expected= %lu | diff = %lu ; tolorance= %lu \r\n",ret,timer_expected_res,diff,timer_tolorence);}

	return TEST_PASS;
}

// ----------- Main Test  -----------

uint8_t Test_Timer(const TestCommand_t *cmd)
{
	// check Command Validity for test start
	if(cmd->iterations == 0U)
	{
		printf("[TIMER][FAIL] number of iterations is zero\r\n");
		return TEST_FAIL;
	}

	// Start Iterations tests
	printf("[TIMER][INFO]---------Start TIMER Loopback Test---------\r\n");
	for(uint8_t i = 0U; i < cmd->iterations; i++)
	{
		printf("[TIMER][INFO]-----\r\n");
		printf("[TIMER][INFO]- Iteration %u#/%u#",i+1U,cmd->iterations);

		if(TEST_PASS != run_iteration())
		{
			printf("[TIMER][FAIL] Test Fail on Iteration %u#\r\n",i+1U);
			return TEST_FAIL;
		}
	}
	printf("[TIMER][PASS] All %u# iterations have Passed\r\n", cmd->iterations);
	printf("[TIMER][INFO]**************************************************\r\n");
	return TEST_PASS;
}
