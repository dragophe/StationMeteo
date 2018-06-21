#include "Driver_USART.h"               // CMSIS Driver:USART
#include "LPC17xx.h"                    // Device header
#include "Driver_I2C.h"                 // ::CMSIS Driver:I2C
#include "Board_GLCD.h"                 // ::Board Support:Graphic LCD
#include "GLCD_Config.h"                // Keil.MCB1700::Board Support:Graphic LCD
#include "Board_ADC.h"                  // ::Board Support:A/D Converter
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SLAVE_I2C_ADDR       0x76 //Adresse du capteur I2C Pression/Température

extern ARM_DRIVER_USART Driver_USART1;
extern ARM_DRIVER_I2C Driver_I2C0;
extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

int niveau();
int trigger_signal();


//------------------INITIALISATION I2C-------------//
void Init_I2C(void){
	Driver_I2C0.Initialize(NULL);
	Driver_I2C0.PowerControl(ARM_POWER_FULL);
	Driver_I2C0.Control(	ARM_I2C_BUS_SPEED,				// 2nd argument = débit
							ARM_I2C_BUS_SPEED_STANDARD  );	// 100 kHz
	Driver_I2C0.Control(	ARM_I2C_BUS_CLEAR,
							0 );
}

//------------------INITIALISATION UART-------------//
void Init_UART(void){
	Driver_USART1.Initialize(NULL);
	Driver_USART1.PowerControl(ARM_POWER_FULL);
	Driver_USART1.Control( ARM_USART_MODE_ASYNCHRONOUS |
	ARM_USART_DATA_BITS_8 |
	ARM_USART_STOP_BITS_1 |
	ARM_USART_PARITY_NONE |
	ARM_USART_FLOW_CONTROL_NONE,
	9600);
	Driver_USART1.Control(ARM_USART_CONTROL_TX,1);
	Driver_USART1.Control(ARM_USART_CONTROL_RX,1);
}

int main (void){
	
	//---------------Déclaration-----------------------// 
	uint8_t tabEnv[10],rst[10],lala[10];
	uint8_t I2C[6];
	uint32_t temp;
	int i = 0;
	int temperature,pression,humidite,vent,niveau_eau;
	char str_press[50],str_temp[50],trame[50];
	
	//------Initialisation des périphériques-----------//
	Init_UART();
	Init_I2C();
	GLCD_Initialize();
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	
	rst[0] = 0x06; //Commande reset du capteur I2C
	lala[0] = 0x48; //Commande d'initialisation du capteur I2C
	tabEnv[0] = 0x10; //Commande de la demande pression/température
	
	Driver_I2C0.MasterTransmit(SLAVE_I2C_ADDR,rst,1,false); //Reset du capteur
	while (Driver_I2C0.GetStatus().busy == 1);
	
	while (1){

		//--------------------------------------------Transmition I2C (barometer/temp)----------------------------------------------------------------------/
		Driver_I2C0.MasterTransmit(SLAVE_I2C_ADDR,lala,1,false); //Envoie de la trame d'initialisation du capteur
		while (Driver_I2C0.GetStatus().busy == 1);
		Driver_I2C0.MasterTransmit(SLAVE_I2C_ADDR,tabEnv,1,false); //Envoie de la trame de demande des valeurs de pression/température
		while (Driver_I2C0.GetStatus().busy == 1);
		Driver_I2C0.MasterReceive(SLAVE_I2C_ADDR,I2C,6,false); //Récéption des valeurs dans un tableau
		while (Driver_I2C0.GetStatus().busy == 1);
		
		//Mise en places des différents octets de la valeurs pression dans un int/
		pression = I2C[5];
		pression += I2C[4]<<8;
		pression += I2C[3]<<16;
		pression /= 10;
		
		//Mise en places des différents octets de la valeurs température dans un int/
		temperature = I2C[2];
		temperature += I2C[1]<<8;
		temperature += I2C[0]<<16;
		temperature /= 10;
				
		//--------------------------------------------Transmition Digital(humidity)---------------------------------------------------------/
		humidite=trigger_signal(); //Récupération de la valeur de l'humidité
		
		//--------------------------------------------Transmition Analogic(water sensor)---------------------------------------------------/
		niveau_eau=niveau(); //Récupération de la valeur du niveau d'eau
			
		//--------------------------------------------Transmition Bluetooth----------------------------------------------------------------------/
		sprintf(trame,"#%3d%3d%5d020%3d",temperature,humidite,pression,niveau_eau);	
		GLCD_DrawString(10,150,trame);
		while(Driver_USART1.GetStatus().tx_busy == 1);
		Driver_USART1.Send(trame,18);
		
		for(i=0;i<30000000;i++);
		
	}
	return 0;
}

int trigger_signal(){
	
	int tab[16];
	int hum = 0;
	int i=0;
	int j=0;
	int temps;
	int humidite=0;
	
	LPC_GPIO4->FIODIR3 |=0x20; //P4.29 en entrée
	LPC_GPIO4->FIOPIN3 &=0xDF;
	for(i=0;i<300000;i++);
	LPC_GPIO4->FIOPIN3 |=0x20; //Déclenchement du signal
	LPC_GPIO4->FIODIR3 &=0xDF; //P4.29 en sortie
	
	
	//Attente de la trame d'initialisation/
	while((LPC_GPIO4->FIOPIN3&0x20)==0x20);
	while((LPC_GPIO4->FIOPIN3&0x20)==0x00);
	while((LPC_GPIO4->FIOPIN3&0x20)==0x20);
	while((LPC_GPIO4->FIOPIN3&0x20)==0x00);
	
	for(j=31;j>0;j--)
	{
		while((LPC_GPIO4->FIOPIN3&0x20)==0x00);//attente 1
			
		temps=0;
	
		while((LPC_GPIO4->FIOPIN3&0x20)==0x20)/*attente 0*/			temps++;
	
		if (temps>220)
			hum|=(1<<j); //Le bit est à 1
		else
			hum&=~(1<<j); //Le bit est à 0
	}
		
	tab[0]=hum>>24;
	tab[1]=0;
	
	humidite=tab[0];
	
	return humidite;
}



int niveau (void)
{
	
	int16_t niveau;
	
	ADC_Initialize();			//initialisation des ports analogiques 
	ADC_StartConversion();		//se connecte au capteur Analogique 
		
	while(ADC_ConversionDone() !=0);
	niveau = ADC_GetValue();			//recupere la valeur entre 4096 et 0
	if(niveau <= 1000)
			niveau = (niveau-1000)/(-1);

	return niveau;
}