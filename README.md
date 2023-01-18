# EDF-scheduler
EDF-scheduling implemented in Free RTOS

# Earliest deadline first (EDF)
 dynamic priority scheduling implemented in real-time operating systems
 
 ### tool and drivers
 
 microcontroller used is LPC2129 ( an ARM7TDMI-S based microcontroller)
 IDE used is Keil
 this porject is written in c programming language and uses URAT and GPIO drivers which are not included
 
 ### Aim of the project 
 
 Calculation of The CPU Load
 Calculation of Rate-Monotonic Utilization of the application
 Time Demand Analysis
 
 ### file description 
 
 FreeRTOSConfig.h: changes and added macros in the configration inorder for the edf scheduler to run ex: ``` configUSE_EDF_SCHEDULER ```
 
 tasks.c: changes added for the edf: 
 1-new RedyList by task's deadline
 2-put the deadline value in xStateListItem
 3-Store the period in tick of the task.
 4-Definition of xTaskPeriodicCreate 
 5-initialize the period insert the period value in the generic list iteam before to add thetask in RL
 6-Udate The Deadline of Current Task
 7-Create the idle task without storing its handle
 8- the idle task is ready to execute if there is no ather tasks in the ready list.
 9-pointer to the new running task
 
 main.c: applicayion to Verify that the edf scheduler is working well using queue and URAT and to calculation of The CPU Load and task's excution time 
 making sure that the application is schedulable
 
 
 ### keil simulation results
 
 <img src="https://user-images.githubusercontent.com/78441476/213159890-304c406c-663d-4cc2-b5ef-1ef53b64bcd2.png" width="800" >
