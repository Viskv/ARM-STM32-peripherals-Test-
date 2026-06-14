/*
 * @file	test_adc.c
 * @brif	ADC accuracy test ADC1 channel 0
 *
 * 			test sequnce:
 * 			* start ADC
 * 			* read ADC Value
 * 			* check mesured Value agains expected Value
 *
 *
 * @pre
 * 			* PA0 must be connected to a known stable DC voltage before running this test.
 * 			* ADC_REFERENCE_VALUE in app_config.h must be set to the expected 12-bit count for that voltage
 *
 *
 */

#include "test_adc.h"
#include "app_config.h"
#include <stdio.h>

// ----------- Test iteration -----------
static uint8_t run_iteration()
{
	uint32_t diff = 0xFFFFFFFFUL;
	// start ADC
	HAL_ADC_Start(ADC_HANDLE);

	// wait till the convertion from analog to digital will end
	if(HAL_ADC_PollForConversion(ADC_HANDLE, TEST_TIMEOUT_MS) != HAL_OK)
	{
		HAL_ADC_Stop(ADC_HANDLE);
		printf("[ADC][FAIL] Conversion timeout\r\n");
		return TEST_FAIL;
	}

	// Get Value
	uint32_t measured_val = HAL_ADC_GetValue(ADC_HANDLE);
	HAL_ADC_Stop(ADC_HANDLE);

	// Compare to Expected Value
	if(measured_val > ADC_REF_VALUE) {diff = measured_val - ADC_REF_VALUE;}
	else{diff = ADC_REF_VALUE - measured_val;}

	if(diff > ADC_TOLERANCE)
	{
		printf("[ADC][FAIL] measured_ADC = %lu ; Expected_ADC = %u | measured_tolorance = %lu ; expected_tolorance = %u\r\n",measured_val,ADC_REF_VALUE,diff,ADC_TOLERANCE);
		return TEST_FAIL;
	}
	else {printf("[ADC][PASS] measured_ADC = %lu ; Expected_ADC = %u | measured_tolorance = %lu ; expected_tolorance = %u\r\n",measured_val,ADC_REF_VALUE,diff,ADC_TOLERANCE);}

	return TEST_PASS;
}

// ----------- Main Test  -----------

uint8_t Test_ADC(const TestCommand_t *cmd)
{
	// check Command Validity for test start
	if(cmd->iterations == 0U)
	{
		printf("[ADC][FAIL] number of iterations is zero\r\n");
		return TEST_FAIL;
	}

	// Start Iterations tests
	printf("[ADC][INFO]---------Start ADC Loopback Test---------\r\n");
	for(uint8_t i = 0U; i < cmd->iterations; i++)
	{
		printf("[ADC][INFO]-----\r\n");
		printf("[ADC][INFO]- Iteration %u#/%u#",i+1U,cmd->iterations);

		if(TEST_PASS != run_iteration())
		{
			printf("[ADC][FAIL] Test Fail on Iteration %u#\r\n",i+1U);
			return TEST_FAIL;
		}
	}
	printf("[ADC][PASS] All %u# iterations have Passed\r\n", cmd->iterations);
	printf("[ADC][INFO]**************************************************\r\n");
	return TEST_PASS;
}
