/*
 * @file udp_handller.h
 * @brif UdpServerTask listning on UDP_LISTEM_PORT to get TestCommand_t and send it to the tests_select_ctrl.
 * 		 in the end send TestResultes_t back to the original IP:port
 */

#ifndef UDP_HANDLLER_H_
#define UDP_HANDLLER_H_

#include "cmsis_os.h"
#include "protocol.h"

extern osMessageQueueId_t hTestCmdQue;// queue shard with test_selct_ctrl for TestCommand_t
extern osMessageQueueId_t hTestResQue;// queue shard with test_selct_ctrl for TestResult_t

void UdpServerTask(void *args); // UDP Reciver loop


#endif /* UDP_HANDLLER_H_ */
