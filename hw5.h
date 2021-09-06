/*
 * Name: Zackary McClamma
 * Course: ECE 532
 * Assignment: Homework 5
 * Date: 18 NOV 2019
 * File: hw5.h
 *
 * */

#ifndef HW5_H_
#define HW5_H_

#include <sys/alt_irq.h>
#include <sys/alt_timestamp.h>

#define I2C_BASE 0x80000
#define UART_BASE 0x60000
#define RED_LED_BASE 0x90000
#define GREEN_LED_BASE 0xA0000

#define _500_MS_TICKS 23000000

#define TIMER_IRQ 2

typedef struct str_timer_regs{
	unsigned int stats;
	unsigned int control;
	unsigned int periodl;
	unsigned int periodh;
	unsigned int snapl;
	unsigned int snaph;
}timer_regs;

typedef volatile struct{
	unsigned int uart_rxdata;
	unsigned int uart_txdata;
	unsigned int uart_status;
	unsigned int uart_control;
	unsigned int uart_divisor;
	unsigned int uart_eop;
}uart_reg;

typedef volatile struct{
	unsigned int i2c_tfr_cmd;
	unsigned int i2c_rxdata;
	unsigned int i2c_ctrl;
	unsigned int i2c_iser;
	unsigned int i2c_isr;
	unsigned int i2c_status;
	unsigned int i2c_tfr_cmd_fifo_lvl;
	unsigned int i2c_rx_data_fifo_lvl;
	unsigned int i2c_scl_low;
	unsigned int i2c_scl_high;
	unsigned int i2c_sda_hold;
}i2c_reg;

typedef volatile struct{
	unsigned int data;
	unsigned int dir;
	unsigned int intmask;
	unsigned int edge;
	unsigned int outset;
	unsigned int outclear;
}gpio_regs;

void i2c_init(void);
void eep_write(unsigned short addr, unsigned char data);
unsigned char eep_read(unsigned short addr);
void printMenu(void);

void led_on(int* base);
void led_off(int* base);

void timer_isr(void);


#endif /* HW5_H_ */
