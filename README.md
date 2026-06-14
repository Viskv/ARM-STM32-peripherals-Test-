# ARM-STM32-peripherals-Test-

A FreeRTOS-based hardware verification suite for the **NUCLEO-F756ZG** (STM32F756ZG). 
A Linux/Win PC application sends test commands over UDP/Ethernet to the board, 
which runs the requested peripheral test and reports pass/fail.

| | |
|---|---|
| **Target** | NUCLEO-F756ZG (STM32F756ZG, Cortex-M7 @ 216 MHz) |
| **RTOS** | FreeRTOS (CMSIS-OS v2) |
| **Toolchain** | STM32CubeMX + STM32CubeIDE 1.19.0 |
| **Peripherals tested** | UART · SPI · I2C · ADC · Timer · Ethernet (MAC & PHY) |

---
 
## Default Network Configuration
 
| Setting | Value |
|---|---|
| UUT static IP | `192.168.1.200` |
| UUT UDP port | `7000` |
| Netmask | `255.255.255.0` |
 
The PC and the board must be on the same subnet (via a direct cable or a switch).
Change the UUT IP in CubeMX's LwIP settings and `UDP_LISTEN_PORT` in
`app_config.h` if needed — the PC server takes the IP/port as command-line
arguments.
 
---

## Status LEDs (UUT)
 
| LED | Meaning |
|---|---|
| Blue | Test in progress |
| Green | Last result: `SUCCESS` |
| Red | Last result: `FAILURE` |
