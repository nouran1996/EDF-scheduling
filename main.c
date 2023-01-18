/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"
#include "queue.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )
	
TaskHandle_t ToggleTaskHandle = NULL;
TaskHandle_t Button_1_MonitorHandle = NULL;
TaskHandle_t Button_2_MonitorHandle = NULL;
TaskHandle_t Periodic_TransmitterHandle = NULL;
TaskHandle_t Uart_ReceiverHandle = NULL;
TaskHandle_t Load_1_SimulationHandle = NULL;
TaskHandle_t Load_2_SimulationHandle = NULL;
QueueHandle_t SendReceiverHandle = NULL;

TickType_t xLastWakeTime_1;
TickType_t xLastWakeTime_2;
TickType_t xLastWakeTime_3;
TickType_t xLastWakeTime_4;
TickType_t xLastWakeTime_5;
TickType_t xLastWakeTime_6;

float CPU_Load;
int System_Time;
int Task1_Time_In=0,Task1_Time_Out=0,Task1_Total_Time=0;
int Task2_Time_In=0,Task2_Time_Out=0,Task2_Total_Time=0;
int Task3_Time_In=0,Task3_Time_Out=0,Task3_Total_Time=0;
int Task4_Time_In=0,Task4_Time_Out=0,Task4_Total_Time=0;
int Task5_Time_In=0,Task5_Time_Out=0,Task5_Total_Time=0;
int Task6_Time_In=0,Task6_Time_Out=0,Task6_Total_Time=0;


/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/



/* Task to be created. */

void vApplicationTickHook( void )
{
	GPIO_write(PORT_0,PIN2,PIN_IS_HIGH);
	GPIO_write(PORT_0,PIN2,PIN_IS_LOW);
}

void Button_1_Monitor( void * pvParameters )
{
	int Button_1_State;
	char Button_1_array1[] = "1 falling edges" ;
	char Button_1_array2[] = "1 rising edges" ;
	xLastWakeTime_1 = xTaskGetTickCount();
	
	vTaskSetApplicationTaskTag( NULL,(void*)1);
 

    for( ;; )
    {
       
			Button_1_State=GPIO_read(PORT_0,PIN0);
			if(Button_1_State == 0)
			{
				xQueueSend(SendReceiverHandle,&Button_1_array1,50);
			}
			else if (Button_1_State == 1)
		  {
				xQueueSend(SendReceiverHandle,&Button_1_array2,50);
			}
			vTaskDelayUntil(&xLastWakeTime_1,50);
    }
}

void Button_2_Monitor( void * pvParameters )
{
	int Button_2_State;
	char Button_2_array1[] = "2 falling edges" ;
	char Button_2_array2[] = "2 rising edges" ;
	xLastWakeTime_2 = xTaskGetTickCount();
	
	vTaskSetApplicationTaskTag( NULL,(void*)2);
 

    for( ;; )
    {
       
			Button_2_State=GPIO_read(PORT_0,PIN1);
			if(Button_2_State == 0)
			{
				xQueueSend(SendReceiverHandle,&Button_2_array1,50);
			}
			else if (Button_2_State == 1)
		  {
				xQueueSend(SendReceiverHandle,&Button_2_array2,50);
			}
			vTaskDelayUntil(&xLastWakeTime_2,50);
    }
}

void Periodic_Transmitter( void * pvParameters )
{
	char array[] = "preiodic string every" ;
	xLastWakeTime_3 = xTaskGetTickCount();
	
	vTaskSetApplicationTaskTag( NULL,(void*)3);
 

    for( ;; )
    {
       xQueueSend(SendReceiverHandle,array,100);
			vTaskDelayUntil(&xLastWakeTime_3,100);
    }
}

void Uart_Receiver( void * pvParameters )
{
	 int8_t RX = 0;
	 xLastWakeTime_4 = xTaskGetTickCount();
	
	vTaskSetApplicationTaskTag( NULL,(void*)4);
  

    for( ;; )
    {
       xQueueReceive(SendReceiverHandle,&RX,50);
		   vSerialPutString(&RX, 50);
			 vTaskDelayUntil(&xLastWakeTime_4,20);
    }
}

void Load_1_Simulation( void * pvParameters )
{
	int execution_time_5ms = 0 ;
	xLastWakeTime_5 = xTaskGetTickCount();
	
	vTaskSetApplicationTaskTag( NULL,(void*)5);
 
    for( ;; )
    {
      for(execution_time_5ms=0;execution_time_5ms<=37400;execution_time_5ms++)
			{
			}
			vTaskDelayUntil(&xLastWakeTime_5,10);
    }
}
void Load_2_Simulation( void * pvParameters )
{
	int32_t execution_time_12ms  ;
	xLastWakeTime_6 = xTaskGetTickCount();
	
 vTaskSetApplicationTaskTag( NULL,(void*)6);

    for( ;; )
    {
			for(execution_time_12ms=0;execution_time_12ms<=89600;execution_time_12ms++)
			{
				execution_time_12ms=execution_time_12ms;
			}
			vTaskDelayUntil(&xLastWakeTime_6,100);
    }
}


/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();
	 /* Create the task, storing the handle. */
	     xTaskPeriodicCreate(
                    Button_1_Monitor,                  
                    "Button_1_Monitor Task",           
                    100,                               
                    ( void * ) 0,                      
                    1,                                 
                    &Button_1_MonitorHandle,
                      50  );       
										
				xTaskPeriodicCreate(
                    Button_2_Monitor,                 
                    "Button_2_Monitor Task",          
                    100,                               
                    ( void * ) 0,                      
                    1,                                 
                    &Button_2_MonitorHandle,
                    50  );      
										
				xTaskPeriodicCreate(
                    Periodic_Transmitter,              
                    "Periodic_Transmitter Task",       
                    100,                               
                    ( void * ) 0,                      
                    1,                                 
                    &Periodic_TransmitterHandle,
             					100					);   
										
				xTaskPeriodicCreate(
                    Uart_Receiver,                    
                    "Uart_Receiver Task",              
                    100,                               
                    ( void * ) 0,                      
                    1,                                 
                    &Uart_ReceiverHandle,
                        20	);       
										
       xTaskPeriodicCreate(
                    Load_1_Simulation,                
                    "Load_1_Simulation Task",          
                    100,                               
                    ( void * ) 0,                      
                    1,                                
                    &Load_1_SimulationHandle,          
                    10);      
										
			 xTaskPeriodicCreate(
                    Load_2_Simulation,                /* Function that implements the task. */
                    "Load_2_Simulation Task",         /* Text name for the task. */
                    100,                              /* Stack size in words, not bytes. */
                    ( void * ) 0,                     /* Parameter passed into the task. */
                    1,                                /* Priority at which the task is created. */
                    &Load_2_SimulationHandle,         /* Used to pass out the created task's handle. */
                    100 );                             /* periodicity at which the task is created. */

										
										
SendReceiverHandle=xQueueCreate(10,20*sizeof(int8_t));
	
    /* Create Tasks here */


	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 1000;
	T1TCR |= 0x1;
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/




