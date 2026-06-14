/*
 * @file protocol.h
 * @brif
 *
 * Command frame (PC → UUT):
 *   [test_id : 4 B][peripheral : 1 B][iterations : 1 B]
 *   [pattern_len : 1 B][pattern : pattern_len B]
 *
 * Result frame (UUT → PC):
 *   [test_id : 4 B][result : 1 B]
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include <stdint.h>

// -----------Peripheral ID-----------
#define PERIPHERAL_Timer 	0x01U	// TIM2 Accurecy
#define PERIPHERAL_UART	 	0x02U	// UART7 <-> UART8 loopback
#define PERIPHERAL_SPI 	 	0x04U	// SPI1 Master <-> SPI3 (Slave)
#define PERIPHERAL_I2C 	 	0x08U	// I2C1 Maste <-> I2C2 Slave
#define PERIPHERAL_ADC 	 	0x10U	// ADC1
#define PERIPHERAL_OTA 	 	0x20U	// bonus OTA FW Update

// ----------- Results Codes -----------
#define TEST_PASS 0x01U
#define TEST_FAIL 0xFFU

// ----------- Frame Size -----------
#define PROTOCOL_MAX_PATTERN_LEN 255U
#define PROTOCOL_CMD_LEN		 7U // [Test ID]+[peripheral]+[iterations]+[pattern_len] = [4B]+[1B]+[1B]+[1B]
#define PROTOCOL_RESULT_LEN		 5U // [Test ID]+[Result]

// ----------- OTA op-code -----------
#define OTA_OPCODE_START 0x01U	// Erase flash sectors
#define OTA_OPCODE_DATA	 0x02U	// Write to Flash
#define OTA_OPCODE_END	 0x03U	// Signals the end of the transfer and triggers integrity checks (CRC)

// -----------Stracts-----------

typedef struct __attribute__((packed))
{
	uint32_t test_id;		// uniq Test id
	uint8_t	 peripheral;	// which periperal to test
	uint8_t	 iterations;	// num of test iterations to run
	uint8_t	 pattern_len;	// len of bit pattern
	uint8_t	 pattern[PROTOCOL_MAX_PATTERN_LEN]; // Teast Data | FW Data Part
} TestCommand_t;

typedef struct __attribute__((packed))
{
	uint32_t test_id; // uniq Test id
	uint8_t	 result;
} TestResult_t;


#endif /* PROTOCOL_H_ */
