/*
 *  @file	udp_handller.c
 *	@brif	LwIP UDP Server FreeRTOS Task
 *
 *			FreeRTOS Task for handelling incoming masseges from outside,
 *
 * 		 	stop task untill data is arrive	      <------------|
 *		 	save address and port to send back replay		   |
 *			put recived data into the cmd struct			   |
 *			send cmd to TestSelectCtrl Task					   |
 *			wait till test end runing and get results		   |
 *			send results back via the saved port and address ->|
 */

#include "udp_handller.h"
#include "app_config.h"
#include "protocol.h"
#include "lwip/api.h"
#include <string.h>
#include <stdio.h>

// Handllers define here; extern defined in .h file
osMessageQueueId_t hTestCmdQue = NULL;
osMessageQueueId_t hTestResQue = NULL;

static void sendUdpResponse(struct netconn *conn, const TestResult_t *res, const ip_addr_t *addr, u16_t port)
{
	struct netbuf *outputBuff = netbuf_new();
	if (outputBuff != NULL)
	{
		void *data = netbuf_alloc(outputBuff, PROTOCOL_RESULT_LEN);
		if (data != NULL)
		{
			memcpy(data, res, PROTOCOL_RESULT_LEN);
			netconn_sendto(conn, outputBuff, addr, port);
		}
		netbuf_delete(outputBuff);
	}
}


void UdpServerTask(void *args)
{
	(void)args; // ignor args as we do not use it

	// -------------- create UDP connection --------------
	struct netconn *conn = netconn_new(NETCONN_UDP);
	if(conn == NULL)
	{

		osThreadExit();
		return;
	}
	// -------------- bind to listening port --------------
	if(netconn_bind(conn, IP_ADDR_ANY, UDP_LISTEN_PORT) != ERR_OK)
		{
			printf("[UDP][FAIL] bind to port %u failed\r\n",UDP_LISTEN_PORT);
			netconn_delete(conn);
			osThreadExit();
			return;
		}
	printf("[UDP][INFO] Listening on port %u\r\n",UDP_LISTEN_PORT);

	while(1)
	{
		// -------------- stop task untill data is arrive --------------
		struct netbuf *inputbuf = NULL;
		if(netconn_recv(conn, &inputbuf) != ERR_OK) {continue;}

		// -------------- save address and port to send back replay
		ip_addr_t client_addr = *netbuf_fromaddr(inputbuf);
		u16_t     client_port =  netbuf_fromport(inputbuf);

		// -------------- put recived data into the cmd struct --------------
		uint8_t all_data[PROTOCOL_CMD_LEN+PROTOCOL_MAX_PATTERN_LEN]; // where to put
		u16_t 	recivedMasgLen = netbuf_len(inputbuf);
		TestCommand_t cmd;
		memset(&cmd, 0, sizeof(TestCommand_t));

		if (recivedMasgLen <= (PROTOCOL_CMD_LEN+PROTOCOL_MAX_PATTERN_LEN) && recivedMasgLen > PROTOCOL_CMD_LEN )
		{
			netbuf_copy(inputbuf, all_data, recivedMasgLen);
			netbuf_delete(inputbuf); // delete dynamic allocation of input buffer
		}
		else
		{
			netbuf_delete(inputbuf); // delete dynamic allocation of input buffer
			printf("[UDP][FAIL] The recived data is to big or short; Input buff length = %u\r\n",recivedMasgLen);
			continue;
		}

		memcpy(&cmd.test_id, all_data, 4U);
		cmd.peripheral  = all_data[4];
		cmd.iterations  = all_data[5];
		cmd.pattern_len = all_data[6];

		if (0x20U < cmd.peripheral)
		{
			printf("[UDP][FAIL] Wrong Peripheral: 0x%02X\r\n",cmd.peripheral);
			TestResult_t err_res = { .test_id = cmd.test_id, .result = TEST_FAIL };
			sendUdpResponse(conn, &err_res, &client_addr, client_port);
			continue;
		}

		if (recivedMasgLen < (PROTOCOL_CMD_LEN + cmd.pattern_len))
		{
		    printf("[UDP][FAIL] Protocol Command pattern length mismatch\r\n");
		    TestResult_t err_res = { .test_id = cmd.test_id, .result = TEST_FAIL };
		    sendUdpResponse(conn, &err_res, &client_addr, client_port);
			continue;
		}
		else
		{
			memcpy(cmd.pattern, all_data + PROTOCOL_CMD_LEN, cmd.pattern_len);
			printf("[UDP][INFO] Test_ID=0x%08lX ; Peripheral=0x%02X ; Iterations=%u\r\n", cmd.test_id, cmd.peripheral, cmd.iterations);
		}

		// -------------- send cmd to TestSelectCtrl Task --------------
		osMessageQueuePut(hTestCmdQue, &cmd, 0U, osWaitForever);

		// -------------- wait till test end runing and get results --------------
		TestResult_t res;
		if(osMessageQueueGet(hTestResQue, &res, NULL, TEST_TIMEOUT_MS + 1000U) != osOK) // add extra time to responce
		{
			res.test_id = cmd.test_id;
			res.result  = TEST_FAIL;
			printf("[UDP][FAIL] Result timeout for ID=0x%08lX\r\n", cmd.test_id);
		}
		// -------------- send results back via the saved port and address --------------
		sendUdpResponse(conn, &res, &client_addr, client_port);

		printf("[UDP][%s] ID=0x%08lX\r\n", (res.result == TEST_PASS) ? "PASS" : "FAIL",res.test_id);
	}
}
