#include "protocol.h"
#include "udp_client.h"
#include "test_recorder.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define TARGET_UUT_IP   "192.168.1.200"
// #define TARGET_UUT_IP   "192.168.1.10"
#define TARGET_UUT_PORT 7000

// Helper to validate peripheral ID
int is_valid_peripheral(uint8_t p) 
{
    return (p == 0x01 || p == 0x02 || p == 0x04 || p == 0x08 || p == 0x10 || p == 0x20);
}

// Parse hex string (e.g., "DEADBEEF") into raw bytes
size_t parse_hex_string(const char *hex_str, uint8_t *output, size_t max_len) 
{
    size_t count = 0;
    while (*hex_str && *hex_str != '\n' && *hex_str != '\r' && *(hex_str + 1) && count < max_len) 
    {
        char byte_str[3] = {hex_str[0], hex_str[1], '\0'};
        output[count++] = (uint8_t)strtol(byte_str, NULL, 16);
        hex_str += 2;
    }
    return count;
}

void print_menu(void) 
{
    printf("\n==================================\n");
    printf("      UUT COMMAND INTERFACE\n");
    printf("==================================\n");
    printf(" Target Peripherals:\n");
    printf("    1 : Timer        8 : I2C\n");
    printf("    2 : UART        10 : ADC\n");
    printf("    4 : SPI         20 : OTA\n");
    printf("  q/Q : quit\n");
    printf("==================================\n");
}

// Logic shared by both CLI args and manual input
void run_test(uint8_t peripheral, uint8_t iterations, const char *pattern_hex) 
{
    TestCommand_t cmd = {0};
    cmd.test_id     = rand() % 90000 + 10000;
    cmd.peripheral  = peripheral;
    cmd.iterations  = iterations;
    cmd.pattern_len = (uint8_t)parse_hex_string(pattern_hex, cmd.pattern, PROTOCOL_MAX_PATTERN_LEN);

    TestResult_t response = {0};
    int transport_status = -1;

    printf("\n[+] Dispatching Test ID 0x%08X to Peripheral 0x%02X...\n", cmd.test_id, cmd.peripheral);
    
    if (udp_sendCmd(&cmd) == 0) 
    {
        printf("[+] Listening for hardware execution response...\n");
        transport_status = udp_recvRes(&response, UDP_RECV_TIMEOUT_Sec);
    } 
    else 
    {
        printf("[-] Network transmission layer error encountered.\n");
    }

    recorder_logToConsole(&cmd, &response, transport_status);
    recorder_logToFile("test_results.csv", &cmd, &response, transport_status);
}

int main(int argc, char *argv[]) 
{
    if (udp_init(TARGET_UUT_IP, TARGET_UUT_PORT) != 0) 
    {
        printf("[-] Failed to initialize UDP client.\n");
        return 1;
    }
    printf("[+] Socket successfully opened for UUT at %s:%d\n", TARGET_UUT_IP, TARGET_UUT_PORT);
    
    srand((unsigned int)time(NULL));

    if (argc == 4) 
    {
        uint8_t p = (uint8_t)strtol(argv[1], NULL, 16);
        uint8_t i = (uint8_t)atoi(argv[2]);
        if (!is_valid_peripheral(p) || i == 0) {
            printf("[-] Error: Invalid CLI arguments.\n");
            return 1;
        }
        run_test(p, i, argv[3]);
    } 
    else 
    {
        char buf[PROTOCOL_MAX_PATTERN_LEN]; 
        while (1) 
        {
            print_menu();
            
            // Peripheral
            printf("Enter Peripheral ID (or 'q'): ");
            if (!fgets(buf, sizeof(buf), stdin) || buf[0] == 'q' || buf[0] == 'Q') 
            {
                break;
            }

            uint8_t peripheral = (uint8_t)strtol(buf, NULL, 16);
            if (!is_valid_peripheral(peripheral)) 
            { 
                printf("[-] Invalid Peripheral ID.\n"); 
                continue; 
            }

            // Iterations
            printf("Enter iterations (decimal): ");
            if (!fgets(buf, sizeof(buf), stdin)) 
            {
                break;
            }

            int i = atoi(buf);
            if (i <= 0) 
            { 
                printf("[-] Invalid iteration count.\n"); 
                continue; 
            }

            // Pattern
            printf("Enter pattern in hex (e.g., DEADBEEF): ");
            if (!fgets(buf, sizeof(buf), stdin)) 
            {
                break;
            }
            
            run_test(peripheral, (uint8_t)i, buf);
        }
    }

    printf("[+] Closing network connections.\n");
    udp_close();
    return 0;
}