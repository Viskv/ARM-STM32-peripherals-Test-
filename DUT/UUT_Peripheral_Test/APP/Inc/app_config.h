/**
 * @file    app_config.h
 * @brief   Project-wide configuration — handles, pin aliases, and constants.
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "main.h"       // stm32f7xx_hal.h
#include "cmsis_os.h"   // CMSIS-OS v2 / FreeRTOS
#include "usart.h"  	// huart3, huart7, huart8
#include "spi.h"    	// hspi1, hspi3
#include "i2c.h"    	// hi2c1, hi2c2
#include "adc.h"    	// hadc1
#include "tim.h"    	// htim2

// ----------------- General -----------------
#define TEST_TIMEOUT_MS 	5000U   // Per-peripheral test timeout
#define MAX_BIT_PATTERN_LEN 255U    // Protocol maximum

// ----------------- LED helpers -----------------
#define LED_GREEN_ON()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,  GPIO_PIN_SET)
#define LED_GREEN_OFF()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,  GPIO_PIN_RESET)
#define LED_BLUE_ON()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,  GPIO_PIN_SET)
#define LED_BLUE_OFF()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,  GPIO_PIN_RESET)
#define LED_RED_ON()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET)
#define LED_RED_OFF()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET)

// ----------------- Debug UART (USART3 → ST-LINK Virtual COM Port, PD8/PD9) -----------------
#define UART_DEBUG                  (&huart3)

// ----------------- UART loopback pair (UART8-TX→UART7-RX  and  UART7-TX→UART8-RX) -----------------
#define UART8_H (&huart8)   // Send the test pattern
#define UART7_H (&huart7)   // Echo the pattern back
#define UART_CRC_THRESHOLD 100U // Use CRC above this length

// ----------------- SPI loopback pair (SPI1 master, SPI3 slave) -----------------
#define SPI_MASTER (&hspi1)
#define SPI_SLAVE  (&hspi3)
#define SPI_CS_GPIO_PORT GPIOD 		 // Software CS for SPI1 master Port = D
#define SPI_CS_GPIO_PIN  GPIO_PIN_14 // Software CS for SPI1 master Pin = 14
#define SPI_CS_ZERO() HAL_GPIO_WritePin(SPI_CS_GPIO_PORT, SPI_CS_GPIO_PIN, GPIO_PIN_RESET)
#define SPI_CS_ONE()  HAL_GPIO_WritePin(SPI_CS_GPIO_PORT, SPI_CS_GPIO_PIN, GPIO_PIN_SET)

// ----------------- I2C loopback pair (I2C1 master, I2C2 slave) -----------------
#define I2C_MASTER     (&hi2c1)
#define I2C_SLAVE  	   (&hi2c2)
#define I2C_SLAVE_ADDR (0x30U << 1) // 7-bit slave address 0x30, left-shifted by 1

// ----------------- ADC -----------------
#define ADC_HANDLE 	  (&hadc1)
#define ADC_REF_VALUE 0U  // ToDo: Mesure and Change
#define ADC_TOLERANCE 50U // Allowed deviation from ADC_REFERENCE_VALUE (in raw counts, 12-bit = 0–4095)

// ----------------- Timer -----------------
#define TIMER_HANDLE 		(&htim2)
#define TIMER_PRESCALER_VAL 215U 	 // TIM2 1 MHz tick rate from 216 MHz SYSCLK. -> PSC = (216 000 000 / 1 000 000) - 1 = 215
#define TIMER_WINDOW_MS 	100U 	 // Measurement window in ms; expected TIM2 count = TIMER_WINDOW_MS * 1000
#define TIMER_TOLERANCE_PERCENT 2U 	 // deviation in '%' for timer accuracy test

// -----------------Networking
#define UDP_LISTEN_PORT 7000U

#endif /* APP_CONFIG_H */
