/*
 * @file	test_select_ctrl.c
 * @brif	test selction and ctrl FreeRTOS Task
 *
 *			Task sequnce:
 *			* Create cmd empty param to get data  <-|
 *			* get the data to cmd			 		|
 *			* Run Test based on Test_id		 		|
 *			* send res back	----------------------->|
 */

#include "test_select_ctrl.h"
#include "udp_handller.h"
#include "app_config.h"
#include "protocol.h"
#include "test_adc.h"
#include "test_i2c.h"
#include "test_spi.h"
#include "test_timer.h"
#include "test_uart.h"
#include <stdio.h>

void TestSelctCtrlTask(void *args)
{
	(void)args; // ignor args as we do not use it

	while(1)
	{
		TestCommand_t cmd;
		uint8_t test_status= 0xFE;

		// until data is recived from ETH PORT the task will sleep, other task will run while on sleep
		// cmd recive the data by copy
		// msg_prio = NULL - only use 1 main task to get data from UDP Server
		if(osMessageQueueGet(hTestCmdQue, &cmd, NULL, osWaitForever) != osOK)
		{
			continue; // reset Task in case of ERROR from osMessageQueueGet, save from crash
		}

		LED_BLUE_ON(); // Massage recived Start Test

		switch (cmd.peripheral)
		{
			case PERIPHERAL_Timer:
				test_status = Test_Timer(&cmd);
				break;
			case PERIPHERAL_UART:
				test_status = Test_UART(&cmd);
				break;
			case PERIPHERAL_SPI:
				test_status = Test_SPI(&cmd);
				break;
			case PERIPHERAL_I2C:
				test_status = Test_I2C(&cmd);
				break;
			case PERIPHERAL_ADC:
				test_status = Test_ADC(&cmd);
				break;
			default:
				printf("[Tests_CTRL] Unknown peripheral 0x%02X\r\n",cmd.peripheral);
				test_status = TEST_FAIL;
				break;
		}

		LED_BLUE_OFF(); // End Test

		if(TEST_PASS == test_status)
		{
			LED_GREEN_ON();
			LED_RED_OFF();
		}
		else if (TEST_FAIL == test_status)
		{
			LED_GREEN_OFF();
			LED_RED_ON();
		}
		else // in case of ERROR
		{
			LED_GREEN_ON();
			LED_RED_ON();
		}

		TestResult_t res;
		res.result = test_status;
		res.test_id= cmd.test_id;

		//Send Result back throw UDP
		osMessageQueuePut(hTestResQue, &res, 0U, osWaitForever);
	}
}
