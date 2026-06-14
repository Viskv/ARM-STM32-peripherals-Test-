/*
 * @file 	test_select_ctrl.h
 * @brif
 * 			get Test Command from UDP server, run the relavant peripheral test,
 * 			and send the test results to tesults out.
 */

#ifndef TEST_SELECT_CTRL_H_
#define TEST_SELECT_CTRL_H_

void TestSelctCtrlTask(void *args); // pass NULL if their are no args input

#endif /* TEST_SELECT_CTRL_H_ */
