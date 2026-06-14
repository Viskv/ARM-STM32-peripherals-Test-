/*
 * @file	test_i2c.h
 * @brif	I2C loopback test; I2C1 Maste <-> I2C2 Slave
 */

#ifndef TEST_I2C_H_
#define TEST_I2C_H_

#include "protocol.h"

uint8_t Test_I2C(const TestCommand_t *cmd);

#endif /* TEST_I2C_H_ */
