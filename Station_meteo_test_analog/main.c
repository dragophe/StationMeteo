#include "LPC17xx.h"                    // Device header
#include "Board_ADC.h"                  // ::Board Support:A/D Converter
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GPIO_LPC17xx.h"               // Keil::Device:GPIO
#include <math.h>
#include <stdio.h>

extern GLCD_FONT GLCD_Font_16x24;

int main (void)
{
	
	int recup;
	int b=4275;
	int R0=100000;
	float R, temp;
	char affichage[50];
	
	ADC_Initialize();
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	
	while(1) {
	ADC_StartConversion();
	while(ADC_ConversionDone() !=0);
	recup=ADC_GetValue();
	//R=(1023/recup)-1;
	//R=R*R0;
	//temp = 1/(log(R/R0)/b+1/298.15)-273.15;
	sprintf(affichage,"temperature = %d",recup);
	GLCD_DrawString(10,10,affichage);
	} 
	
	return 0;
}
