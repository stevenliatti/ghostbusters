/**
 * @file		trace_mgt.c
 * @brief		This file contains all the functions to manage the traces
 *
 * @author		Steven Liatti
 * @author		Orphée Antoniadis
 * @author		Raed Abdennadher
 * @bug			No known bugs.
 * @date		April 12, 2017
 * @version		1.0
 */

#include "game.h"

#define BUFFER_SIZE	1024

struct {
	trace_t buffer[BUFFER_SIZE];
	uint8_t read;
	uint8_t write;
} circ_buffer = {{},0,0};

/**
 * @brief 		Task with the lower priority. Note that configUSE_IDLE_HOOK
 * 				must be set to 1 in FreeRTOSConfig.h to activate this task.
 */
void vApplicationIdleHook(void) {
	while (circ_buffer.read != circ_buffer.write) {
		uart0_send((uint8_t*)&circ_buffer.buffer[circ_buffer.read], 8);
		circ_buffer.read = (circ_buffer.read + 1) % BUFFER_SIZE;
	}
}

/**
 * @brief 		Write a trace to a memory buffer. Note that this function is
 *              automatically called by FreeRTOS in privileged mode.
 *
 * @param		trace_id: trace ID. Usually the task number in FreeRTOS.
 * @param		val: 1 if task becomes active, 0 otherwise
 */
void write_trace(uint8_t trace_id, short val) {
	trace_t trace;
	trace.synchro = SYNCHRO_WORD;
	trace.sig_idx = trace_id;
	trace.val = val;
	trace.time = getTime(TIMER0);
	circ_buffer.buffer[circ_buffer.write] = trace;
	circ_buffer.write = (circ_buffer.write + 1) % BUFFER_SIZE;
}
