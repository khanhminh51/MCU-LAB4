/*
 * scheduler.c
 *
 *  Created on: Nov 11, 2022
 *      Author: DELL
 */

#include "scheduler.h"

//void SCH_Init(void){
//	unsigned char i;
//	for(i = 0; i < SCH_MAX_TASKS; i++){
//		SCH_Delete_Task();
//	}
//	Error_Code_G = 0;
//	Timer_init();
//	Watchdog_init();
//}

unsigned char SCH_Add_Task(void (*pFunction)(), unsigned int DELAY, unsigned int PERIOD){
	unsigned char index = 0;
	while((SCH_tasks_G[index].pTask !=0) && (index < SCH_MAX_TASKS)){
		index++;
	}
	if(index == SCH_MAX_TASKS){
		Error_Code_G = ERROR_SCH_TOO_MANY_TASK;
		return SCH_MAX_TASKS;
	}
	SCH_tasks_G[index].pTask = pFunction;
	SCH_tasks_G[index].Delay = DELAY;
	SCH_tasks_G[index].Period = PERIOD;
	SCH_tasks_G[index].RunMe = 0;
	return index;
}

void SCH_Update(void){
	unsigned char index;
	for(index = 0; index <SCH_MAX_TASKS; index++){
		if(SCH_tasks_G[index].pTask){
			if(SCH_tasks_G[index].Delay == 0){
			SCH_tasks_G[index].Delay++;
				if(SCH_tasks_G[index].Period){
					SCH_tasks_G[index].Delay = SCH_tasks_G[index].Period;
					}
				}
			else{
			SCH_tasks_G[index].Delay--;
			}
		}
	}
}

void SCH_Dispatch_Tasks(void){
	unsigned char index;
	for(index = 0; index <SCH_MAX_TASKS; index++){
		if(SCH_tasks_G[index].RunMe > 0){
			(*SCH_tasks_G[index].pTask)();
			SCH_tasks_G[index].RunMe--;
			if(SCH_tasks_G[index].Period == 0){
				SCH_Delete_Task(index);
			}
		}
	}
	SCH_Report_Status();
	SCH_Go_To_Sleep();
}
unsigned char SCH_Delete_Task(const tByte TASK_INDEX){
	unsigned char Return_code;
	SCH_tasks_G[TASK_INDEX].pTask = 0x0000;
	SCH_tasks_G[TASK_INDEX].Delay = 0;
	SCH_tasks_G[TASK_INDEX].Period = 0;
	SCH_tasks_G[TASK_INDEX].RunMe = 0;

	return Return_code;
}
