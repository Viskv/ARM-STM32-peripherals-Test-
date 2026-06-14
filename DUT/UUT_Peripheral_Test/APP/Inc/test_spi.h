/*
 * @file	test_spi.h
 * @brif	SPI full-duplex loopback test; SPI1 Master <-> SPI3 (Slave)
 */

#ifndef TEST_SPI_H_
#define TEST_SPI_H_

#include "protocol.h"

uint8_t Test_SPI(const TestCommand_t *cmd);

#endif /* TEST_SPI_H_ */
