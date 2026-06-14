/*
 * @file	app_main.c
 * @brif
 */

#include "app_main.h"
#include "app_config.h"
#include "app_callbackes.h"
#include "udp_handller.h"
#include "test_select_ctrl.h"
#include "tools.h"
#include <stdio.h>

// ------------------------------------ Tasks Attributes  ------------------------------------

static const osThreadAttr_t k_udp_task_attr = {
    .name       = "UdpServerTask",
    .stack_size = 1024U * 4U,        // 4 KB — netconn API needs headroom
    .priority   = osPriorityAboveNormal,
};

static const osThreadAttr_t k_test_task_attr = {
    .name       = "TestSelctCtrlTask",
    .stack_size = 2048U * 4U,        // 8 KB — all test drivers share this stack
    .priority   = osPriorityNormal,
};

// ------------------------------------ Main function  ------------------------------------

void App_Main_Init()
{
	// Init Tools for printf competability
	Tools_Init();

	// Init callbacks DMA/IT
	App_Callbacks_Init();


	hTestCmdQue = osMessageQueueNew(1U, sizeof(TestCommand_t), NULL);
	hTestResQue = osMessageQueueNew(1U, sizeof(TestCommand_t), NULL);

	osThreadNew(UdpServerTask,    NULL, &k_udp_task_attr);
	osThreadNew(TestSelctCtrlTask, NULL, &k_test_task_attr);

	printf("[APP] Tasks created. Listening on UDP port %u\r\n",UDP_LISTEN_PORT);
}
