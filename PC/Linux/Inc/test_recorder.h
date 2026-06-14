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

#include "protocol.h"

void recorder_logToConsole(const TestCommand_t *cmd, const TestResult_t *res, int transport_status);

void recorder_logToFile(const char *filename, const TestCommand_t *cmd, const TestResult_t *res, int transport_status);

#endif // TEST_RECORDER_H