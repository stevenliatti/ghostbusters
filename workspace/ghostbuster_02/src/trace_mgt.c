
/*
 * Description: trace management using UART
 * Created on : 23.3.2017
 * Author     : VP
 */
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
#include <traces_ref.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "uart.h"
#include "timer.h"

struct {
	trace_t buffer[BUFFER_SIZE];
	uint8_t read;
	uint8_t write;
} circ_buffer = {{},0,0};

/* Description: write a trace to a memory buffer. Note that this function is
 *              automatically called by FreeRTOS in privileged mode.
 *
 * Parameters: trace_id: trace ID. Usually the task number in FreeRTOS.
 *             val: 1 if task becomes active, 0 otherwise
 */
void write_trace(uint8_t trace_id, short val) {
	//write_trace_ref(trace_id, val);		// to be replaced by your own implementation
	trace_t trace;
	trace.synchro = SYNCHRO_WORD;
	trace.sig_idx = trace_id;
	trace.val = val;
	trace.time = getTime(TIMER0);
	circ_buffer.buffer[circ_buffer.write] = trace;
	circ_buffer.write = (circ_buffer.write + 1) % BUFFER_SIZE;
}



void vApplicationIdleHook(void) {
	while(1) {
		// implement trace sending here after having set configUSE_IDLE_HOOK to 1 in FreeRTOSConfig.h
		uart0_send((uint8_t*)&circ_buffer.buffer[circ_buffer.read], 8);
		circ_buffer.read = (circ_buffer.read + 1) % BUFFER_SIZE;
		taskYIELD();		// force changement de contexte
	}
}


