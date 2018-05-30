#include "Driver_USART.h"               // CMSIS Driver:USART
#include "LPC17xx.h"                    // Device header
#include "Driver_I2C.h"                 // ::CMSIS Driver:I2C
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include <stdio.h>
#include <stdlib.h>

#define SLAVE_I2C_ADDR       0xEC

extern ARM_DRIVER_USART Driver_USART1;
extern ARM_DRIVER_I2C Driver_I2C0;
extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

void Init_I2C(void){
	Driver_I2C0.Initialize(NULL);
	Driver_I2C0.PowerControl(ARM_POWER_FULL);
	Driver_I2C0.Control(	ARM_I2C_BUS_SPEED,				// 2nd argument = débit
							ARM_I2C_BUS_SPEED_STANDARD  );	// 100 kHz
	Driver_I2C0.Control(	ARM_I2C_BUS_CLEAR,
							0 );
}

void Init_UART(void){
	Driver_USART1.Initialize(NULL);
	Driver_USART1.PowerControl(ARM_POWER_FULL);
	Driver_USART1.Control( ARM_USART_MODE_ASYNCHRONOUS |
	ARM_USART_DATA_BITS_8 |
	ARM_USART_STOP_BITS_1 |
	ARM_USART_PARITY_NONE |
	ARM_USART_FLOW_CONTROL_NONE,
	115200);
	Driver_USART1.Control(ARM_USART_CONTROL_TX,1);
	Driver_USART1.Control(ARM_USART_CONTROL_RX,1);
}

int main (void){
	char tab[50];
	uint8_t valeurI2CLSB;
	uint8_t valeurI2CMSB;
	char affichage[20];
	uint8_t tabEnvoie1, tabEnvoie2;
	char I2C[2];
	
	Init_UART();
	Init_I2C();
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	
	
	while (1){
		tabEnvoie1=0x82;
		tabEnvoie2=0x83;
		while(Driver_USART1.GetStatus().tx_busy == 1);
		Driver_USART1.Send("#24510010400006010",18);
		Driver_I2C0.MasterTransmit(SLAVE_I2C_ADDR,&tabEnvoie1,1,false);
		while (Driver_I2C0.GetStatus().busy == 1);
		Driver_I2C0.MasterReceive(SLAVE_I2C_ADDR,&valeurI2CLSB,1,false);
		while (Driver_I2C0.GetStatus().busy == 1);
		Driver_I2C0.MasterTransmit(SLAVE_I2C_ADDR,&tabEnvoie2,1,false);
		while (Driver_I2C0.GetStatus().busy == 1);
		Driver_I2C0.MasterReceive(SLAVE_I2C_ADDR,&valeurI2CMSB,1,false);
		while (Driver_I2C0.GetStatus().busy == 1);
		I2C[0]=valeurI2CMSB;
		I2C[1]=valeurI2CLSB;
		GLCD_DrawString(10,10,I2C);
		
		
		//sprintf(affichage,"%d",valeurI2C);
		//GLCD_DrawString(10,10,affichage);
		
	}
	return 0;
}