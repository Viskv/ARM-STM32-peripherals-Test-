/*
 * @file tools.h
 * @brif printf/scanf redairection to UART Debug (USART3)
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <stdint.h>
#include <stddef.h>


// init the printf
void Tools_Init();

/*
 * cumpute CRC 32, check sum
 * input:	pointer to start of data| number of byets of data
 * return:	Calculeted sum of CRC 32
 */
uint32_t Tools_CRC32(const uint8_t *data, size_t len);

int __io_putchar(int ch);
int _write(int file, char *ptr, int len); // printf()
int _read(int file, char *ptr, int len);  // scanf()

#endif /* TOOLS_H_ */
