/*
 * Name: Zackary McClamma
 * Course: ECE 532
 * Assignment: Homework 5
 * Date: 18 NOV 2019
 * File: main.c
 *
 * */
#include <stdio.h>
#include <system.h>
#include "hw5.h"
#include "includes.h"

OS_EVENT *tex;

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define TASK1_PRIORITY      2
#define TASK2_PRIORITY      1

/* Prints "Hello World" and sleeps for three seconds */
void task1(void* pdata)
{
  INT8U err;
  i2c_init();
  while (1)
  {
	int opt, addr, data, addrSet, dataSet = 0;
	unsigned char read;
	//display menu
	printf("Options:\r\n");
	printf("1) Write EEPROM\r\n");
	printf("2) Read EEPROM\r\n");
	printf("3) Turn on Red LED\r\n");
	printf("4) Turn off Red LED\r\n");
	scanf("%d", &opt);
	fflush(stdin);
	printf("You chose option %d\r\n",opt);
	switch(opt){

	case 1:
		addrSet = 0;
		dataSet = 0;
		while(addrSet == 0)
		{
			printf("Enter Address: ");
			scanf("%d", &addr);
			fflush(stdin);
			printf(" %d\r\n", addr);

			if((addr < 0) || (addr > 127))
			{
				printf("Invalid input try again\r\n");
			}
			else
			{
				addrSet = 1;
			}
		}
		while(dataSet == 0)
		{
			printf("Enter Data: ");
			scanf("%d", &data);
			fflush(stdin);
			printf(" %d\r\n", data);

			if (data > 255)
			{
				printf("Invalid input try again\r\n");
			}
			else
			{
				dataSet = 1;
			}
		}
		eep_write(addr, data);
		OSTimeDlyHMSM(0, 0, 1, 0);
		break;
	case 2:
		addrSet = 0;
		while(addrSet == 0)
		{
			printf("Enter Address: ");
			scanf("%d", &addr);
			fflush(stdin);
			printf("\r\n");
			if((addr < 0) || (addr > 127))
			{
				printf("Invalid input try again\r\n");
			}
			else
			{
				addrSet = 1;
			}
		}
		read = eep_read(addr);
		printf("Value at address %d is %d\r\n", addr, (int)read);
		OSTimeDlyHMSM(0, 0, 1, 0);
		break;
	case 3:
		led_on(RED_LED_BASE);
		OSTimeDlyHMSM(0, 0, 1, 0);
		break;
	case 4:
		led_off(RED_LED_BASE);
		OSTimeDlyHMSM(0, 0, 1, 0);
		break;
	default:
		printf("Invalid option please try again.");
		break;

	}
   // OSTimeDlyHMSM(0, 0, 1, 0);
	//OSMutexPost(tex);
	//OSTimeDlyHMSM(0, 0, 0, 500);
  }
}
/* Toggles the Green LED every 500ms */
void task2(void* pdata)
{
	while(1){
	  INT8U err;
	  OSMutexPend(tex,0,&err);
	  unsigned int* gled = GREEN_LED_BASE;

		  if(*gled == 0x01){
			  led_off(GREEN_LED_BASE);
		  }
		  else{
		  	led_on(GREEN_LED_BASE);
		  }
	  OSMutexPost(tex);
	  OSTimeDlyHMSM(0, 0, 0, 500);
	}
}
/* The main function creates two task and starts multi-tasking */
int main(void)
{
  INT8U err;
  tex = OSMutexCreate(1,&err);
  OSTaskCreateExt(task1,
                  NULL,
                  (void *)&task1_stk[TASK_STACKSIZE-1],
                  TASK1_PRIORITY,
                  TASK1_PRIORITY,
                  task1_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
              
               
  OSTaskCreateExt(task2,
                  NULL,
                  (void *)&task2_stk[TASK_STACKSIZE-1],
                  TASK2_PRIORITY,
                  TASK2_PRIORITY,
                  task2_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
  OSStart();
  return 0;
}

void led_on(int* base){
	INT8U err;
	OSMutexPend(tex,0,&err);
	*base = 0x01;
	OSMutexPost(tex);
	return;
}

void led_off(int* base){
	INT8U err;
	OSMutexPend(tex,0,&err);
	*base = 0x00;
	return;
}

void i2c_init(void){
	i2c_reg *reg = I2C_0_BASE;

	reg->i2c_scl_low = 1000;
	reg->i2c_scl_high = 1000;
	reg->i2c_sda_hold = 500;
	reg->i2c_tfr_cmd_fifo_lvl = 8;
	reg->i2c_rx_data_fifo_lvl = 8;
}

void eep_write(unsigned short addr, unsigned char data){
	i2c_reg* reg = I2C_0_BASE;
	INT8U err;
	unsigned char addr_low = addr >> 8;
	unsigned char addr_high = addr & 0x00FF;
	OSMutexPend(tex,0,&err);
	reg->i2c_ctrl = 0x1;
	reg->i2c_tfr_cmd = 0x2A0;
	reg->i2c_tfr_cmd = addr_high;
	reg->i2c_tfr_cmd = addr_low;
	reg->i2c_tfr_cmd = data | 0x100;

	while(reg->i2c_status != 0);
	reg->i2c_ctrl = 0x0;
	OSMutexPost(tex);
	OSTimeDlyHMSM(0, 0, 0, 5);
}

unsigned char eep_read(unsigned short addr){
	i2c_reg* reg = I2C_0_BASE;
	INT8U err;
	unsigned char addr_low = addr >> 8;
	unsigned char addr_high = addr & 0x00FF;
	unsigned char data;
	unsigned char tempData = 0;
	OSMutexPend(tex,0,&err);

	reg->i2c_ctrl = 0x1;
	reg->i2c_tfr_cmd = 0x2A0;
	reg->i2c_tfr_cmd = addr_high;
	reg->i2c_tfr_cmd = addr_low;
	reg->i2c_tfr_cmd = 0x2A1;
	reg->i2c_tfr_cmd = tempData | 0x100;


	while(reg->i2c_status != 0);
	data = reg->i2c_rxdata;
	reg->i2c_ctrl = 0x0;
	OSMutexPost(tex);
	return data;
}


