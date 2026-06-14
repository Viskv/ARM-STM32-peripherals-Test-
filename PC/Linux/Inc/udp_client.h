/*
 * @file    udp_client.h
 * @brif    UDP socket wrapper — send a TestCommand_t to the UUT and
 *          receive the TestResult_t response.
 *          UUT 192.168.1.200:7000
 */

#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include "protocol.h"
#include <stdint.h>

#define UDP_RECV_TIMEOUT_Sec 60 // Sec to wait before timeout is called

int udp_init(const char *ip, uint16_t port); // Open connection ; -1 = error, 0 = OK

int udp_sendCmd(const TestCommand_t *cmd); // -1 = error, 0 = OK

int udp_recvRes(TestResult_t *res, int timeout_sec); // -1 = error, 0 = OK

int udp_close(void); // close connection ; -1 = error, 0 = OK

#endif // UDP_CLIENT_H