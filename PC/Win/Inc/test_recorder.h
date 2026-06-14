#ifndef TEST_RECORDER_H
#define TEST_RECORDER_H

#include "protocol.h"

void recorder_logToConsole(const TestCommand_t *cmd, const TestResult_t *res, int transport_status);

void recorder_logToFile(const char *filename, const TestCommand_t *cmd, const TestResult_t *res, int transport_status);

#endif // TEST_RECORDER_H