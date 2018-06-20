#include "LPC17xx.h"                    // Device header
#include "Board_ADC.h"                  // ::Board Support:A/D Converter
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GPIO_LPC17xx.h"               // Keil::Device:GPIO
#include <math.h>
#include <stdio.h>

extern GLCD_FONT GLCD_Font_16x24;

int main (void)
{
	
	int16_t niveau;
	//int tour;int valVT;
	char affichage[50];
	
	ADC_Initialize();			//initialisation des ports analogiques 
	
	GLCD_Initialize();		//initialisation de l'ecran LCD 
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	
	while(1) {
		
	ADC_StartConversion();		//se connecte au capteur Analogique 
		
	while(ADC_ConversionDone() !=0);
	niveau = ADC_GetValue();			//recupere la valeur entre 4096 et 0
		
		
		/////////////////////////////////////////////// palier determinant le niveau d'eau present////////////////////////////////////////
					if(niveau <= 1000)
		{
			niveau = (niveau-1000)/(-1);
			sprintf(affichage," ");
			GLCD_DrawString(0, 3*24, affichage);}
		
					
			
			/////////////////////////////////////////affichage de la valeur a envoyer avec le niveau d'eau////////////////////

	sprintf(affichage,"Niveau= %04d",niveau);
			GLCD_DrawString(0, 2*24, affichage);
		
	} 
	
	return 0;
}
