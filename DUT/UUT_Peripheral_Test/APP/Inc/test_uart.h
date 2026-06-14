/*
 * @file	test_uart.h
 * @brif	UART loopback test; UART7 <-> UART8 loopback
 */

#ifndef TEST_UART_H_
#define TEST_UART_H_

#include "protocol.h"

uint8_t Test_UART(const TestCommand_t *cmd);

#endif /* TEST_UART_H_ */
