/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * robot.c: Control a Universal Robots UR5e over PMOD 1
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
//#include "xil_printf.h"
#include "xgpiops.h"

#define GPIO_DEVICE_ID  	XPAR_XGPIOPS_0_DEVICE_ID
#define PIN_1 54
#define PIN_2 54+1
#define PIN_3 54+2
#define PIN_4 54+3

XGpioPs Gpio;
XGpioPs_Config *ConfigPtr;

void display_menu();
void init_gpio();
void clear_all();
void set_pin(u32 pin_number);

int main()
{
    init_platform();
    init_gpio();

    print("Control an industrial robot from the MiniZed!\n\r");

	char key_input;

	while(1){

		display_menu();
		read(1, (char*)&key_input, 1);

		switch (key_input) {
			case '1':
				set_pin(PIN_1);
				break;
			case '2':
				set_pin(PIN_2);
				break;
			case '3':
				set_pin(PIN_3);
				break;
			case '4':
				set_pin(PIN_4);
				break;
			case '0':
				clear_all();
				break;
		}
	}

    cleanup_platform();
    return 0;
}

void display_menu()
{
	print("\r\nSelect a robot program to run\r\n");
	print("  1-4 to run a program\r\n");
	print("  0 to stop\r\n");
}

/*
 * Setup GPIOs as output
 */
void init_gpio()
{
	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	XGpioPs_CfgInitialize(&Gpio, ConfigPtr, ConfigPtr->BaseAddr);

	XGpioPs_SetDirectionPin(&Gpio, PIN_1, 1);
	XGpioPs_SetOutputEnablePin(&Gpio, PIN_1, 1);
	XGpioPs_WritePin(&Gpio, PIN_1, 0x0);

	XGpioPs_SetDirectionPin(&Gpio, PIN_2, 1);
	XGpioPs_SetOutputEnablePin(&Gpio, PIN_2, 1);
	XGpioPs_WritePin(&Gpio, PIN_2, 0x0);

	XGpioPs_SetDirectionPin(&Gpio, PIN_3, 1);
	XGpioPs_SetOutputEnablePin(&Gpio, PIN_3, 1);
	XGpioPs_WritePin(&Gpio, PIN_3, 0x0);

	XGpioPs_SetDirectionPin(&Gpio, PIN_4, 1);
	XGpioPs_SetOutputEnablePin(&Gpio, PIN_4, 1);
	XGpioPs_WritePin(&Gpio, PIN_4, 0x0);
}

/*
 * Set all 4 pins low
 */
void clear_all()
{
	XGpioPs_WritePin(&Gpio, PIN_1, 0);
	XGpioPs_WritePin(&Gpio, PIN_2, 0);
	XGpioPs_WritePin(&Gpio, PIN_3, 0);
	XGpioPs_WritePin(&Gpio, PIN_4, 0);
}

/*
 * Set a single pin high
 */
void set_pin(u32 pin_number)
{
	clear_all();
	XGpioPs_WritePin(&Gpio, pin_number, 0x1);
}
