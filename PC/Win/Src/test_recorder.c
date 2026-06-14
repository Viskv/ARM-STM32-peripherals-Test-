#include "test_recorder.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

static const char* get_peripheral_name(uint8_t peripheral) 
{
    switch(peripheral) 
    {
        case PERIPHERAL_Timer: return "TIMER (TIM2 Accuracy)";
        case PERIPHERAL_UART:  return "UART (UART7 <-> UART8 Loopback)";
        case PERIPHERAL_SPI:   return "SPI (SPI1 Master <-> SPI3 Slave)";
        case PERIPHERAL_I2C:   return "I2C (I2C1 Master <-> I2C2 Slave)";
        case PERIPHERAL_ADC:   return "ADC (ADC1)";
        case PERIPHERAL_OTA:   return "OTA (Bonus Firmware Update)";
        default:               return "UNKNOWN_PERIPHERAL";
    }
}

void recorder_logToConsole(const TestCommand_t *cmd, const TestResult_t *res, int transport_status) 
{
    printf("\n=======================================================\n");
    printf("                  TEST EXECUTION SUMMARY               \n");
    printf("=======================================================\n");
    printf("Test ID        : 0x%08X\n", cmd->test_id);
    printf("Target Periph  : %s\n", get_peripheral_name(cmd->peripheral));
    printf("Iterations     : %d\n", cmd->iterations);
    
    if (transport_status != 0) 
    {
        printf("Execution State: [ERROR] Network Timeout or Socket Drop out!\n");
    }
    else 
    {
        if (res->test_id != cmd->test_id) 
        {
            printf("Test State: [ERROR] Packet Sync Mismatch! (Expected ID: 0x%08X, Received: 0x%08X)\n", cmd->test_id, res->test_id);
        } 
        else if (res->result == TEST_PASS) 
        {
            printf("Test State: [PASS].\n");
        } 
        else if (res->result == TEST_FAIL) 
        {
            printf("Test State: [FAIL].\n");
        } 
        else 
        {
            printf("Test State: [UNKNOWN] Target returned unexpected Status Code: 0x%02X\n", res->result);
        }
    }
    printf("*******************************************************\n\n");
}

void recorder_logToFile(const char *filename, const TestCommand_t *cmd, const TestResult_t *res, int transport_status) 
{
    FILE *file = fopen(filename, "a+"); // open file in append mode, create if doesn`t exist
    if (!file) return;

    fseek(file, 0, SEEK_END); //  go to end of file
    if (ftell(file) == 0) // if this is a new file, write hadders
    {
        fprintf(file, "Timestamp,Test_ID,Peripheral,Iterations,Status,Raw_Firmware_Result\n");
    }

    time_t now = time(NULL); // get now time
    struct tm time_info; 
    char time_str[26];
    
    localtime_s(&time_info, &now);
    asctime_s(time_str, sizeof(time_str), &time_info); 
    time_str[strcspn(time_str, "\r\n")] = 0; // Remove formatting newline character

    const char *status_str = "UNKNOWN_ERROR";
    if (transport_status != 0) // Network error or timeout occurred
    {
        status_str = "TIMEOUT_OR_NET_ERROR";
    } 
    else 
    {
        if (res->test_id != cmd->test_id) {status_str = "ID_MISMATCH";}
        else if (res->result == TEST_PASS) {status_str = "PASSED";}
        else if (res->result == TEST_FAIL) {status_str = "FAILED";}
        else {status_str = "UNEXPECTED_RESULT_CODE";}
    }

    fprintf(file, "%s,0x%08X,%s,%u,%s,0x%02X\n", time_str, cmd->test_id, get_peripheral_name(cmd->peripheral), 
                    cmd->iterations, status_str, (transport_status == 0) ? res->result : 0x00);

    fclose(file);
}