/******************************************************************************
 * FILE NAME:    Scheduler.c
 * DESCRIPTION:
 * 
 * DATE BEGUN:  
 * BY:          
 * PRODUCT NAME:
 * APPLICATION:
 * TARGET H/W:
 * DOC REF:
 *****************************************************************************
 */

/*****************************************************************************
** #include 
*****************************************************************************/
#include "stdint.h"
#include "stdbool.h"


//! \name finit state machine state
//! @{
typedef enum {
    fsm_rt_err          = -1,    //!< fsm error, error code can be get from other interface
    fsm_rt_cpl          = 0,     //!< fsm complete
    fsm_rt_on_going     = 1,     //!< fsm on-going
} fsm_rt_t;
//! @}

static fsm_rt_t task_print(void);
static fsm_rt_t task_delay(void);
static fsm_rt_t print(void);
static fsm_rt_t delay(void);
static bool s_bFlag = false;
/****************************************************************************/
/**
 * Function Name: void scheduler( void )
 * Description: none
 *
 * Param:   none
 * Return:  none
 * Author:  
 ****************************************************************************/
void scheduler( void )
{	
    while(!serial_out('z'));
    while(!serial_out('t'));
    while(!serial_out('2'));
    while(!serial_out('0'));
    while(!serial_out('1'));
    while(!serial_out('2'));
    while(!serial_out('\r'));
    while(!serial_out('\n'));
    
    /* loop here forever */
	while(1){    
        task_print();     
        task_delay();     
	}
		
}

#define TASK_PRINT_RESET_FSM()	\
	do{	\
		s_tTaskPrintState = TASK_PRINT_START;	\
	}while(0)	
static fsm_rt_t task_print(void)
{
	static enum{
		TASK_PRINT_START=0,
		TASK_PRINT_ING
	}s_tTaskPrintState = TASK_PRINT_START;
	switch (s_tTaskPrintState){
		case TASK_PRINT_START:
			if(s_bFlag){
				s_tTaskPrintState = TASK_PRINT_ING;
			}
			break;
			
		case TASK_PRINT_ING:
			if(fsm_rt_cpl == print()){
				TASK_PRINT_RESET_FSM();
				return fsm_rt_cpl;
			}
			break;
	}
	return fsm_rt_on_going;
}

#define TASK_DELAY_RESET_FSM()	\
	do{	\
		s_tTaskDelayState = TASK_DELAY_START;	\
	}while(0)	
static  fsm_rt_t task_delay(void)
{
	static enum{
		TASK_DELAY_START=0,
		TASK_DELAY_ING
	}s_tTaskDelayState = TASK_DELAY_START;
	switch(s_tTaskDelayState){
		case TASK_DELAY_START:
			s_bFlag = false;
		
		case TASK_DELAY_ING:
			if(fsm_rt_cpl == delay()){
				s_bFlag = true;
				TASK_DELAY_RESET_FSM();
				return fsm_rt_on_going;
			}
	}
	return fsm_rt_on_going;
}

#define PRINT_RESET_FSM()	\
	do{	\
		s_tPrintState = PRINT_START;	\
	}while(0)
	
static fsm_rt_t print(void)
{
	static enum{
		PRINT_START = 0,
		PRINT_OUT_H,
		PRINT_OUT_E,
		PRINT_OUT_L1,
		PRINT_OUT_L2,
		PRINT_OUT_O,
		PRINT_OUT_R,
		PRINT_OUT_N
	}s_tPrintState = PRINT_START;
	switch(s_tPrintState){
		case PRINT_START:
			s_tPrintState = PRINT_OUT_H;
		
		case PRINT_OUT_H:
			if(serial_out('h')){
				s_tPrintState = PRINT_OUT_E;
			}
			break;
		case PRINT_OUT_E:
			if(serial_out('e')){
				s_tPrintState = PRINT_OUT_L1;
			}
			break;
		case PRINT_OUT_L1:
			if(serial_out('l')){
				s_tPrintState = PRINT_OUT_L2;
			}
			break;
		case PRINT_OUT_L2:
			if(serial_out('l')){
				s_tPrintState = PRINT_OUT_O;
			}
			break;
		case PRINT_OUT_O:
			if(serial_out('o')){
				s_tPrintState = PRINT_OUT_R;
			}
			break;
		case PRINT_OUT_R:
			if(serial_out('\r')){
				s_tPrintState = PRINT_OUT_N;
			}
			break;
		case PRINT_OUT_N:
			if(serial_out('\n')){
				PRINT_RESET_FSM();
				return fsm_rt_cpl;
			}
			break;
	}
	return fsm_rt_on_going;
}

#define DELAY_RESET_FSM()	\
	do{	\
		s_tDelaytState = DELAY_START;	\
	}while(0)
#define DEALY_TIME (0x2ffff)
	
static fsm_rt_t delay(void)
{
	static enum{
		DELAY_START = 0,
		DELAY
	}s_tDelaytState = DELAY_START;
	static uint32_t s_wDelayCounter = 0;
	switch(s_tDelaytState){
		case DELAY_START:
			s_wDelayCounter = 0;
			s_tDelaytState = DELAY;

		case DELAY:
			s_wDelayCounter ++;
			if(s_wDelayCounter > DEALY_TIME){	
				s_wDelayCounter = 0;				
				DELAY_RESET_FSM();
				return fsm_rt_cpl;
			}
			break;
	}			
	return fsm_rt_on_going;
}

/*****************************************************************************
** End File
*****************************************************************************/



