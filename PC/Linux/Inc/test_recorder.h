/*
 * @file    test_record.h
 * @brif    Persistent test record management.
 *
 *          Records are appended to a CSV file (RECORDS_FILE) with one
 *          line per test.  A separate counter file (TEST_ID_FILE) keeps
 *          track of the next available Test-ID across programme runs.
 *
 *          CSV format (one line per test):
 *            TEST-ID   , DATE & TIME          , DURATION(Sec) , PERIPHERAL , ITER , RESULT
 *            0x00000001, 2024-01-15 10:30:45  , 2.345         , UART       , 3    , SUCCESS
 */

#ifndef TEST_RECORDER_H
#define TEST_RECORDER_H

#include <stdint.h>

#define RECORDED_FILE_NAME "test_records"
#define TEST_ID_FILE_NAME  "test_id"

typedef struct
{
    uint32_t test_id;
    char     timestamp[32];   // "YYYY-MM-DD HH:MM:SS"
    double   duration_sec;
    uint8_t  peripheral;
    uint8_t  iterations;
    uint8_t  result;
} TestRecord_t;

void print_by_id(uint32_t test_id); // Print the records matching the given Test-ID

void print_all(); // Print all records in CSV

int save_record(const TestRecord_t *rec); // Append one record to CSV file

const char get_peripheral_name(uint8_t peripheral);

#endif // TEST_RECORDER_H